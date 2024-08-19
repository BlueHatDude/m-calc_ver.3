#include "mcalc3.h"
#include <stddef.h>


void test_tokenization(void) {
    MC3_evaluate("2 + 4 - 6 * 8 / 10 ^ 12", NULL);
    MC3_evaluate("8 * (2 + 4)", NULL);
    MC3_evaluate("sin(pi / 2)", NULL);
    MC3_evaluate("ln(e ^ 5)", NULL);
}


int main(void) {
    test_tokenization();
}
