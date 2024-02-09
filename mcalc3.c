#include "mcalc3.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
typedef float float32_t;
typedef double float64_t;


#define MAX_TOKENS 25
#define ORDER_OF_OP_ROWS 6
#define ORDER_OF_OP_COLUMNS 8

typedef uint8_t OrderOfOpsTracker[ORDER_OF_OP_ROWS][ORDER_OF_OP_COLUMNS];


typedef enum TokenType {
    EMPTY,
    OP_ADD,
    OP_SUB,
    OP_MULT,
    OP_DIV,
    OP_EXP,
    L_PAR,
    R_PAR,
    INTEGER,
    DECIMAL,
    VARIABLE,
} TokenType;


typedef struct MC3_EquToken {
    TokenType type;

    union {
        int64_t ivalue;
        float64_t fvalue;
        char identifier;
    };
} MC3_EquToken;


static MC3_ErrorCode tokenizeEqu(const char* equ) {
    for (size_t i = 0; equ[i] != '\0'; ++i) {
        switch (equ[i]) {
            case '+': break;
            case '-': break;
            case '*': break;
            case '/': break;
            case '^': break;
            case '(': break;
            case ')': break;
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                break;
            case ' ': break;
            default:
                break;
        }
    }

    return MC3_NO_ERROR;
}


static void initTokens(MC3_EquToken tokens[], const size_t size) {
    for (size_t i = 0; i < size; ++i) {
        tokens[i].type = EMPTY;
    }
}


void printTokens(const MC3_EquToken tokens[], const size_t size) {
    for (size_t i = 0; i < size; ++i) {
        switch (tokens[i].type) {
            case OP_ADD: break;
            case OP_SUB: break;
            case OP_MULT: break;
            case OP_DIV: break;
            case OP_EXP: break;
            case L_PAR: break;
            case R_PAR: break;
            case INTEGER: break;
            case DECIMAL: break;
            case VARIABLE: break;
        }
    }
}

extern double MC3_evaluateEqu(const char* equ) {
    /* Initialization */
    MC3_EquToken tokens[MAX_TOKENS];

    /* Tokenization */
    tokenizeEqu(equ);

    /* Parsing */

    /* Clean-Up */

    return -0.0;
} 
