#include "mcalc3.h"
#include "mlogging.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>


enum MC3_EquTknIdentifier {
    EMPTY,
    OPERATOR,
    L_PAR,
    R_PAR,
    INTEGER,
    DECIMAL,
    VARIABLE // not yet implemented
};


enum MC3_EquTknSubType {
    OP_PLUS,
    OP_MINUS,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_EXPONENT,
};


typedef struct MC3_EquToken {
    enum MC3_EquTknIdentifier type;
    
    union {
        int64_t ivalue;
        float64_t fvalue;
        char varname;

        enum MC3_EquTknSubType subtype;
    };
} MC3_EquToken;


enum OpsNParsArrType {
    AT__INDEX_TRACKERS    = 0,
    AT__LEFT_PARENTHESIS  = 1,
    AT__RIGHT_PARENTHESIS = 2,
    AT__EXPONENT          = 3,
    AT__MULT_AND_DIV      = 4,
    AT__ADD_AND_SUB       = 5,
};


enum OpsNParsPtrType{
    PT__PARENTHESIS = 0,
    PT__EXPONENTS   = 1,
    PT__MULT_N_DIVS = 2,
    PT__ADD_N_SUB   = 3,
};


/*
    Pointer Types:
        0: Parenthesis
        1: Exponents
        2: Multiplication & Division
        3: Addition & Subtraction

                                                             0  1  2  3  4
    Index Trackers:            [ 0, 0, 0, 0, 0, 0, 0, 0 ]
    Left Parenthesis:          [ 0, 0, 0, 0, 0, 0, 0, 0 ]
    Right Parenthesis:         [ 0, 0, 0, 0, 0, 0, 0, 0 ]
    Exponent:                  [ 0, 0, 0, 0, 0, 0, 0, 0 ]
    Multiplication & Division: [ 0, 0, 0, 0, 0, 0, 0, 0 ]
    Addition & Subtraction:    [ 0, 0, 0, 0, 0, 0, 0, 0 ]
*/


static inline MC3_ErrorCode appendEquTkn(MC3_EquToken tokens[], MC3_EquToken token, size_t maxSize) {
    unsigned i = 0;
    while( tokens[i].type != EMPTY ) {
        if(i != maxSize) {
            i++;
        } else {
            return NOT_ENOUGH_SPACE;
        }
    }
    tokens[i] = token;
    return NO_ERROR;
}


MC3_ErrorCode readEqu(const char equ[], MC3_EquToken tokens[], const size_t len) {
    const size_t strLen = strlen(equ);
    MC3_EquToken temp_token = { .type = EMPTY };
    char str_num[15] = "              "; // 14 width empty string
    MC3_ErrorCode error_code = NO_ERROR;
    
    for (unsigned i = 0; i < strLen; ++i) {
        switch(equ[i]) {
            case '+':
                temp_token.type = OPERATOR;
                temp_token.subtype = OP_PLUS;
                error_code = appendEquTkn(tokens, temp_token, len);
                break;
            case '-':
                temp_token.type = OPERATOR;
                temp_token.subtype = OP_MINUS;
                error_code = appendEquTkn(tokens, temp_token, len);
                break;
            case '*':
                temp_token.type = OPERATOR;
                temp_token.subtype = OP_MULTIPLY;
                error_code = appendEquTkn(tokens, temp_token, len);
                break;
            case '/':
                temp_token.type = OPERATOR;
                temp_token.subtype = OP_DIVIDE;
                error_code = appendEquTkn(tokens, temp_token, len);
                break;
            case '^':
                temp_token.type = OPERATOR;
                temp_token.subtype = OP_EXPONENT;
                error_code = appendEquTkn(tokens, temp_token, len);
                break;
            case '(':
                temp_token.type = L_PAR;
                error_code = appendEquTkn(tokens, temp_token, len);
                break;
            case ')':
                temp_token.type = R_PAR;
                error_code = appendEquTkn(tokens, temp_token, len);
                break;
            case ' ':
                break;
            default: {
                unsigned j = 0;
                bool_t isDecimal = FALSE;

                while( charInStr(equ[i], "0123456789.") ) {
                    // printf("DEBUGGING~ %d | %c\n", equ[i], equ[i]);
                    
                    if (equ[i] == '.') 
                        isDecimal = TRUE;
                    
                    str_num[j] = equ[i];
                    i++;
                    j++;
                }

                if(isDecimal) {
                    temp_token.type = DECIMAL;
                    temp_token.fvalue = atof(str_num);
                } else {
                    temp_token.type = INTEGER;
                    temp_token.ivalue = atoi(str_num);
                }
                appendEquTkn(tokens, temp_token, len);
                strcpy(str_num, "              "); // resets str_num to empty string

                i--;

                break;
            }
        }
    }

    return error_code;
}


MC3_ErrorCode readOpsAndPars(MC3_EquToken tokens[], uint8_t ops_and_pars[6][8]) {
    size_t tokensEnd = 0;
    while (tokens[tokensEnd].type != EMPTY)
        tokensEnd++;
    
    for (int8_t i = tokensEnd; i != -1; --i) {
        if (tokens[i].type == OPERATOR) {
            switch(tokens[i].subtype) {
                case OP_PLUS:
                    ops_and_pars[AT__ADD_AND_SUB][ ops_and_pars[AT__INDEX_TRACKERS][AT__ADD_AND_SUB] ] = i;
                    ops_and_pars[AT__INDEX_TRACKERS][AT__ADD_AND_SUB]++;
                    break;
                case OP_MINUS:
                    ops_and_pars[AT__ADD_AND_SUB][ ops_and_pars[AT__INDEX_TRACKERS][AT__ADD_AND_SUB] ] = i;
                    ops_and_pars[AT__INDEX_TRACKERS][AT__ADD_AND_SUB]++;
                    break;
                case OP_MULTIPLY:
                    ops_and_pars[AT__MULT_AND_DIV][ ops_and_pars[AT__INDEX_TRACKERS][PT__MULT_N_DIVS] ] = i;
                    ops_and_pars[AT__INDEX_TRACKERS][PT__MULT_N_DIVS]++;
                    break;
                case OP_DIVIDE:
                    ops_and_pars[AT__MULT_AND_DIV][ ops_and_pars[AT__INDEX_TRACKERS][PT__MULT_N_DIVS] ] = i;
                    ops_and_pars[AT__INDEX_TRACKERS][PT__MULT_N_DIVS]++;
                    break;
                case OP_EXPONENT:
                    ops_and_pars[AT__EXPONENT][ ops_and_pars[AT__INDEX_TRACKERS][PT__EXPONENTS] ] = i;
                    ops_and_pars[AT__INDEX_TRACKERS][PT__EXPONENTS]++;
                    break;
            }
        } else if (tokens[i].type == L_PAR) {
            size_t temp_index = i;
            while(tokens[temp_index].type != R_PAR) temp_index++;

            ops_and_pars[AT__LEFT_PARENTHESIS][ ops_and_pars[AT__INDEX_TRACKERS][PT__PARENTHESIS] ] = i;
            ops_and_pars[AT__RIGHT_PARENTHESIS][ ops_and_pars[AT__INDEX_TRACKERS][PT__PARENTHESIS] ] = temp_index;
            ops_and_pars[AT__INDEX_TRACKERS][PT__PARENTHESIS]++;
        } 
    }

    return NO_ERROR;
}


enum NumTypes {
    INT_THEN_INT,
    DECIMAL_THEN_DECIMAL,
    INT_THEN_DECIMAL,
    DECIMAL_THEN_INT,
}; 


void evalOp(MC3_EquToken tokens[], const size_t index, const size_t before, const size_t after) {
    
    switch (tokens[index].subtype) {
        case OP_PLUS: {
            if ( (tokens[before].type == INTEGER) && (tokens[after].type == DECIMAL) ) {
                tokens[index].type = INTEGER;
                tokens[index].ivalue = tokens[before].ivalue + tokens[after].ivalue;
            } else {
                tokens[index].type = DECIMAL;
                tokens[index].fvalue = ( (tokens[before].type == INTEGER) ? tokens[before].ivalue : tokens[before].fvalue )\
                + ( (tokens[after].type == INTEGER) ? tokens[after].ivalue : tokens[after].fvalue );
            }
            break;
        }
        case OP_MINUS: {
            if ( (tokens[before].type == INTEGER) && (tokens[after].type == DECIMAL) ) {
                tokens[index].type = INTEGER;
                tokens[index].ivalue = tokens[before].ivalue - tokens[after].ivalue;
            } else {
                tokens[index].type = DECIMAL;
                tokens[index].fvalue = ( (tokens[before].type == INTEGER) ? tokens[before].ivalue : tokens[before].fvalue )\
                - ( (tokens[after].type == INTEGER) ? tokens[after].ivalue : tokens[after].fvalue );
            }
            break;
        }
        case OP_MULTIPLY: {      
            if ( (tokens[before].type == INTEGER) && (tokens[after].type == DECIMAL) ) {
                tokens[index].type = INTEGER;
                tokens[index].ivalue = tokens[before].ivalue * tokens[after].ivalue;
            } else {
                tokens[index].type = DECIMAL;
                tokens[index].fvalue = ( (tokens[before].type == INTEGER) ? tokens[before].ivalue : tokens[before].fvalue )\
                * ( (tokens[after].type == INTEGER) ? tokens[after].ivalue : tokens[after].fvalue );
            }
            break;
        }
        case OP_DIVIDE: {
            tokens[index].type = DECIMAL;
            tokens[index].fvalue = ( (tokens[before].type == INTEGER) ? tokens[before].ivalue : tokens[before].fvalue )\
            + ( (tokens[after].type == INTEGER) ? tokens[after].ivalue : tokens[after].fvalue );
            break;
        }
        case OP_EXPONENT: {
            if ( (tokens[before].type == INTEGER) && (tokens[after].type == DECIMAL) ) {
                tokens[index].type = INTEGER;
                tokens[index].ivalue = tokens[before].ivalue + tokens[after].ivalue;
            } else {
                tokens[index].fvalue = ( (tokens[before].type == INTEGER) ? tokens[before].ivalue : tokens[before].fvalue )\
                + ( (tokens[after].type == INTEGER) ? tokens[after].ivalue : tokens[after].fvalue );
            }
            break;
        }
    }

    tokens[before].type = EMPTY;
    tokens[after].type = EMPTY;
}


MC3_ErrorCode subEvaluateOp(MC3_EquToken tokens[], const size_t index) {
    size_t before = index;
    size_t after = index;

    while (tokens[before].type != INTEGER && tokens[before].type != DECIMAL)
        before--;
    while (tokens[after].type != INTEGER && tokens[after].type != DECIMAL)
        after++;

    evalOp(tokens, index, before, after);

    return NO_ERROR;
}


void evaluateOps(uint8_t opsAndPars[6][8], MC3_EquToken tokens[]) {
    uint8_t writtenUpTo = 0;
    (void) tokens;
    (void) opsAndPars;

    return;
    subEvaluateOp(tokens, 0x00);
}


void __logTokens(MC3_EquToken tokens[], size_t size) {
    for(unsigned i = 0; i < size; ++i) {
        switch(tokens[i].type) {
            case EMPTY:
                printf("Index: %2d | Type: EMPTY \n", i);
                break;
            case INTEGER:
                printf("Index: %2d | Type: INTEGER | ivalue: %lld \n", i, (long long) tokens[i].ivalue);
                break;
            case DECIMAL:
                printf("Index: %2d | Type: DECIMAL | fvalue: %lf \n", i, tokens[i].fvalue);
                break;
            case L_PAR:
                printf("Index: %2d | Type: L_PAR \n", i);
                break;
            case R_PAR:
                printf("Index: %2d | Type: R_PAR \n", i);
                break;
            case VARIABLE:
                printf("Index: %2d | Type: VARIABLE | varname: %c \n", i, tokens[i].varname);
                break;
            case OPERATOR:
                switch(tokens[i].subtype) {
                    case OP_PLUS:
                        printf("Index: %2d | Type: OP_PLUS \n", i);
                        break;
                    case OP_MINUS:
                        printf("Index: %2d | Type: OP_MINUS \n", i);
                        break;
                    case OP_MULTIPLY:
                        printf("Index: %2d | Type: OP_MULTIPLY \n", i);
                        break;
                    case OP_DIVIDE:
                        printf("Index: %2d | Type: OP_DIVIDE \n", i);
                        break;
                    case OP_EXPONENT:
                        printf("Index: %2d | Type: OP_EXPONENT \n", i);
                        break;
                }
                break;
            default:
                printf("Index: %2d | Type: UNIDENTIFIED TOKEN \n", i);
                break;
        }    
    } 
}


void __logOpsAndPars(uint8_t ops_and_pars[6][8]) {
    for (unsigned i = 0; i < 6; ++i) {
        printf("%d: [", i);
        for (unsigned j = 0; j < 8; ++j) {
            printf("%d%s", ops_and_pars[i][j], (j == 7) ? "" : ", ");
        }
        puts("]");
    }
}


void checkErrors(MC3_ErrorCode code) {
    (void) code;
} 


double MC3_evaluate(const char *equation) {
    MC3_EquToken tokens[25];
    const size_t tokensSize = sizeof(tokens)/sizeof(tokens[0]);
    clearTokens(tokens, tokensSize);
    uint8_t ops_n_pars[6][8] = {
        { 0, 0, 0, 0, 0, 0, 0, 0 },    
        { 0, 0, 0, 0, 0, 0, 0, 0 },    
        { 0, 0, 0, 0, 0, 0, 0, 0 },    
        { 0, 0, 0, 0, 0, 0, 0, 0 },    
        { 0, 0, 0, 0, 0, 0, 0, 0 },    
        { 0, 0, 0, 0, 0, 0, 0, 0 },    
    };
    MC3_ErrorCode error_code = NO_ERROR;
     
    error_code = readEqu(equation, tokens, tokensSize);
    checkErrors(error_code);
    error_code = readOpsAndPars(tokens, ops_n_pars);
    checkErrors(error_code);
 
    return -0.0;
} 


/**
 * @brief sets all tokens to type 'EMPTY'
 * 
 * @param tokens 
 * @param size 
 */
static inline void MC3_initTokens(MC3_EquToken tokens[], const size_t size) {
    for (size_t i = 0; i < size; ++i) {
        tokens[i].type = EMPTY;
    }
}


static inline void MC3_initOpsNPars(uint8_t nums[6][8], const size_t rows, const size_t columns) {
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < columns; ++j) {
            nums[i][j] = 0;
        }
    }
}


void DEBUGGING_printOpsNPars(const uint8_t nums[6][8]) {
    puts("Operators & Parenthesis");
    
    /*
        uint8_t ops_n_pars[6][8] = {
            {0, 0, 0, 0, 0, 0, 0, 0}, // left parenthesis
            {0, 0, 0, 0, 0, 0, 0, 0}, // right parenthesis
            {0, 0, 0, 0, 0, 0, 0, 0}, // exponents
            {0, 0, 0, 0, 0, 0, 0, 0}, // multiplication and division
            {0, 0, 0, 0, 0, 0, 0, 0}, // addition and subtraction
        };
    */
    
    for (size_t i = 0; i < 6; ++i) {
        printf("%zu: [", i);
        for (size_t j = 0; j < 8; ++j) {
            printf("%d%s \n", nums[i][j], (j == (8 - 1)) ? " " : ", ");
        }
        printf("]\n");
    }
}


void MC3_RUN(void) {
    /* Intialization */
    MC3_EquToken tokens[25];
    const size_t tokensSize = sizeof(tokens)/sizeof(tokens[0]);
    MC3_initTokens(tokens, tokensSize);
    uint8_t ops_and_pars[6][8];
    MC3_initOpsNPars(ops_and_pars, 6, 8);

    /* Operations */
    readEqu("(2 + 4) * (2 ^ 2 / 8)", tokens, tokensSize);
    readOpsAndPars(tokens, ops_and_pars);
    evaluateOps(ops_and_pars, tokens); /* segumentation fault occurs here */
}


void MC3_TESTS(void) {
    MC3_EquToken tokens[25];
    uint8_t ops_n_pars[6][8] = {
        { 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0 },
    };
    const size_t tokensLen = sizeof(tokens) / sizeof(tokens[0]);
    clearTokens(tokens, tokensLen);

    /* readEqu */
    // Single Digits | Single Addition Operator
        readEqu("2 + 4", tokens, tokensLen);
        MLOG_output_test("2 + 4 properly tokenized", TRUE,
            (tokens[0].type == INTEGER && tokens[0].ivalue == 2)
            && (tokens[1].type == OPERATOR && tokens[1].subtype == OP_PLUS)
            && (tokens[2].type == INTEGER && tokens[2].ivalue == 4)
        );
        clearTokens(tokens, tokensLen);
    // Single Digits | Single Subtraction Operator
        readEqu("2 - 4", tokens, tokensLen);
        MLOG_output_test("2 - 4 properly tokenized", TRUE,
            (tokens[0].type == INTEGER && tokens[0].ivalue == 2)
            && (tokens[1].type == OPERATOR && tokens[1].subtype == OP_MINUS)
            && (tokens[2].type == INTEGER && tokens[2].ivalue == 4)
        );
        clearTokens(tokens, tokensLen);
    // Single Digits | Single Multiplication Operator
        readEqu("2 * 4", tokens, tokensLen);
        MLOG_output_test("2 * 4 properly tokenized", TRUE,
            (tokens[0].type == INTEGER && tokens[0].ivalue == 2)
            && (tokens[1].type == OPERATOR && tokens[1].subtype == OP_MULTIPLY)
            && (tokens[2].type == INTEGER && tokens[2].ivalue == 4)
        );
        clearTokens(tokens, tokensLen);
    // Single Digits | Single Division Operator
        readEqu("2 + 4", tokens, tokensLen);
        MLOG_output_test("2 + 4 properly tokenized", TRUE,
            (tokens[0].type == INTEGER && tokens[0].ivalue == 2)
            && (tokens[1].type == OPERATOR && tokens[1].subtype == OP_PLUS)
            && (tokens[2].type == INTEGER && tokens[2].ivalue == 4)
        );
        clearTokens(tokens, tokensLen);
    // Single Digits | Single Exponent Operator
        readEqu("2 + 4", tokens, tokensLen);
        MLOG_output_test("2 + 4 properly tokenized", TRUE,
            (tokens[0].type == INTEGER && tokens[0].ivalue == 2)
            && (tokens[1].type == OPERATOR && tokens[1].subtype == OP_PLUS)
            && (tokens[2].type == INTEGER && tokens[2].ivalue == 4)
        );
        clearTokens(tokens, tokensLen);

    clearTokens(tokens, tokensLen);

    /* readOpsAndPars */
    // Single Addition Operator
        readEqu("2 + 4", tokens, tokensLen);
        readOpsAndPars(tokens, ops_n_pars);
        MLOG_output_test("readsOpsNPars for 2 + 4", TRUE,\
                                        (ops_n_pars[AT__ADD_AND_SUB][0] == 1));
        clearTokens(tokens, tokensLen);
    
    // Single Subtraction Operator
        readEqu("2 - 4", tokens, tokensLen);
        readOpsAndPars(tokens, ops_n_pars);
        MLOG_output_test("readsOpsNPars for 2 - 4", TRUE,\
                                        (ops_n_pars[AT__ADD_AND_SUB][0] == 1));
        clearTokens(tokens, tokensLen);

    // Single Multiplication Operator
        readEqu("2 * 4", tokens, tokensLen);
        readOpsAndPars(tokens, ops_n_pars);
        MLOG_output_test("readsOpsNPars for 2 * 4", TRUE,\
                                        (ops_n_pars[AT__MULT_AND_DIV][0] == 1));
        clearTokens(tokens, tokensLen);
    
    // Single Division Operator
        readEqu("2 * 4", tokens, tokensLen);
        readOpsAndPars(tokens, ops_n_pars);
        MLOG_output_test("readsOpsNPars for 2 * 4", TRUE,\
                                        (ops_n_pars[AT__MULT_AND_DIV][0] == 1));
        clearTokens(tokens, tokensLen);
    
    // Single Exponent Operator
        readEqu("2 ^ 4", tokens, tokensLen);
        readOpsAndPars(tokens, ops_n_pars);
        MLOG_output_test("readsOpsNPars for 2 ^ 4", TRUE,\
                                        (ops_n_pars[AT__EXPONENT][0] == 1));
        clearTokens(tokens, tokensLen);
}
