#include "mcalc3.h"


enum MC3_EquTknType {
  EMPTY,
  OP_PLUS,
  OP_MINUS,
  OP_MULTIPLY,
  OP_DIVIDE,
  OP_EXPONENT,
  INTEGER,
  DECIMAL
};


typedef struct MC3_EquToken {
  enum MC3_EquTknType type;
  
  union {
    int64_t ivalue;
    float64_t fvalue;
  };
} MC3_EquToken;


double MC3_evaluate(const char *equation) {
  
} 

