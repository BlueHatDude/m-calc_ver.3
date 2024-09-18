#include "mcalc3.h"
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h> /* only for debugging purposes */


typedef float float32_t;
typedef double float64_t;


#define MAX_TOKENS 100


enum TokenType {

    /* operators */
    OP_ADD,
    OP_SUB,
    OP_MULT,
    OP_DIV,
    OP_EXP,

    /* grouping */
    PAR_LEFT,
    PAR_RIGHT,

    /* types */
    TYPE_INTEGER,
    TYPE_DECIMAL,
    TYPE_EMPTY,

    /* functions */
    FN_SIN,
    FN_COS,
    FN_TAN,
    FN_LOG_10,
    FN_LOG_E,

    /* constants */
    CONSTANT_PI,
    CONSTANT_E

};


struct EquToken {
    
    enum TokenType type;
    
    union {
        int64_t ivalue;
        float64_t fvalue;
    }; 

    struct EquToken* left_operand;
    struct EquToken* right_operand;

};


struct TokensList {
    /* array of tokens */
    struct EquToken tokens[MAX_TOKENS];
    /* maximum number of tokens which can be stored */
    unsigned int capacity;
    /* current index of tokens */
    unsigned int tkns_pos;
    /* positions of operators in tokens */
    unsigned int operators[MAX_TOKENS / 2];
    unsigned int op_pos;
};


/* ===== Utility Functions =====*/

static inline void clear_string(char* str, const size_t len) {
    memset(str, '\0', len);
}


/**
 * @brief checks to see if substring is in string at str_start 
 * 
 * @return true 
 * @return false 
 */
static inline bool string_at(const char sub_str[], const char* str_start) {
    return (strstr(str_start, sub_str) == str_start);
}


/* ===== Calculator Functions =====*/

struct TokensList init_list(void) {
    struct TokensList list;
    for (int i = 0; i < MAX_TOKENS; i++)
        list.tokens[i].type = TYPE_EMPTY;
    list.capacity = MAX_TOKENS;
    list.tkns_pos = 0;

    return list;
}


static inline void set_token(struct EquToken* dest, enum TokenType type, double value) {
    dest->type = type;

    if (type == TYPE_INTEGER) {
        dest->ivalue = (int64_t) value;
    } else if (type == TYPE_DECIMAL) {
        dest->fvalue = value;
    }
}


/**
 * @brief 
 * 
 * @param ref 
 * @param tkns 
 * @param pos 
 * @param max 
 * @return MC3_ErrorCode 
 */
static MC3_ErrorCode add_token(const struct EquToken* ref, struct TokensList* list) {
    if (list->tkns_pos >= list->capacity) {
        return MC3_TOKENS_LIMIT_REACHED;
    }

    double value;
    if (ref->type == TYPE_INTEGER) {
        value = ref->ivalue;
    } else if (ref->type == TYPE_DECIMAL) {
        value = ref->fvalue;
    }

    set_token(&list->tokens[list->tkns_pos], ref->type, value);
    list->tkns_pos++;

    return MC3_NO_ERROR;
}


static MC3_ErrorCode read_number(const char* equ, struct EquToken* tkn, size_t* index) {
    const size_t j = (*index);
    bool is_decimal = false;
    while (isdigit(equ[*index]) || equ[*index] == '.') {
        if (equ[*index] == '.')
            is_decimal = true;
        
        (*index)++;
    }

    if (is_decimal) {
        set_token(tkn, TYPE_DECIMAL, strtod(&equ[j], NULL));
    } 
    else {
        set_token(tkn, TYPE_INTEGER, strtol(&equ[j], NULL, 10));
    }

    /* prevents position pointer from skipping over character after number */
    (*index)--;

    return MC3_NO_ERROR;
}

static bool is_operator(char c) {
    switch (c) {
        case '+': case '-': case '*': case '/': case '^':
            return true;
        default:
            return false;
    }
}


static enum TokenType char_to_type(char c) {
    switch (c) {
        case '+': return OP_ADD;
        case '-': return OP_SUB;
        case '*': return OP_MULT;
        case '/': return OP_DIV;
        case '^': return OP_EXP;
        default:  return TYPE_EMPTY;
    }
}


static bool is_grouping(char c) {
    return (c == '(') || (c == ')');
}


/**
 * @brief tokenizes string `equ` and reads into `tkns`
 * 
 * @param equ 
 * @param tkns 
 * @param tkns_size 
 * @return MC3_ErrorCode 
 */
static MC3_ErrorCode tokenize(const char* equ, struct TokensList* list) {
    struct EquToken ref_token;
    const size_t EQU_LENGTH = strlen(equ);

    for (size_t i = 0; i < EQU_LENGTH; i++) {
        /* checking for numbers, operators, and grouping */
        if (is_operator(equ[i])) {
            list->operators[list->op_pos] = i;
            list->op_pos++;
            enum TokenType type = char_to_type(equ[i]);
            set_token(&ref_token, type, 0);
            
        } else if (is_grouping(equ[i])) {
            if (equ[i] == '(')
                set_token(&ref_token, PAR_LEFT, 0);
            else
                set_token(&ref_token, PAR_RIGHT, 0);

        } else if (isdigit(equ[i])) {
            read_number(equ, &ref_token, &i);
        }

        /* checking for functions */
        else if (string_at("sin", &equ[i])) {
            set_token(&ref_token, FN_SIN, 0);
        }
        else if (string_at("cos", &equ[i])) {
            set_token(&ref_token, FN_COS, 0);
        } 
        else if (string_at("tan", &equ[i])) {
            set_token(&ref_token, FN_TAN, 0);
        } 
        else if (string_at("log", &equ[i])) {
            set_token(&ref_token, FN_LOG_10, 0);
        } 
        else if (string_at("ln", &equ[i])) {
            set_token(&ref_token, FN_LOG_E, 0);
        }
        
        /* checking for constants */
        else if (string_at("pi", &equ[i])) {
            set_token(&ref_token, CONSTANT_PI, 0);
        }
        else if (string_at("e", &equ[i])) {
            set_token(&ref_token, CONSTANT_E, 0);
        }
        else {
            ref_token.type = TYPE_EMPTY;
        }

        /* adding token to tokens array */
        if (ref_token.type != TYPE_EMPTY) {
            add_token(&ref_token, list);
        }
    }

    return MC3_NO_ERROR;
}


/**
 * @brief helper function for parse_tokens
 * 
 * @return MC3_ErrorCode 
 */
static MC3_ErrorCode sub_parse_tokens(void) {

}


/**
 * @brief creates parse tree using tokens
 * 
 * @param tkns 
 * @param size 
 * @return MC3_ErrorCode 
 */
static MC3_ErrorCode parse_tokens(struct TokensList* list) {
    /* [INT, ADD, INT, SUB, INT, MULT, INT, DIV, INT, EXP, INT] */
    (void) list;
    return MC3_NO_ERROR;
}


/* ===== Error Handling Functions =====*/

const char* getErrorString(const MC3_ErrorCode err) {
    switch (err) {
    case MC3_NO_ERROR:
        return "No error";
    case MC3_TOKENS_LIMIT_REACHED:
        return "Tokens limit Reached. Expression too long.";
    case MC3_INVALID_CHARACTER_FOUND:
        return "Invalid character was found.";
    default:
        return "Invalid Error Code";
    }
}


/* ===== Debugging Functions =====*/

/* DEBUGGING */
void print_token(const struct EquToken tkn) {
    switch (tkn.type) {
    case OP_ADD:
        printf("OP_ADD");
        break;
    case OP_SUB:
        printf("OP_SUB");
        break;
    case OP_MULT:
        printf("OP_MULT");
        break;
    case OP_DIV:
        printf("OP_DIV");
        break;
    case OP_EXP:
        printf("OP_EXP");
        break;
    case PAR_LEFT:
        printf("PAR_LEFT");
        break;
    case PAR_RIGHT:
        printf("PAR_RIGHT");
        break;
    case TYPE_INTEGER:
        printf("TYPE_INTEGER: %ld", tkn.ivalue);
        break;
    case TYPE_DECIMAL:
        printf("TYPE_DECIMAL: %lf", tkn.fvalue);
        break;
    case TYPE_EMPTY:
        printf("TYPE_EMPTY");
        break;
    case FN_SIN:
        printf("FN_SIN");
        break;
    case FN_COS:
        printf("FN_COS");
        break;
    case FN_TAN:
        printf("FN_TAN");
        break;
    case FN_LOG_10:
        printf("FN_LOG_10");
        break;
    case FN_LOG_E:
        printf("FN_LOG_E");
        break;
    case CONSTANT_PI:
        printf("CONSTANT_PI");
        break;
    case CONSTANT_E:
        printf("CONSTANT_E");
        break;
    }
}


/* DEBUGGING */
void print_tokens(const struct TokensList* list) {
    for (size_t i = 0; i < list->tkns_pos; i++) {
        printf("%-2zu: ", i);
        print_token(list->tokens[i]);
        puts("");
    }
}


/* DEBUGGING */
void print_compact_tokens(const struct TokensList* list) {
    printf("[ ");
    for (unsigned int i = 0; i < list->tkns_pos; i++) {
        print_token(list->tokens[i]);
        printf(", ");
    }
    puts("]");
}


/* DEBUGGING */
void print_tokens_full(const struct TokensList* list) {
    for (unsigned int i = 0; i < list->tkns_pos; i++) {
        printf("%02u: ", i);
        print_token(list->tokens[i]);
        printf(" | left: ");
        if (list->tokens[i].left_operand == NULL) {
            printf(" | left: NULL");
        } else {
            printf(" | left: ");
            print_token(*(list->tokens[i].left_operand));
        }
        printf(" | right: ");
        if (list->tokens[i].right_operand == NULL) {
            printf(" | right: NULL");
        } else {
            printf(" | right: ");
            print_token(*(list->tokens[i].left_operand));
        }
        puts("");
    }
}


/**
 * @brief evaluates a mathematical expression from a string, returning the result as a double.
 * Calculator supports all basic arithmetic operators: +, -, *, /, and ^. Also supports basic  
 * scientific functions such as ln, sin and constants such as e and pi. The calculator follows 
 * standard PEMDAS order of operations. 
 * 
 * @param equ 
 * @param err if parameter is NULL, then the error will not be set. Otherwise, the value of any error
 * that occurs is written to `err`.
 * @return double 
 */
double MC3_evaluate(const char *equ, MC3_ErrorCode* err) {
    (void) equ;
    
    struct TokensList tokens_list = init_list();
    MC3_ErrorCode error_code = MC3_NO_ERROR;

    error_code = tokenize(equ, &tokens_list);
    error_code = parse_tokens(&tokens_list);

    print_compact_tokens(&tokens_list);

    if (err != NULL) 
        *err = error_code;

    return 0.0;
}
