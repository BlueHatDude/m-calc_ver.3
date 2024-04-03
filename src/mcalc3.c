#include "mcalc3.h"
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
typedef float float32_t;
typedef double float64_t;


#define MAX_TOKENS 25
#define ORDER_OF_OP_ROWS 6
#define ORDER_OF_OP_COLUMNS 8

typedef uint8_t OrderOfOpsTracker[ORDER_OF_OP_ROWS][ORDER_OF_OP_COLUMNS];


typedef enum TokenType {
    EMPTY,
    L_PAR,
    R_PAR,
    TYPE_INTEGER,
    TYPE_DECIMAL,
    TYPE_VARIABLE,
    OP_ADD,
    OP_SUB,
    OP_MULT,
    OP_DIV,
    OP_EXP,
} TokenType;


typedef struct MC3_EquToken {
    
    TokenType type;

    union {
        int64_t ivalue;
        float64_t fvalue;
        char identifier;
    };

} MC3_EquToken;


typedef struct MC3_Solution {
    char identifier;
    double value;
} MC3_Solution;


static inline void clearBuffer(char* s) {
    s[0] = '\0';
}


int checkTokensOverflow(int* index, const int capacity) {
    if ( (*index) >= capacity ) {
        return 0;
    } else {
        return 1;
    }
}


static inline void addToken(MC3_EquToken* reference, MC3_EquToken tokens[], const int index) {
    tokens[index] = *reference;
}


/**
 * @brief sets the type of MC3_EquToken object
 * 
 * @param token which token's properties to set 
 * @param type is of type double, but can accept int or char
 */
static inline void setType(MC3_EquToken* token, const TokenType type, const double value) {
    token->type = type;

    /* ivalue, fvalue, identifier */
    switch (token->type) {
        case TYPE_INTEGER:
            token->ivalue = (int) value;
            break;
        case TYPE_DECIMAL:
            token->fvalue = value;
            break;
        case TYPE_VARIABLE:
            token->identifier = (char) value;
            break;
        default:
            break;
    }
}


/**
 * @brief tokenizes string `equ` into MC3_EquToken `tokens` 
 * 
 * @param equ 
 * @return MC3_ErrorCode 
 */
static MC3_ErrorCode tokenizeEqu(const char* equ, MC3_EquToken tokens[], const size_t len) {
    (void) len;

    uint8_t tokensIndex = 0; 
    MC3_EquToken tempToken;
    const size_t bufferSize = 12;
    char buffer[bufferSize + 1];


    for (size_t i = 0; equ[i] != '\0'; ++i) {
        switch (equ[i]) {
            case '+':
                setType(&tempToken, OP_ADD, -0.0);
                addToken(&tempToken, tokens, tokensIndex); 
                break;
            case '-':
                setType(&tempToken, OP_SUB, -0.0); 
                addToken(&tempToken, tokens, tokensIndex); 
                break;
            case '*': 
                setType(&tempToken, OP_MULT, -0.0); 
                addToken(&tempToken, tokens, tokensIndex); 
                break;
            case '/':
                setType(&tempToken, OP_DIV, -0.0); 
                addToken(&tempToken, tokens, tokensIndex); 
                break;
            case '^':
                setType(&tempToken, OP_EXP, -0.0); 
                addToken(&tempToken, tokens, tokensIndex); 
                break;
            case '(': 
                setType(&tempToken, L_PAR, -0.0); 
                addToken(&tempToken, tokens, tokensIndex); 
                break;
            case ')': ; 
                setType(&tempToken, R_PAR, -0.0); 
                addToken(&tempToken, tokens, tokensIndex); 
                break;
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9': {
                while (isnumber(equ[i])) {
                    int j = 0;
                    buffer[j] = equ[i];
                    j++;
                    i++;
                }
                
                clearBuffer(buffer);
                break;
            }
            case ' ': break;
            default: /* sqrt, sin, cos, tan, sqrt */
                
                break;
        }
        tokensIndex++;
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
            case TYPE_INTEGER: break;
            case TYPE_DECIMAL: break;
            case TYPE_VARIABLE: break;
            case EMPTY: break;
        }
    }
}
