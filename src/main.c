#include "mcalc3.h"
#include <stdio.h>


int main(const int argc, const char* argv[]) {
    MC3_ErrorCode error = MC3_NO_ERROR;
    
    for (int i = 1; i < argc; i++) {
        double result = MC3_evaluate(argv[i], &error);

        if (!error_occured(&error)) {
            printf("%s = %lf\n", argv[i], result);
        }
    }
}
