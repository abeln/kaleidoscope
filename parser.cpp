#include "lexer.h"
#include "ast.h"
#include "llvm/ADT/STLExtras.h"

static Token curr_tok;

static Token get_next_tok() {
    curr_tok = get_token();
    return curr_tok;
}

static bool is_curr_tok(char c) {
    return curr_tok == Token::tok_unknown && unknown_val == c;
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

// Parsing functions

std::unique_ptr<Expr> parse_expr() {
    return nullptr;
}

std::unique_ptr<Expr> parse_num() {
    auto res = llvm::make_unique<Num>(num_val);
    get_next_tok();
    return std::move(res);
}

std::unique_ptr<Expr> parse_parens() {
    get_next_tok(); // eat '('
    auto expr = parse_expr();
    if (!expr) return nullptr;
    if (!is_curr_tok(')')) return log_err_expr("expected ')'");
    get_next_tok(); // eat ')'
    return expr;
}

std::unique_ptr<Expr> parse_id() {
    std::string id = id_val;
    get_next_tok();
    if (!is_curr_tok('(')) return llvm::make_unique<Var>(id); // simple id

    // we're parsing a function
    get_next_tok();
    std::vector<std::unique_ptr<Expr>> args;
    if (!is_curr_tok(')')) {
        while (true) {
            auto arg = parse_expr();
            if (!arg) return nullptr;
            args.push_back(std::move(arg));
            if (is_curr_tok(')')) break;
            if (!is_curr_tok(',')) return log_err_expr("expected ',' while parsing argument list");
            get_next_tok();
        }
    }

    get_next_tok(); // eat ')'
    return llvm::make_unique<App>(id, std::move(args));
}

std::unique_ptr<Expr> parse_primary() {
    switch (curr_tok) {
        case Token::tok_id:
            return parse_id();
        case Token::tok_num:
            return parse_num();
        default:
            if (is_curr_tok('(')) return parse_parens();
            return log_err_expr("unknown token when parsing an expression");
    }
}

