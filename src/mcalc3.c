#include "mcalc3.h"
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>


typedef float float32_t;
typedef double float64_t;


#define MAX_TOKENS 50


typedef enum TokenType {

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

} TokenType;


typedef struct EquToken {
    
    TokenType type;
    
    union {
        int64_t ivalue;
        float64_t fvalue;
    }; 

} EquToken;


/* ===== Utility Functions =====*/

static inline void clearString(char* str, const size_t len) {
    memset(str, '\0', len);
}


/* ===== Calculator Functions =====*/

static void initTokens(EquToken tokens[], const size_t size) {
    for (size_t i = 0; i < size; i++) {
        tokens[i].type = TYPE_EMPTY;
    }
}


static inline void setToken(EquToken* tkn, TokenType type, double value) {
    tkn->type = type;
    if (type == TYPE_INTEGER) {
        tkn->ivalue = (int64_t) value;
    }
    else if (type == TYPE_DECIMAL) {
        tkn->fvalue = value;
    }
}


static MC3_ErrorCode addToken(const EquToken* ref, EquToken tkns[], size_t* pos,
                                const size_t max)
{
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


static MC3_ErrorCode tokenize(const char* equ, EquToken tkns[], const size_t max) {
    EquToken ref_token;
    size_t pos = 0;
    const size_t EQU_LENGTH = strlen(equ);
    const unsigned short STR_NUM_SIZE = 15;
    char str_num[STR_NUM_SIZE];
    (void) str_num;

    for (size_t i = 0; i < EQU_LENGTH; i++) {
        switch (equ[i]) {
            case '+':
                setToken(&ref_token, OP_ADD, 0);
                break;
            case '-':
                setToken(&ref_token, OP_SUB, 0);
                break;
            case '*':
                setToken(&ref_token, OP_MULT, 0);
                break;
            case '/':
                setToken(&ref_token, OP_DIV, 0);
                break;
            case '^':
                setToken(&ref_token, OP_EXP, 0);
                break;
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9': {
                const int j = (int) i;
                bool is_decimal = false;
                while (isdigit(equ[i]) || equ[i] == '.') {
                    if (equ[i] == '.')
                        is_decimal = true;
                    
                    i++;
                }

                if (is_decimal) {
                    setToken(&ref_token, TYPE_DECIMAL, strtod(&equ[j], NULL));
                } 
                else {
                    setToken(&ref_token, TYPE_INTEGER, strtol(&equ[j], NULL, 10));
                }

                break; 
            }
            default: 
                ref_token.type = TYPE_EMPTY;
                break;
        }

        if (ref_token.type != TYPE_EMPTY) {
            addToken(&ref_token, tkns, &pos, max);
        }
    }

    return NO_ERROR;
}


void DEBUGGING_printToken(const EquToken tkn);
void DEBUGGING_printTokens(const EquToken tkns[], const size_t size);


double MC3_evaluate(const char *equ) {
    /* declare requisite variables */
    MC3_ErrorCode errorCode = NO_ERROR;
    (void) errorCode;
    EquToken tokens[MAX_TOKENS];
    initTokens(tokens, MAX_TOKENS);
    
    /* lex and parse expression */
    errorCode = tokenize(equ, tokens, MAX_TOKENS);

    /* debugging */
    DEBUGGING_printTokens(tokens, MAX_TOKENS);

    return 0.0;
}


void DEBUGGING_printToken(const EquToken tkn) {
    switch (tkn.type) {
    case OP_ADD:
        puts("OP_ADD");
        break;
    case OP_SUB:
        puts("OP_SUB");
        break;
    case OP_MULT:
        puts("OP_MULT");
        break;
    case OP_DIV:
        puts("OP_DIV");
        break;
    case OP_EXP:
        puts("OP_EXP");
        break;
    case PAR_LEFT:
        puts("PAR_LEFT");
        break;
    case PAR_RIGHT:
        puts("PAR_RIGHT");
        break;
    case TYPE_INTEGER:
        printf("TYPE_INTEGER: %ld\n", tkn.ivalue);
        break;
    case TYPE_DECIMAL:
        printf("TYPE_DECIMAL: %lf\n", tkn.fvalue);
        break;
    case TYPE_EMPTY:
        puts("TYPE_EMPTY");
        break;
    case FN_SIN:
        puts("FN_SIN");
        break;
    case FN_COS:
        puts("FN_COS");
        break;
    case FN_TAN:
        puts("FN_TAN");
        break;
    case FN_LOG_10:
        puts("FN_LOG_10");
        break;
    case FN_LOG_E:
        puts("FN_LOG_E");
        break;
    }
}


void DEBUGGING_printTokens(const EquToken tkns[], const size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%-2zu: ", i);
        DEBUGGING_printToken(tkns[i]);
    }
}
