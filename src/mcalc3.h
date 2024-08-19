#ifndef MCALCULATOR_VER3_H_
#define MCALCULATOR_VER3_H_


    typedef enum {
        NO_ERROR,
        TOKENS_LIMIT_REACHED
    } MC3_ErrorCode;

    extern double MC3_evaluate(const char* equ, MC3_ErrorCode* err);

#endif /* MCALCULATOR_VER3_H_ */
