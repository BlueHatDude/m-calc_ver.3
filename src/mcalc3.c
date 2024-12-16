#include "mcalc3.h"
#include "mlogging.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOKENS 100

/* ===== String Functions =====*/

void clear_string(char *str, const size_t len) { memset(str, '\0', len); }

/**
 * @brief checks to see if substring is in string at str_start
 *
 * @return true
 * @return false
 */
bool string_at(const char sub_str[], const char *str_start) {
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

struct TreeNode {
    struct Token *token;
    struct Token *left_operand;
    struct Token *right_operand;
};

/* ===== Token Functions =====*/

const char *type_to_str(enum TokenType type) {
    switch (type) {
    case OP_ADD:
        return "OP_ADD";
    case OP_SUB:
        return "OP_SUB";
    case OP_MULT:
        return "OP_MULT";
    case OP_DIV:
        return "OP_DIV";
    case OP_EXP:
        return "OP_EXP";
    case PAR_LEFT:
        return "PAR_LEFT";
    case PAR_RIGHT:
        return "PAR_RIGHT";
    case TYPE_INTEGER:
        return "TYPE_INTEGER";
    case TYPE_DECIMAL:
        return "TYPE_DECIMAL";
    case TYPE_EMPTY:
        return "TYPE_EMPTY";
    case FN_SIN:
        return "FN_SIN";
    case FN_COS:
        return "FN_COS";
    case FN_TAN:
        return "FN_TAN";
    case FN_LOG_10:
        return "FN_LOG_10";
    case FN_LOG_E:
        return "FN_LOG_E";
    case CONSTANT_PI:
        return "CONSTANT_PI";
    case CONSTANT_E:
        return "CONSTANT_E";
    default:
        return NULL;
    }
}

enum TokenType char_to_type(char ch) {
    switch (ch) {
    case '+':
        return OP_ADD;
    case '-':
        return OP_SUB;
    case '*':
        return OP_MULT;
    case '/':
        return OP_DIV;
    case '^':
        return OP_EXP;
    case '(':
        return PAR_LEFT;
    case ')':
        return PAR_RIGHT;
    default:
        return TYPE_EMPTY;
    }
}

enum TokenType str_to_fn(const char *str) {
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

const char *fn_type_to_str(enum TokenType type) {
    switch (type) {
    case FN_SIN:
        return "sin";
    case FN_COS:
        return "cos";
    case FN_TAN:
        return "tan";
    case FN_LOG_10:
        return "log";
    case FN_LOG_E:
        return "ln";
    default:
        return NULL;
    }
}

bool is_operator(char ch) {
    switch (ch) {
    case '+':
    case '-':
    case '*':
    case '/':
    case '^':
        return true;
    default:
        return false;
    }
}

bool is_grouping(char ch) { return (ch == '(') || (ch == ')'); }

bool is_constant(const char *str, const int index) {
    char *funcs[] = {"pi", "e"};
    int len = sizeof(funcs) / sizeof(char *);

    for (int i = 0; i < len; i++) {
        if (string_at(funcs[i], &str[index]))
            return true;
    }

    return false;
}

bool is_func(const char *str, const int index) {
    char *funcs[] = {"sin", "cos", "tan", "log", "ln"};
    int len = sizeof(funcs) / sizeof(char *);

    for (int i = 0; i < len; i++) {
        if (string_at(funcs[i], &str[index]))
            return true;
    }

    return false;
}

bool is_op_type(enum TokenType type) {
    return ((type >= OP_ADD) && (type <= OP_EXP));
}

bool tokens_equal(struct Token a, struct Token b) {
    return (a.type == b.type) && (a.ivalue == b.ivalue) &&
           (a.fvalue == b.fvalue);
}

bool tokens_arr_equal(struct Token a[], enum TokenType b[], int len) {
    for (int i = 0; i < len; i++) {
        if (a[i].type != b[i]) {
            return false;
        }
    }

    return true;
}

void set_token(struct Token *dest, enum TokenType type, double value) {
    dest->type = type;

    if (type == TYPE_INTEGER) {
        dest->ivalue = (long long)value;
    } else if (type == TYPE_DECIMAL) {
        dest->fvalue = value;
    }
}

/* ===== Tokenization Functions =====*/

struct TokensList new_list(void) {
    struct TokensList list = {
        .capacity = MAX_TOKENS, .tkns_pos = 0, .op_pos = 0};

    for (int i = 0; i < MAX_TOKENS; i++) {
        list.tokens[i].type = TYPE_EMPTY;
        list.operators[i] = 0;
    }

    return list;
}

void clear_list(struct TokensList *list) {
    list->tkns_pos = 0;
    list->op_pos = 0;
    for (unsigned int i = 0; i < list->capacity; i++) {
        list->tokens[i].type = TYPE_EMPTY;
        list->tokens[i].ivalue = 0;
        list->tokens[i].fvalue = 0;
        list->operators[i] = 0;
    }
}

void add_operator(struct TokensList *list, const char *equ, int *iterator) {
    if (list->tkns_pos < (list->capacity - 1)) {
        const enum TokenType TYPE = char_to_type(equ[*iterator]);
        set_token(&list->tokens[list->tkns_pos], TYPE, 0);
        list->operators[list->op_pos] = list->tkns_pos;

        list->tkns_pos++;
        (*iterator)++;
        list->op_pos++;
    }
}

void add_number(struct TokensList *list, const char *equ, int *iterator) {
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

void add_function(struct TokensList *list, const char *equ, int *iterator) {

    enum TokenType type = str_to_fn(&equ[*iterator]);
    int len = strlen(fn_type_to_str(type));

    if (type == TYPE_EMPTY) {
        /* string was not a function */
        // TODO: add functionality
    } else {
        set_token(&list->tokens[list->tkns_pos], type, 0);
        list->tkns_pos++;
        *iterator += len;
    }
}

void add_constant(struct TokensList *list, const char *equ, int *iterator) {
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

struct Token get_token_at(struct TokensList *list, unsigned int index) {
    if (index >= list->capacity) {
        MLOG_error("Trying to access invalid index of TokensList");
        exit(EXIT_FAILURE);
    }

    return list->tokens[index];
}

enum TokenType get_type_at(struct TokensList *list, unsigned int index) {
    if (index >= list->capacity) {
        MLOG_error("Trying to access invalid index of TokensList");
        exit(EXIT_FAILURE);
    }

    return list->tokens[index].type;
}

/**
 * @brief Takes string `equ` and tokenizes it into Tokens, writing to `list`.
 *
 * @param equ
 * @param list
 * @return MC3_ErrorCode
 */
void tokenize(const char *equ, struct TokensList *list, MC3_ErrorCode *err) {
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

/* ===== Parsing Functions =====*/

struct Parser {
    struct TokensList *token_list;
    unsigned int index;
};

/**
 * @brief Parses num/par and evaluates any adjacent multiplication/divison.
 */
double parse_multdiv(struct Parser *parser);

/**
 * @brief Parses num/par and evaluates any adjacent addition/subtraction.
 */
double parse_addsub(struct Parser *parser);

/**
 * @brief Parses num/par. Returns the value of the expression
 */
double parse_numpar(struct Parser *parser);

struct Parser new_parser(struct TokensList *list) {
    struct Parser parser = {
        .token_list = list,
        .index = 0,
    };

    return parser;
}

struct Token get_current(struct Parser *parser) {
    return parser->token_list->tokens[parser->index];
}

void consume(struct Parser *parser, enum TokenType type) {
    if (get_current(parser).type == type) {
        // MLOG_logf("Consumed token of type: %s", type_to_str(type));
        parser->index++;
    } else {
        MLOG_errorf("Expected a different type: %d", type);
        exit(EXIT_FAILURE);
    }
}

double parse_multdiv(struct Parser *parser) {
    double value = parse_numpar(parser);
    struct Token current = get_current(parser);

    while (current.type == OP_MULT || current.type == OP_DIV) {
        if (current.type == OP_MULT) {
            consume(parser, OP_MULT);
            value *= parse_numpar(parser);
        } else {
            consume(parser, OP_DIV);
            value /= parse_numpar(parser);
        }

        current = get_current(parser);
    }

    return value;
}

double parse_addsub(struct Parser *parser) {
    double value = parse_multdiv(parser);
    struct Token current = get_current(parser);

    while (current.type == OP_ADD || current.type == OP_SUB) {
        if (current.type == OP_ADD) {
            consume(parser, OP_ADD);
            value += parse_numpar(parser);
        } else {
            consume(parser, OP_SUB);
            value -= parse_numpar(parser);
        }

        current = get_current(parser);
    }

    return value;
}

double parse_numpar(struct Parser *parser) {
    double value;
    struct Token current = get_current(parser);

    if (current.type == TYPE_INTEGER || current.type == TYPE_DECIMAL) {
        if (current.type == TYPE_INTEGER) {
            value = (double)current.ivalue;
            consume(parser, TYPE_INTEGER);
            return value;
        } else if (current.type == TYPE_DECIMAL) {
            value = current.fvalue;
            consume(parser, TYPE_DECIMAL);
            return value;
        }
    } else if (current.type == PAR_LEFT) {
        consume(parser, PAR_LEFT);
        value = parse_numpar(parser);
        consume(parser, PAR_RIGHT);
        return value;
    } else {
        MLOG_error("Expected factor (num or par)");
        exit(1);
    }

    return -1;
}

/**
 * @brief generates a parse tree from list of tokens.
 *
 * @returns A reference to the root of the parse tree.
 */
double parse(struct TokensList *list) {
    struct Parser parser = new_parser(list);
    double result = parse_addsub(&parser);
    return result;
}

/* ===== Error Handling Functions =====*/

const char *get_error_str(const MC3_ErrorCode err) {
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

void write_error(MC3_ErrorCode *err_obj, MC3_ErrorCode code) {
    if (err_obj != NULL)
        *err_obj = code;
}

// /* ===== Main Function =====*/

// /**
//  * @brief evaluates a mathematical expression from a string, returning the
//  * result as a double. Calculator supports all basic arithmetic operators:
//  * +, -, *, /, and ^. Also supports basic scientific functions such as ln,
//  * sin and constants such as e and pi. The calculator follows  standard
//  PEMDAS
//  * order of operations.
//  *
//  * @param equ
//  * @param err if parameter is NULL, then the error will not be set.
//  Otherwise,
//  * the value of any error that occurs is written to `err`.
//  * @return double
//  */
double MC3_evaluate(const char *equ, MC3_ErrorCode *err) {
    struct TokensList tokens_list = new_list();
    MC3_ErrorCode error_code = MC3_NO_ERROR;

    tokenize(equ, &tokens_list, &error_code);
    if (error_code != MC3_NO_ERROR) {
        write_error(err, error_code);
        return 0.0;
    }

    double result = parse(&tokens_list);

    return result;
}

/* ==== Tests ==== */

void print_token(struct Token* token) {
    if (token->type == TYPE_INTEGER) {
        printf("INTEGER(%lld)", token->ivalue);
    } else if (token->type == TYPE_DECIMAL) {
        printf("DECIMAL(%lf)", token->fvalue);
    } else {
        printf("%s", type_to_str(token->type));
    }
}

void test_tokenization(void) {
    struct TokensList list = new_list();

    MLOG_log("Testing Suite: Tokenization");

    clear_list(&list);
    tokenize("2 + 4", &list, NULL);
    MLOG_test("2 + 4", tokens_arr_equal(
                  list.tokens,
                  (enum TokenType[]){TYPE_INTEGER, OP_ADD, TYPE_INTEGER}, 3));
    // MLOG_array_custom(list.tokens, list.tkns_pos, print_token);

    // tokenize("2 + 4 - 6 * 8 / 10 ^ 12", &list, NULL);
    // MLOG_test(tokens_arr_equal(list.tokens,
    //                            (enum TokenType[]){
    //                                TYPE_INTEGER, OP_ADD, TYPE_INTEGER,
    //                                OP_SUB, TYPE_INTEGER, OP_MULT,
    //                                TYPE_INTEGER, OP_DIV, TYPE_INTEGER,
    //                                OP_EXP, TYPE_INTEGER},
    //                            11),
    //           "2 + 4 - 6 * 8 / 10 ^ 12")
    // tokenize("2 + 4 - 6 * 8 / 10 ^ 12", , &error_code);
    // MLOG_test(error_code == MC3_NO_ERROR, "2 + 4 - 6 * 8 / 10 ^ 12");
    // tokenize("8 * (2 + 4)", &error_code);
    // MLOG_test(error_code == MC3_NO_ERROR, "8 * (2 + 4)");
    // tokenize("sin(pi / 2)", &error_code);
    // MLOG_test(error_code == MC3_NO_ERROR, "sin(pi / 2)");
    // tokenize("ln(e ^ 5)", &error_code);
    // MLOG_test(error_code == MC3_NO_ERROR, "ln(e ^ 5)");
    // tokenize("2x * 5", &error_code);
    // MLOG_test(error_code == MC3_INVALID_CHARACTER_FOUND, "2x * 5");
    // tokenize("5 Ω 2", &error_code);
    // MLOG_test(error_code == MC3_INVALID_CHARACTER_FOUND, "5 Ω 2");
}

void test_evaulation(void) {
    double result = 0.0;

    MLOG_log("Testing Suite: Evaluation");

    result = MC3_evaluate("2 + 4", NULL);
    MLOG_test("2 + 4", ((int) result) == 6);
    // MLOG_logf("Result: %lf\n", result);

    // result = MC3_evaluate("2 * (4 + 8)", NULL);
    // MLOG_test(((int)result) == 24, "2 * (4 + 8)");
    // MLOG_logf("Result: %lf\n", result);

    // result = MC3_evaluate("(2 + 4) * 8", NULL);
    // MLOG_test(((int)result) == 48, "(2 + 4) * 8");
    // MLOG_logf("Result: %lf\n", result);

    // result = MC3_evaluate("(2 + 4) / (6 + 8)", NULL);
    // MLOG_test(result == (6.0 / 14), "(2 + 4) / (6 + 8)");
    // MLOG_logf("Result: %lf\n", result);
}
