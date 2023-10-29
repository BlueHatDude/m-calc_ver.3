#ifndef __MTECH__M_CALCULATOR_H_
    #define __MTECH__M_CALCULATOR_H_

    #include <stdint.h>
    typedef float float32_t;
    typedef double float64_t;
    typedef long double float128_t;
    typedef enum { FALSE, TRUE } bool_t;


    typedef enum MC3_ErrorCode {
        NO_ERROR,
        NOT_ENOUGH_SPACE,
        DIV_BY_ZERO,
    } MC3_ErrorCode;


    void MC3_RUN(void);
    void MC3_TESTS(void);
    double MC3_evaluate(const char *equation);


    #include <string.h>
    static inline bool_t charInStr(const char argChar, const char argStr[]) {
        const size_t strLen = strlen(argStr);
        for(unsigned i = 0; i < strLen; ++i) {
            if(argChar == argStr[i])
                return TRUE;
        }
        return FALSE;
    }


#endif

