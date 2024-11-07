#ifndef MCALCULATOR_VER3_H_
#define MCALCULATOR_VER3_H_


    typedef enum {
        MC3_NO_ERROR,
        MC3_TOKENS_LIMIT_REACHED,
        MC3_INVALID_CHARACTER_FOUND
    } MC3_ErrorCode;

    extern double MC3_evaluate(const char* equ, MC3_ErrorCode* err);
    extern const char* getErrorString(const MC3_ErrorCode err);

#endif /* MCALCULATOR_VER3_H_ */
