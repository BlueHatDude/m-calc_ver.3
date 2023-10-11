#include "mcalc3.h"
#include "mlogging.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


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


static inline MC3_ErrorCode appendEquTkn(MC3_EquToken tokens[], MC3_EquToken token, size_t maxSize) {
  unsigned i = 0;
  while( tokens[i].type != EMPTY ) {
    if(i < maxSize) {
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
  
  for(unsigned i = 0; i < strLen; ++i) {
    switch(equ[i]) {
      case '+':
        temp_token.type = OPERATOR;
        temp_token.subtype = OP_PLUS;
        appendEquTkn(tokens, temp_token, len);
        break;
      case '-':
        temp_token.type = OPERATOR;
        temp_token.subtype = OP_MINUS;
        appendEquTkn(tokens, temp_token, len);
        break;
      case '*':
        temp_token.type = OPERATOR;
        temp_token.subtype = OP_MULTIPLY;
        appendEquTkn(tokens, temp_token, len);
        break;
      case '/':
        temp_token.type = OPERATOR;
        temp_token.subtype = OP_DIVIDE;
        appendEquTkn(tokens, temp_token, len);
        break;
      case '^':
        temp_token.type = OPERATOR;
        temp_token.subtype = OP_EXPONENT;
        appendEquTkn(tokens, temp_token, len);
        break;
      case '(':
        temp_token.type = L_PAR;
        appendEquTkn(tokens, temp_token, len);
        break;
      case ')':
        temp_token.type = R_PAR;
        appendEquTkn(tokens, temp_token, len);
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


void MC3_RUN(void) {
  MC3_EquToken tokens[25];
  const size_t tokensSize = sizeof(tokens)/sizeof(tokens[0]);
  for(unsigned i = 0; i < tokensSize; ++i)
    tokens[i].type = EMPTY;

  read_equ("4.5 + 4", tokens, tokensSize);
  DEBUGGING_logTokens(tokens, tokensSize);
}


double MC3_evaluate(const char *equation) {
  (void) equation;

  return -0.0;
} 

