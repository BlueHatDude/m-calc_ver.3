#include "mcalc3.h"


void testTokenization(void) {
    MC3_evaluate("2 + 4 - 6 * 8 / 10 ^ 12");
    MC3_evaluate("8 * (2 + 4)");
    MC3_evaluate("sin(pi / 2)");
    MC3_evaluate("ln(e ^ 5)");
}


int main(void) {

}
