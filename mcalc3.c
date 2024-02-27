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
    EMPTY = 0,
    L_PAR,
    R_PAR,
    OPERATOR,
    INTEGER,
    DECIMAL,
    VARIABLE,
} TokenType;


/* enum starts at 10 to avoid collisions */
typedef enum OperatorType {
    OP_ADD = 10,
    OP_SUB,
    OP_MULT,
    OP_DIV,
    OP_EXP,
} OperatorType;


typedef struct MC3_EquToken {
    TokenType type;

    union {
        OperatorType optype;
        int64_t ivalue;
        float64_t fvalue;
        char identifier;
    };
} MC3_EquToken;


typedef struct MC3_Solution {
    char identifier;
    double value;
} MC3_Solution;


int checkTokensOverflow(int* index, const int capacity) {
    if ( (*index) >= capacity ) {
        return 0;
    } else {
        return 1;
    }
}


static inline void tokensAdd(MC3_EquToken* reference, MC3_EquToken tokens[], const int index) {
    tokens[index] = *reference;
}


/**
 * @brief tokenizes string `equ` into MC3_EquToken `tokens` 
 * 
 * @param equ 
 * @return MC3_ErrorCode 
 */
static MC3_ErrorCode tokenizeEqu(const char* equ, MC3_EquToken tokens[], const size_t len) {
    uint8_t tokensIndex = 0; 
    MC3_EquToken tempToken;

    for (size_t i = 0; equ[i] != '\0'; ++i) {
        switch (equ[i]) {
            case '+':
                ; 
                break;
            case '-': ; 
                break;
            case '*': ; 
                break;
            case '/': ; 
                break;
            case '^': ; 
                break;
            case '(': ; 
                break;
            case ')': ; 
                break;
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                break;
            case ' ': break;
            default:
                break;
        }
    }

    return MC3_Error_NO_ERROR;
}


static void initTokens(MC3_EquToken tokens[], const size_t size) {
    for (size_t i = 0; i < size; ++i) {
        tokens[i].type = EMPTY;
    }
}


/** Debugging Functions */
void printTokens(const MC3_EquToken[], const size_t);


/**
 * @brief evaluates expression within `equ`, returns result
 * 
 * @param equ 
 * @return double 
 */
extern double MC3_evaluateEqu(const char* equ) {
    /* Initialization */
    MC3_EquToken tokens[MAX_TOKENS];
    
    initTokens(tokens, MAX_TOKENS);

    /* Tokenization */
    tokenizeEqu(equ, tokens, MAX_TOKENS);

    /* Parsing */

    /* Clean-Up */

    /* Debugging */

    return -0.0;
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
            case EMPTY: break;
        }
    }
}
