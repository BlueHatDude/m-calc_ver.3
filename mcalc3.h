#ifndef __MTECH__M_CALCULATOR_H_
    #define __MTECH__M_CALCULATOR_H_

    #include <stdint.h>
    typedef float float32_t;
    typedef double float64_t;
    typedef long double float128_t;
    typedef enum { FALSE, TRUE } bool_t;


    typedef enum MC3_ErrorCode {
        DIV_BY_ZERO,
    } MC3_ErrorCode;


    double MC3_evaluate(const char *equation);


#endif

