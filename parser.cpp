#include "lexer.h"
#include "ast.h"

static Token curr_tok;

static Token get_next_tok() {
    curr_tok = get_token();
    return curr_tok;
}

// Error handling

std::unique_ptr<Expr> log_err_expr(const char* str) {
    fprintf(stderr, "log_err: %s\n", str);
    return nullptr;
}

std::unique_ptr<Proto> log_err_proto(const char* str) {
    fprintf(stderr, "log_err: %s\n", str);
    return nullptr;
}



