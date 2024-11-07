#include "mcalc3.h"
#include "mlogging.h"


void test_tokenization(void) {
    MC3_ErrorCode error_code = MC3_NO_ERROR;

    MC3_evaluate("2 + 4 - 6 * 8 / 10 ^ 12", &error_code);
    MLOG_test(error_code == MC3_NO_ERROR, "2 + 4 - 6 * 8 / 10 ^ 12");
    puts(getErrorString(error_code));

    MC3_evaluate("8 * (2 + 4)", &error_code);
    MLOG_test(error_code == MC3_NO_ERROR, "8 * (2 + 4)");
    MC3_evaluate("sin(pi / 2)", &error_code);
    MLOG_test(error_code == MC3_NO_ERROR, "sin(pi / 2)");
    MC3_evaluate("ln(e ^ 5)", &error_code);
    MLOG_test(error_code == MC3_NO_ERROR, "ln(e ^ 5)");
    MC3_evaluate("2x * 5", &error_code);
    MLOG_test(error_code == MC3_INVALID_CHARACTER_FOUND, "2x * 5");
}


void test_parsing(void) {}


int main(void) {
    test_tokenization();
}
