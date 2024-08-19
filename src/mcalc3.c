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


#define MAX_TOKENS 50


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


typedef struct EquToken {
    
    enum TokenType type;
    
    union {
        int64_t ivalue;
        float64_t fvalue;
    }; 

} EquToken;


/* ===== Utility Functions =====*/

static inline void clearString(char* str, const size_t len) {
    memset(str, '\0', len);
}


/**
 * @brief checks to see if substring is in string at str_start 
 * 
 * @return true 
 * @return false 
 */
static inline bool stringAt(const char sub_str[], const char* str_start) {
    return (strstr(str_start, sub_str) == str_start);
}


/* ===== Calculator Functions =====*/

static void initTokens(EquToken tokens[], const size_t size) {
    for (size_t i = 0; i < size; i++) {
        tokens[i].type = TYPE_EMPTY;
    }
}


static inline void setToken(EquToken* tkn, enum TokenType type, double value) {
    tkn->type = type;
    if (type == TYPE_INTEGER) {
        tkn->ivalue = (int64_t) value;
    }
    else if (type == TYPE_DECIMAL) {
        tkn->fvalue = value;
    }
}


static MC3_ErrorCode addToken(const EquToken* ref, EquToken tkns[], size_t* pos, const size_t max) {
    if ((*pos) >= max) {
        return TOKENS_LIMIT_REACHED;
    }
    
    double value;
    if (ref->type == TYPE_INTEGER) {
        value = ref->ivalue;
    }
    else if (ref->type == TYPE_DECIMAL) {
        value = ref->fvalue;
    }

    setToken(&tkns[*pos], ref->type, value);

    (*pos)++;

    return NO_ERROR;
}


static MC3_ErrorCode read_number(const char* equ, EquToken* tkn, size_t* index) {
    const size_t j = (*index);
    bool is_decimal = false;
    while (isdigit(equ[*index]) || equ[*index] == '.') {
        if (equ[*index] == '.')
            is_decimal = true;
        
        (*index)++;
    }

    if (is_decimal) {
        setToken(tkn, TYPE_DECIMAL, strtod(&equ[j], NULL));
    } 
    else {
        setToken(tkn, TYPE_INTEGER, strtol(&equ[j], NULL, 10));
    }

    /* prevents position pointer from skipping over character after number */
    (*index)--;

    return NO_ERROR;
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
static MC3_ErrorCode tokenize(const char* equ, EquToken tkns[], const size_t tkns_size) {
    EquToken ref_token;
    size_t pos = 0;
    const size_t EQU_LENGTH = strlen(equ);

    for (size_t i = 0; i < EQU_LENGTH; i++) {
        /* checking for numbers, operators, and grouping */
        if (is_operator(equ[i])) {
            enum TokenType type = char_to_type(equ[i]);
            setToken(&ref_token, type, 0);
        } else if (is_grouping(equ[i])) {
            if (equ[i] == '(')
                setToken(&ref_token, PAR_LEFT, 0);
            else
                setToken(&ref_token, PAR_RIGHT, 0);
        } else if (isdigit(equ[i])) {
            read_number(equ, &ref_token, &i);
        }
        /* checking for functions */
        else if (stringAt("sin", &equ[i])) {
            setToken(&ref_token, FN_SIN, 0);
        }
        else if (stringAt("cos", &equ[i])) {
            setToken(&ref_token, FN_COS, 0);
        } 
        else if (stringAt("tan", &equ[i])) {
            setToken(&ref_token, FN_TAN, 0);
        } 
        else if (stringAt("log", &equ[i])) {
            setToken(&ref_token, FN_LOG_10, 0);
        } 
        else if (stringAt("ln", &equ[i])) {
            setToken(&ref_token, FN_LOG_E, 0);
        }
        
        /* checking for constants */
        else if (stringAt("pi", &equ[i])) {
            setToken(&ref_token, CONSTANT_PI, 0);
        }
        else if (stringAt("e", &equ[i])) {
            setToken(&ref_token, CONSTANT_E, 0);
        }
        else {
            ref_token.type = TYPE_EMPTY;
        }

        /* adding token to tokens array */
        if (ref_token.type != TYPE_EMPTY) {
            addToken(&ref_token, tkns, &pos, tkns_size);
        }
    }

    return NO_ERROR;
}


/**
 * @brief creates parse tree using tokens
 * 
 * @param tkns 
 * @param size 
 * @return MC3_ErrorCode 
 */
static MC3_ErrorCode parse_tokens(EquToken tkns[], const size_t size);


void DEBUGGING_printToken(const EquToken tkn);
void DEBUGGING_printTokens(const EquToken tkns[], const size_t size);
void DEBUGGING_printCompactTokens(const EquToken tkns[]);


double MC3_evaluate(const char *equ, MC3_ErrorCode* err) {
    /* declare requisite variables */
    MC3_ErrorCode errorCode = NO_ERROR;
    (void) errorCode;
    (void) err;
    EquToken tokens[MAX_TOKENS];
    initTokens(tokens, MAX_TOKENS);
    
    /* lex and parse expression */
    errorCode = tokenize(equ, tokens, MAX_TOKENS);

    /* debugging */
    DEBUGGING_printCompactTokens(tokens);

    return 0.0;
}


void DEBUGGING_printToken(const EquToken tkn) {
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


void DEBUGGING_printTokens(const EquToken tkns[], const size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%-2zu: ", i);
        DEBUGGING_printToken(tkns[i]);
        puts("");
    }
}


void DEBUGGING_printCompactTokens(const EquToken tkns[]) {
    printf("[ ");
    for (int i = 0; tkns[i].type != TYPE_EMPTY; i++) {
        DEBUGGING_printToken(tkns[i]);
        printf(", ");
    }
    puts("]");
}
