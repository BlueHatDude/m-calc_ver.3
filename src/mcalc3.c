#include "mcalc3.h"
#include <stdint.h>
#include <stddef.h>


typedef float float32_t;
typedef double float64_t;


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
    INTEGER,
    DECIMAL,
    EMPTY,

    /* functions */
    SIN,
    COS,
    TAN,
    LOG_10,
    LOG_E,

} TokenType;


typedef struct EquToken {
    
    TokenType type;
    
    union {
        int64_t ivalue;
        float64_t fvalue;
    }; 

} EquToken;


static MC3_ErrorCode initTokens(EquToken tokens[], const size_t size);
static MC3_ErrorCode tokenize(EquToken tokens[], const size_t size);


double MC3_evaluate(const char *equ) {
    (void) equ;
    return 0.0;
}
