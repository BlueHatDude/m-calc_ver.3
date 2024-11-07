#include <stdint.h>
#include "mcalc3.h"
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h> /* only for debugging purposes */


#define MAX_TOKENS 100


/* ===== String Functions =====*/

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

/* ===== Data Types  =====*/

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
        double fvalue;
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
    unsigned int operators[MAX_TOKENS];
    unsigned int op_pos;
};

/* ===== Token Functions =====*/

static enum TokenType char_to_type(char ch) {
    switch (ch) {
        case '+': return OP_ADD;
        case '-': return OP_SUB;
        case '*': return OP_MULT;
        case '/': return OP_DIV;
        case '^': return OP_EXP;
        case '(': return PAR_LEFT;
        case ')': return PAR_RIGHT;
        default:  return TYPE_EMPTY;
    }
}

static enum TokenType str_to_fn(const char* str) {
    if (strncmp(str, "sin", 3) == 0) {
        return FN_SIN;
    } else if (strncmp(str, "cos", 3) == 0) {
        return FN_COS;
    } else if (strncmp(str, "tan", 3) == 0) {
        return FN_TAN;
    } else if (strncmp(str, "log", 3) == 0) {
        return FN_LOG_10;
    } else if (strncmp(str, "ln", 2) == 0) {
        return FN_LOG_E;
    } else {
        return TYPE_EMPTY;
    }
}

static const char* fn_type_to_str(enum TokenType type) {
    switch (type) {
        case FN_SIN: return "sin";
        case FN_COS: return "cos";
        case FN_TAN: return "tan";
        case FN_LOG_10: return "log";
        case FN_LOG_E: return "ln";
        default: return NULL;
    }
}

static bool is_operator(char ch) {
    switch (ch) {
        case '+': case '-': case '*': case '/': case '^':
            return true;
        default:
            return false;
    }
}

static bool is_grouping(char ch) {
    return (ch == '(') || (ch == ')');
}

static bool is_constant(const char* str, const int index) {
    char* funcs[] = {"pi", "e"};
    int len = sizeof(funcs)/sizeof(char*);

    for (int i = 0; i < len; i++) {
        if (string_at(funcs[i], &str[index]))
            return true;
    }

    return false;
}

static bool is_func(const char* str, const int index) {
    char* funcs[] = {"sin", "cos", "tan", "log", "ln"};
    int len = sizeof(funcs)/sizeof(char*);

    for (int i = 0; i < len; i++) {
        if (string_at(funcs[i], &str[index]))
            return true;
    }

    return false;
}

static inline void set_token(struct EquToken* dest, enum TokenType type,
                             double value) {
    dest->type = type;

    if (type == TYPE_INTEGER) {
        dest->ivalue = (int64_t) value;
    } else if (type == TYPE_DECIMAL) {
        dest->fvalue = value;
    }
}

/* ===== Tokens List Functions =====*/

struct TokensList init_list(void) {
    struct TokensList list;
    for (int i = 0; i < MAX_TOKENS; i++)
        list.tokens[i].type = TYPE_EMPTY;
    list.capacity = MAX_TOKENS;
    list.tkns_pos = 0;

    return list;
}

static void add_operator(struct TokensList* list, const char ch) {
    if (list->op_pos < (list->capacity - 1)) {
        struct EquToken token;
        enum TokenType type = char_to_type(ch);
        set_token(&token, type, 0.0);
    }
}

static void add_number(struct TokensList* list, const char* equ,
                       int* iterator) {
    bool is_decimal = false;
    const int START_INDEX = *iterator;

    while (isdigit(equ[*iterator]) || equ[*iterator] == '.') {
        if (equ[*iterator] == '.')
            is_decimal = true;
        (*iterator)++;
    }

    if (is_decimal) {
        double value = strtod(&equ[START_INDEX], NULL);
        set_token(&list->tokens[list->tkns_pos], TYPE_DECIMAL, value);
        list->tkns_pos++;
    } else {
        int value = strtoll(&equ[START_INDEX], NULL, 10);
        set_token(&list->tokens[list->tkns_pos], TYPE_INTEGER, value);
        list->tkns_pos++;
    }
}

static void add_function(struct TokensList* list, const char* equ,
                         int* iterator) {

    enum TokenType type = str_to_fn(&equ[*iterator]);
    int len = strlen(fn_type_to_str(type));

    if (type == TYPE_EMPTY) {
        /* string was not a function */
        ;;;
    } else {
        set_token(&list->tokens[list->tkns_pos], type, 0);
        list->tkns_pos++;
        *iterator += len;
    }
}

static void add_constant(struct TokensList* list, const char* equ,
                         int* iterator) {
    if (string_at("pi", &equ[*iterator])) {
        set_token(&list->tokens[list->tkns_pos], CONSTANT_PI, 0);   
        list->tkns_pos++;
        *iterator += 2; 
    } else if (string_at("e", &equ[*iterator])) {
        set_token(&list->tokens[list->tkns_pos], CONSTANT_E, 0);
        list->tkns_pos++;
        *iterator += 1;
    }
}


/* ===== Calculator Functions =====*/

/**
 * @brief tokenizes string `equ` and reads into `tkns`
 * 
 * @param equ 
 * @param tkns 
 * @param tkns_size 
 * @return MC3_ErrorCode 
 */
static MC3_ErrorCode tokenize(const char* equ, struct TokensList* list) {
    const int EQU_LENGTH = strlen(equ);

    for (int i = 0; i < EQU_LENGTH; i++) {
        if (is_operator(equ[i]) || is_grouping(equ[i])) {
            add_operator(list, equ[i]);
        } else if (isdigit(equ[i])) {
            add_number(list, equ, &i);
        } else if (is_func(equ, i)) {
            add_function(list, equ, &i);
        } else if (is_constant(equ, i)) {
            add_constant(list, equ, &i);
        } else if (equ[i] == ' ') {
            continue;
        } else {
            return MC3_INVALID_CHARACTER_FOUND;
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
    return MC3_NO_ERROR;
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

void write_error(MC3_ErrorCode* err_obj, MC3_ErrorCode code) {
    if (err_obj != NULL)
        *err_obj = code;
}

/* ===== Debugging Functions =====*/

/* DEBUGGING */
// TODO: Update functions to use get_type_str() correctly
const char* get_type_str(const struct EquToken tkn) {
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
            printf("TYPE_INTEGER: %lld",  tkn.ivalue);
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
        if (list->tokens[i].type == TYPE_INTEGER) {
            printf("%-2zu: INTEGER(%lld)", i, list->tokens[i].ivalue);
        } else if (list->tokens[i].type == TYPE_DECIMAL) {
            printf("%-2zu: DECIMAL(%lf)", i, list->tokens[i].fvalue);
        } else {
            const char* type = get_type_str(list->tokens[i]);
            printf("%-2zu: %s", i, type);
        }
        puts("");
    }
}

/* DEBUGGING */
void print_compact_tokens(const struct TokensList* list) {
    printf("[ ");
    for (size_t i = 0; i < list->tkns_pos; i++) {
        if (list->tokens[i].type == TYPE_INTEGER) {
            printf("%-2zu: INTEGER(%lld)", i, list->tokens[i].ivalue);
        } else if (list->tokens[i].type == TYPE_DECIMAL) {
            printf("%-2zu: DECIMAL(%lf)", i, list->tokens[i].fvalue);
        } else {
            const char* type = get_type_str(list->tokens[i]);
            printf("%-2zu: %s", i, type);
        }
        printf(", ");
    }
    puts("]");
}

/* DEBUGGING (NOT IMPLEMENTED YET) */
void print_tokens_full(const struct TokensList* list) {
    (void) list;
}

/**
 * @brief evaluates a mathematical expression from a string, returning the
 * result as a double. Calculator supports all basic arithmetic operators:
 * +, -, *, /, and ^. Also supports basic scientific functions such as ln,
 * sin and constants such as e and pi. The calculator follows  standard PEMDAS
 * order of operations. 
 * 
 * @param equ 
 * @param err if parameter is NULL, then the error will not be set. Otherwise,
 * the value of any error that occurs is written to `err`.
 * @return double 
 */
double MC3_evaluate(const char *equ, MC3_ErrorCode* err) {
    (void) equ;
    
    struct TokensList tokens_list = init_list();
    MC3_ErrorCode error_code = MC3_NO_ERROR;

    error_code = tokenize(equ, &tokens_list);

    if (error_code != MC3_NO_ERROR) {
        write_error(err, error_code);
        return 0.0;
    }

    error_code = parse_tokens(&tokens_list);
    if (error_code != MC3_NO_ERROR)
        goto error_occured;

    print_compact_tokens(&tokens_list);
    if (error_code != MC3_NO_ERROR)
        goto error_occured;


    error_occured: {
        write_error(err, error_code);
        return 0.0;
    }

    return -0.0;
}
