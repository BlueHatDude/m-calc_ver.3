#include "mcalc3.h"
#include "mlogging.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>


enum MC3_EquTknIdentifier {
  EMPTY,
  OPERATOR,
  L_PAR,
  R_PAR,
  INTEGER,
  DECIMAL
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

    enum MC3_EquTknSubType subtype;
  };
} MC3_EquToken;


enum OpsNParsArrType {
  INDEX_TRACKERS    = 0,
  LEFT_PARENTHESIS  = 1,
  RIGHT_PARENTHESIS = 2,
  EXPONENT          = 3,
  MULT_AND_DIV      = 4,
  ADD_AND_SUB       = 5,
};


enum OpsNParsPtrType{
  PARENTHESIS = 0,
  EXPONENTS   = 1,
  MULT_N_DIVS = 2,
  ADD_N_SUB   = 3,
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


static MC3_ErrorCode read_equ(const char equ[], MC3_EquToken tokens[], const size_t len) {
  const size_t strLen = strlen(equ);
  MC3_EquToken temp_token = { .type = EMPTY };
  char str_num[15] = "              "; // 14 width empty string
  MC3_ErrorCode error_code = NO_ERROR;
  
  for(unsigned i = 0; i < strLen; ++i) {
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
          if(equ[i] == '.') 
            isDecimal = TRUE;
          
          str_num[j] = equ[i];
          i++;
          j++;
        }

        if(isDecimal) {
          temp_token.type = DECIMAL;
          temp_token.fvalue = atof(str_num);
          appendEquTkn(tokens, temp_token, len);
        } else {
          temp_token.type = INTEGER;
          temp_token.ivalue = atoi(str_num);
          appendEquTkn(tokens, temp_token, len);
        }
        strcpy(str_num, "              "); // resets str_num to empty string

        break;
      }
    }
  }

  return error_code;
}


static MC3_ErrorCode read_ops_and_pars(MC3_EquToken tokens[], int8_t ops_and_pars[6][8]) {
  size_t tokensEnd = 0;
  while(tokens[tokensEnd].type != EMPTY)
    tokensEnd++;
  
  for(int8_t i = tokensEnd; i != -1; --i) {
    if(tokens[i].type == OPERATOR) {
      switch(tokens[i].subtype) {
        case OP_PLUS:
          ops_and_pars[ADD_AND_SUB][ ops_and_pars[INDEX_TRACKERS][ADD_AND_SUB] ] = i;
          ops_and_pars[INDEX_TRACKERS][ADD_AND_SUB]++;
          break;
        case OP_MINUS:
          ops_and_pars[ADD_AND_SUB][ ops_and_pars[INDEX_TRACKERS][ADD_AND_SUB] ] = i;
          ops_and_pars[INDEX_TRACKERS][ADD_AND_SUB]++;
          break;
        case OP_MULTIPLY:
          ops_and_pars[MULT_AND_DIV][ ops_and_pars[INDEX_TRACKERS][MULT_N_DIVS] ] = i;
          ops_and_pars[INDEX_TRACKERS][MULT_N_DIVS]++;
          break;
        case OP_DIVIDE:
          ops_and_pars[MULT_AND_DIV][ ops_and_pars[INDEX_TRACKERS][MULT_N_DIVS] ] = i;
          ops_and_pars[INDEX_TRACKERS][MULT_N_DIVS]++;
          break;
        case OP_EXPONENT:
          ops_and_pars[EXPONENT][ ops_and_pars[INDEX_TRACKERS][EXPONENTS] ] = i;
          ops_and_pars[INDEX_TRACKERS][EXPONENTS]++;
          break;
      }
    } else if(tokens[i].type == L_PAR) {
      size_t temp_index = i;
      while(tokens[temp_index].type != R_PAR) temp_index++;

      ops_and_pars[LEFT_PARENTHESIS][ ops_and_pars[INDEX_TRACKERS][PARENTHESIS] ] = i;
      ops_and_pars[RIGHT_PARENTHESIS][ ops_and_pars[INDEX_TRACKERS][PARENTHESIS] ] = temp_index;
      ops_and_pars[INDEX_TRACKERS][PARENTHESIS]++;
    } 
  }

  return NO_ERROR;
}


void DEBUGGING_logTokens(MC3_EquToken tokens[], size_t size) {  
  for(unsigned i = 0; i < size; ++i) {
    switch(tokens[i].type) {
      case EMPTY:
        printf("Index: %2d | Type: EMPTY \n", i);
        break;
      case INTEGER:
        printf("Index: %2d | Type: INTEGER | ivalue: %ld \n", i, tokens[i].ivalue);
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
    }    
  }
}


double MC3_evaluate(const char *equation) {
  (void) equation;

  return -0.0;
} 


void MC3_RUN(void) {
  MC3_EquToken tokens[25];
  // operators and parenthesis
  int8_t ops_n_pars[6][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0}, // left parenthesis
    {0, 0, 0, 0, 0, 0, 0, 0}, // right parenthesis
    {0, 0, 0, 0, 0, 0, 0, 0}, // exponents
    {0, 0, 0, 0, 0, 0, 0, 0}, // multiplication and division
    {0, 0, 0, 0, 0, 0, 0, 0}, // addition and subtraction
  };
  (void) ops_n_pars;

  const size_t tokensSize = sizeof(tokens)/sizeof(tokens[0]);
  for(unsigned i = 0; i < tokensSize; ++i)
    tokens[i].type = EMPTY;

  read_equ("4.5 + 4", tokens, tokensSize);
  read_ops_and_pars(tokens, ops_n_pars);
}


void MC3_TESTS(void) {
  MC3_EquToken tokens[25];
  const size_t len = sizeof(tokens) / sizeof(tokens[0]);

  /* read_equ */
  read_equ("2 + 4", tokens, len);
  MLOG_output_test("2 + 4 properly tokenized", TRUE,
    (tokens[0].type == INTEGER && tokens[0].ivalue == 2)
    && (tokens[1].type == OPERATOR && tokens[1].subtype == OP_PLUS)
    && (tokens[2].type == INTEGER && tokens[2].ivalue == 4)
  );


}

