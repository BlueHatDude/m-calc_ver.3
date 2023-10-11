#include "mcalc3.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


enum MC3_EquTknIdentifier {
  EMPTY,
  OPERATOR,
  INTEGER,
  DECIMAL
};


enum MC3_EquTknType {
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

    enum MC3_EquTknType subtype;
  };
} MC3_EquToken;


MC3_ErrorCode read_equ(const char* equ, MC3_EquToken tokens[]) {
  const size_t strLen = strlen(equ);
  for(unsigned i = 0; i < strLen; ++i) {
    switch(equ[i]) {
      case '+':
        break;
      case '-':
        break;
      case '*':
        break;
      case '/':
        break;
      case '^':
        break;
      default:
        break;
    }
  }
}


double MC3_evaluate(const char *equation) {
  return -0.0;
} 

