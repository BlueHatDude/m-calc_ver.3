#ifndef MCALCULATOR_VER3_H_
#define MCALCULATOR_VER3_H_


    typedef enum {
        MC3_NO_ERROR,
        MC3_TOKENS_LIMIT_REACHED,
        MC3_INVALID_CHARACTER_FOUND
    } MC3_ErrorCode;

    extern double MC3_evaluate(const char* equ, MC3_ErrorCode* err);
    extern const char* getErrorString(const MC3_ErrorCode err);

    static inline int error_occured(MC3_ErrorCode* err) {
        return (*err != MC3_NO_ERROR);
    }

    /* ==== Tests ==== */
    void test_tokenization(void);
    void test_parsing(void);
    void test_evaulation(void);

#endif /* MCALCULATOR_VER3_H_ */
