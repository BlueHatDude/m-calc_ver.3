#ifndef MLOGGING_LIBRARY_H_
    #define MLOGGING_LIBRARY_H_

    /** Terminal Colors */

    /* Reset */
    #define SET_DEFAULT "\033[0m"
    /* Terminal Colors */
    #define NO_COLOR SET_DEFAULT
    #define TC_BLACK  "\033[0;30m"
    #define TC_RED    "\033[0;31m"
    #define TC_GREEN  "\033[0;32m"
    #define TC_YELLOW "\033[0;33m"
    #define TC_BLUE   "\033[0;34m"
    #define TC_PURPLE "\033[0;35m"
    #define TC_CYAN   "\033[0;36m"
    #define TC_WHITE  "\033[0;37m"

    /* Logging Functions */

    #include <stdio.h>
    #include <stdarg.h>

    /* basic logging */

    // ! WARNING: this function should not be called directly. Instead, use corresponding macro
    // * Newline is included as part of function.
    static void mlog_log(const char* msg) {
        printf("[LOG]: %s\n", msg);
    }

    // ! WARNING: this function should not be called directly. Instead, use corresponding macro.
    // * Newline is included as part of function.
    static void mlog_logf(const char* msg, ...) {
        va_list args;
        va_start(args, msg);
        printf("[LOG]: ");
        vprintf(msg, args);
        puts("");
        va_end(args);
    }

    // ! WARNING: this function should not be called directly. Instead, use corresponding macro.
    // * Newline is included as part of function.
    static void mlog_logc(const char* color, const char* msg) {
        printf("%s[LOG]: %s%s\n", color, msg, NO_COLOR);
    }

    // ! WARNING: this function should not be called directly. Instead, use corresponding macro.
    // * Newline is included as part of function.
    static void mlog_logfc(const char* color, const char* msg, ...) {
        va_list args;
        va_start(args, msg);
        printf("%s[LOG]: ", color);
        vprintf(msg, args);
        puts(NO_COLOR);
        va_end(args);

    }

    static void mlog_array_int(int arr[], size_t size) {
        printf("[LOG]: [");
        for (size_t i = 0; i < size; i++) {
            printf("%d%s", arr[i], (i == (size - 1) ? "" : ", "));
        }
        puts("]");
    }

    static void mlog_array_char(char arr[], size_t size) {
        printf("[LOG]: [");
        for (size_t i = 0; i < size; i++) {
            printf("%c%s", arr[i], (i == (size - 1) ? "" : ", "));
        }
        puts("]");
    }


    static void mlog_array_double(double arr[], size_t size) {
        printf("[LOG]: [");
        for (size_t i = 0; i < size; i++) {
            printf("%lf%s", arr[i], (i == (size - 1) ? "" : ", "));
        }
        puts("]");
    }

    static void mlog_array_str(char* arr[], size_t size) {
        printf("[LOG]: [");
        for (size_t i = 0; i < size; i++) {
            printf("%s%s", arr[i], (i == (size - 1) ? "" : ", "));
        }
        puts("]");
    }

    /* error logging */

    // ! WARNING: this function should not be called directly. Instead, use corresponding macro.
    // * Newline is included as part of function.
    static void mlog_error(const char* msg) {
        fprintf(stderr, "[ERROR]: %s\n", msg);
    }

    // ! WARNING: this function should not be called directly. Instead, use corresponding macro.
    // * Newline is included as part of function.
    static void mlog_errorf(const char* msg, ...) {
        va_list args;
        va_start(args, msg);
        fprintf(stderr, "[ERROR]: ");
        vfprintf(stderr, msg, args);
        puts("");
        va_end(args);
    }

    // ! WARNING: this function should not be called directly. Instead, use corresponding macro.
    // * Newline is included as part of function.
    static void mlog_errorc(const char* color, const char* msg) {
        fprintf(stderr, "%s[ERROR]: %s%s\n", color, msg, NO_COLOR);
    }

    // ! WARNING: this function should not be called directly. Instead, use corresponding macro.
    // * Newline is included as part of function.
    static void mlog_errorfc(const char* color, const char* msg, ...) {
        va_list args;
        va_start(args, msg);
        fprintf(stderr, "%s[ERROR]: ", color);
        vfprintf(stderr, msg, args);
        puts(NO_COLOR);
        va_end(args);
    }


    /* test logging */

    static void mlog_test(int cond, const char* tag) {
        if (cond) {
            printf("[TEST] %s: %s[PASSED]%s\n", tag, TC_GREEN, NO_COLOR);
        } else {
            printf("[TEST] %s: %s[FAILED]%s\n", tag, TC_RED, NO_COLOR);
        }
    }

    static void mlog_testf(int cond, const char* tag, ...) {
        va_list args;
        va_start(args, tag);
        printf("[TEST] ");
        vprintf(tag, args);

        if (cond) {
            puts(": " TC_GREEN "[PASSED]" NO_COLOR);
        } else {
            puts(": " TC_RED "[FAILED]" NO_COLOR);
        }

        va_end(args);
    }

    static void mlog_testc(const char* color, int cond, const char* tag) {
        if (cond) {
            printf("%s[TEST] %s: %s[PASSED]%s\n", color, tag, TC_GREEN, NO_COLOR);
        } else {
            printf("%s[TEST] %s: %s[FAILED]%s\n", color, tag, TC_RED, NO_COLOR);
        }
    }

    static void mlog_testfc(const char* color, int cond, const char* tag, ...) {
        va_list args;
        va_start(args, tag);
        printf("%s[TEST] ", color);
        vprintf(tag, args);

        if (cond) {
            puts(": " TC_GREEN "[PASSED]" NO_COLOR);
        } else {
            puts(": " TC_RED "[FAILED]" NO_COLOR);
        }

        va_end(args);
    }

    #ifndef MLOGOFF
        /* standard */
        #define MLOG_log(msg) mlog_log(msg);
        #define MLOG_error(msg) mlog_error(msg);
        #define MLOG_test(cond, tag) mlog_test(cond, tag);

        /* arrays */
        #define MLOG_array_int(arr, size) mlog_array_int(arr, size);
        #define MLOG_array_char(arr, size) mlog_array_char(arr, size);
        #define MLOG_array_double(arr, size) mlog_array_double(arr, size);
        #define MLOG_array_str(arr, size) mlog_array_str(arr, size);

        /* format */
        #define MLOG_logf(msg, ...) mlog_logf(msg, __VA_ARGS__);
        #define MLOG_errorf(msg, ...) mlog_errorf(msg, __VA_ARGS__);
        #define MLOG_testf(cond, tag, ...) mlog_testf(cond, tag, __VA_ARGS__);

        /* color */
        #define MLOG_logc(color, msg) mlog_logc(color, msg);
        #define MLOG_errorc(color, msg) mlog_errorc(color, msg);
        #define MLOG_testc(color, cond, tag) mlog_testc(color, cond, tag);

        /* color and format */
        #define MLOG_logfc(color, msg, ...) mlog_logfc(color, msg, __VA_ARGS__);
        #define MLOG_errorfc(color, msg, ...) mlog_errorfc(color, msg, __VA_ARGS__);
        #define MLOG_testfc(color, cond, tag, ...) mlog_testfc(color, cond, tag, __VA_ARGS__);
    #endif

    #ifdef MLOGOFF
        #define MLOG_log(msg) ((void) 0);
        #define MLOG_error(msg) ((void) 0);
        #define MLOG_test(cond, msg) ((void) 0);

        #define MLOG_logf(msg, ...) ((void) 0);
        #define MLOG_errorf(msg, ...) ((void) 0);
        #define MLOG_testf(cond, msg) ((void) 0);

        #define MLOG_logc(color, msg) ((void) 0);
        #define MLOG_errorc(color, msg) ((void) 0);
        #define MLOG_testc(color, cond, msg) ((void) 0);

        #define MLOG_logfc(color, msg, ...) ((void) 0);
        #define MLOG_errorfc(color, msg, ...) ((void) 0);
        #define MLOG_testfc(color, cond, msg, ...) ((void) 0);
    #endif
#endif
