#include "mcalc3.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "mlogging.h"

#define MAX_TOKENS 100

/* ===== String Functions =====*/

static inline void clear_string(char* str, const size_t len) {
    memset(str, '\0', len);
}

/**
 * @brief checks to see if substring is in string at str_start 
 * 
 * @return true 
 * @return false 
 */
static inline bool string_at(const char sub_str[], const char* str_start) {
    return (strstr(str_start, sub_str) == str_start);
}

/* ===== Data Types  =====*/

enum TokenType {

    /* operators */
    OP_ADD,
    OP_SUB,
    OP_MULT,
    OP_DIV,
    OP_EXP,

    /* grouping */
    PAR_LEFT,
    PAR_RIGHT,

    /* types */
    TYPE_INTEGER,
    TYPE_DECIMAL,
    TYPE_EMPTY,

    /* functions */
    FN_SIN,
    FN_COS,
    FN_TAN,
    FN_LOG_10,
    FN_LOG_E,

    /* constants */
    CONSTANT_PI,
    CONSTANT_E

};

struct Token {
    /* Stores the type of the token, TYPE_EMPTY means the token hasn't been
       intialized. */
    enum TokenType type;
    
    /* Stores the value contained in the token, varies based on type. Using an
       integer and floating point data type allows for storing larger range of
       numbers */
    union {
        long long ivalue;
        double fvalue;
    }; 

    /* Used for constructing parse tree, should only be non-NULL for
    operators */
    struct Token* left_operand;
    struct Token* right_operand;
};

struct TokensList {
    /* array of tokens */
    struct Token tokens[MAX_TOKENS];
    /* maximum number of tokens which can be stored */
    unsigned int capacity;
    /* current index of tokens */
    unsigned int tkns_pos;
    /* indexes of operators in tokens */
    unsigned int operators[MAX_TOKENS];
    /* current index of operators[] */
    unsigned int op_pos;
};

/* ===== Tokenization Functions =====*/

static enum TokenType char_to_type(char ch) {
    switch (ch) {
        case '+': return OP_ADD;
        case '-': return OP_SUB;
        case '*': return OP_MULT;
        case '/': return OP_DIV;
        case '^': return OP_EXP;
        case '(': return PAR_LEFT;
        case ')': return PAR_RIGHT;
        default:  return TYPE_EMPTY;
    }
}

static enum TokenType str_to_fn(const char* str) {
    if (strncmp(str, "sin", 3) == 0) {
        return FN_SIN;
    } else if (strncmp(str, "cos", 3) == 0) {
        return FN_COS;
    } else if (strncmp(str, "tan", 3) == 0) {
        return FN_TAN;
    } else if (strncmp(str, "log", 3) == 0) {
        return FN_LOG_10;
    } else if (strncmp(str, "ln", 2) == 0) {
        return FN_LOG_E;
    } else {
        return TYPE_EMPTY;
    }
}

static const char* fn_type_to_str(enum TokenType type) {
    switch (type) {
        case FN_SIN: return "sin";
        case FN_COS: return "cos";
        case FN_TAN: return "tan";
        case FN_LOG_10: return "log";
        case FN_LOG_E: return "ln";
        default: return NULL;
    }
}

static bool is_operator(char ch) {
    switch (ch) {
        case '+': case '-': case '*': case '/': case '^':
            return true;
        default:
            return false;
    }
}

static bool is_grouping(char ch) {
    return (ch == '(') || (ch == ')');
}

static bool is_constant(const char* str, const int index) {
    char* funcs[] = {"pi", "e"};
    int len = sizeof(funcs)/sizeof(char*);

    for (int i = 0; i < len; i++) {
        if (string_at(funcs[i], &str[index]))
            return true;
    }

    return false;
}

static bool is_func(const char* str, const int index) {
    char* funcs[] = {"sin", "cos", "tan", "log", "ln"};
    int len = sizeof(funcs)/sizeof(char*);

    for (int i = 0; i < len; i++) {
        if (string_at(funcs[i], &str[index]))
            return true;
    }

    return false;
}

static inline void set_token(struct Token* dest, enum TokenType type,
                             double value) {
    dest->type = type;

    if (type == TYPE_INTEGER) {
        dest->ivalue = (long long) value;
    } else if (type == TYPE_DECIMAL) {
        dest->fvalue = value;
    }
}

struct TokensList new_list(void) {
    struct TokensList list = {
        .capacity=MAX_TOKENS,
        .tkns_pos=0,
        .op_pos=0
    };

    for (int i = 0; i < MAX_TOKENS; i++) {
        list.tokens[i].type = TYPE_EMPTY;
        list.tokens[i].left_operand = NULL;
        list.tokens[i].right_operand = NULL;
        list.operators[i] = 0;
    }

    return list;
}

static void add_operator(struct TokensList* list, const char* equ,
                         int* iterator) {
    if (list->tkns_pos < (list->capacity - 1)) {
        const enum TokenType TYPE = char_to_type(equ[*iterator]);
        set_token(&list->tokens[list->tkns_pos], TYPE, 0);
        list->operators[list->op_pos] = list->tkns_pos;


        list->tkns_pos++;
        (*iterator)++;
        list->op_pos++;
    }
}

static void add_number(struct TokensList* list, const char* equ,
                       int* iterator) {
    bool is_decimal = false;
    const int START_INDEX = *iterator;

    while (isdigit(equ[*iterator]) || equ[*iterator] == '.') {
        if (equ[*iterator] == '.')
            is_decimal = true;
        (*iterator)++;
    }

    if (is_decimal) {
        double value = strtod(&equ[START_INDEX], NULL);
        set_token(&list->tokens[list->tkns_pos], TYPE_DECIMAL, value);
        list->tkns_pos++;
    } else {
        int value = strtoll(&equ[START_INDEX], NULL, 10);
        set_token(&list->tokens[list->tkns_pos], TYPE_INTEGER, value);
        list->tkns_pos++;
    }
}

static void add_function(struct TokensList* list, const char* equ,
                         int* iterator) {

    enum TokenType type = str_to_fn(&equ[*iterator]);
    int len = strlen(fn_type_to_str(type));

    if (type == TYPE_EMPTY) {
        /* string was not a function */
        ;;;
    } else {
        set_token(&list->tokens[list->tkns_pos], type, 0);
        list->tkns_pos++;
        *iterator += len;
    }
}

static void add_constant(struct TokensList* list, const char* equ,
                         int* iterator) {
    if (string_at("pi", &equ[*iterator])) {
        set_token(&list->tokens[list->tkns_pos], CONSTANT_PI, 0);   
        list->tkns_pos++;
        *iterator += 2; 
    } else if (string_at("e", &equ[*iterator])) {
        set_token(&list->tokens[list->tkns_pos], CONSTANT_E, 0);
        list->tkns_pos++;
        *iterator += 1;
    }
}

static enum TokenType get_type_at(struct TokensList* list, const int index) {
    return list->tokens[index].type;
}

/* ===== Parsing Functions =====*/



/* ===== Calculator Functions =====*/

/**
 * @brief Takes string `equ` and tokenizes it into EquTokens, writing to `list`.
 *  
 * @param equ
 * @param list
 * @return MC3_ErrorCode 
 */
static void tokenize(const char* equ, struct TokensList* list,
                     MC3_ErrorCode* err) {
    MC3_ErrorCode err_code = MC3_NO_ERROR;
    const int EQU_LENGTH = strlen(equ);

    int i = 0;
    while (i < EQU_LENGTH) {
        if (is_operator(equ[i]) || is_grouping(equ[i])) {
            /* parenthesis are also considered operators in this context */
            add_operator(list, equ, &i);
        } else if (isdigit(equ[i])) {
            add_number(list, equ, &i);
        } else if (is_func(equ, i)) {
            add_function(list, equ, &i);
        } else if (is_constant(equ, i)) {
            add_constant(list, equ, &i);
        } else if (equ[i] == ' ') {
            i++;
        } else {
            err_code = MC3_INVALID_CHARACTER_FOUND;
        }
    }

    if (err != NULL)
        *err = err_code;
}

/**
 * @brief Takes list of tokens 
 * 
 * @param list Parameter should already be initalized via the tokenize()
 * function. 
 * @return MC3_ErrorCode 
 */
static void parse(struct TokensList* list, MC3_ErrorCode* err);

/* ===== Error Handling Functions =====*/

const char* get_error_str(const MC3_ErrorCode err) {
    switch (err) {
    case MC3_NO_ERROR:
        return "No error";
    case MC3_TOKENS_LIMIT_REACHED:
        return "Tokens limit Reached. Expression too long.";
    case MC3_INVALID_CHARACTER_FOUND:
        return "Invalid character was found.";
    default:
        return "Invalid Error Code";
    }
}

void write_error(MC3_ErrorCode* err_obj, MC3_ErrorCode code) {
    if (err_obj != NULL)
        *err_obj = code;
}

/* ===== Main Function =====*/

/**
 * @brief evaluates a mathematical expression from a string, returning the
 * result as a double. Calculator supports all basic arithmetic operators:
 * +, -, *, /, and ^. Also supports basic scientific functions such as ln,
 * sin and constants such as e and pi. The calculator follows  standard PEMDAS
 * order of operations. 
 * 
 * @param equ 
 * @param err if parameter is NULL, then the error will not be set. Otherwise,
 * the value of any error that occurs is written to `err`.
 * @return double 
 */
double MC3_evaluate(const char *equ, MC3_ErrorCode* err) {
    struct TokensList tokens_list = new_list();
    MC3_ErrorCode error_code = MC3_NO_ERROR;

    tokenize(equ, &tokens_list, &error_code);
    if (error_code != MC3_NO_ERROR)
        goto error_occured;

    

    /* goto label for consistent error handling */
    error_occured: {
        write_error(err, error_code);
        return 0.0;
    }

    return -0.0;
}

/* ==== Tests ==== */

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

void test_parsing(void) {

}

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
