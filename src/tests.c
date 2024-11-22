#include "mcalc3.h"
#include "mlogging.h"


void test_tokenization(void) {
    MC3_ErrorCode error_code = MC3_NO_ERROR;

    MLOG_log("Testing Suite: Tokenization");
    MC3_evaluate("2 + 4 - 6 * 8 / 10 ^ 12", &error_code);
    MLOG_test(error_code == MC3_NO_ERROR, "2 + 4 - 6 * 8 / 10 ^ 12");
    MC3_evaluate("8 * (2 + 4)", &error_code);
    MLOG_test(error_code == MC3_NO_ERROR, "8 * (2 + 4)");
    MC3_evaluate("sin(pi / 2)", &error_code);
    MLOG_test(error_code == MC3_NO_ERROR, "sin(pi / 2)");
    MC3_evaluate("ln(e ^ 5)", &error_code);
    MLOG_test(error_code == MC3_NO_ERROR, "ln(e ^ 5)");
    MC3_evaluate("2x * 5", &error_code);
    MLOG_test(error_code == MC3_INVALID_CHARACTER_FOUND, "2x * 5");
    MC3_evaluate("5 Ω 2", &error_code);
    MLOG_test(error_code == MC3_INVALID_CHARACTER_FOUND, "5 Ω 2");
}

void test_parsing(void) {}

void test_evaulation(void) {
    double result = 0.0;

    MLOG_log("Testing Suite: Evaluation");

    result = MC3_evaluate("2 + 4", NULL);
    MLOG_test(((int)result) == 6, "2 + 4");

    result = MC3_evaluate("2 * (4 + 8)", NULL);
    MLOG_test(((int) result) == 24, "2 * (4 + 8)");

    result = MC3_evaluate("(2 + 4) * 8", NULL);
    MLOG_test(((int)result) == 48, "(2 + 4) * 8");

    result = MC3_evaluate("(2 + 4) / (6 + 8)", NULL);
    MLOG_test(result == ((double) 6 / 14), "(2 + 4) / (6 + 8)");
}

int main(void) {
    test_tokenization();
    test_evaulation();
}
