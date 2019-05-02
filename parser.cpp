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

std::unique_ptr<Expr> parse_expr();

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

// primary ::=
//   id
//   num
//   ( expr )
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

// The higher the priority the stronger the operator binds.
// Invalid operator returns -1.
static int binop_pri(char op) {
    switch (op) {
        case '<': return 10;
        case '+': return 20;
        case '-': return 20;
        case '*': return 30;
        default: return -1;
    }
}

static bool is_curr_tok_op() {
    return curr_tok == Token::tok_unknown && binop_pri(unknown_val) != -1;
}

std::unique_ptr<BinaryExpr> make_binexp(char op, std::unique_ptr<Expr> lhs, std::unique_ptr<Expr> rhs) {
    return llvm::make_unique<BinaryExpr>(op, std::move(lhs), std::move(rhs));
}

// binop_rhs ::= (bin_op primary)*
std::unique_ptr<Expr> parse_binop_rhs(int previous_pri, std::unique_ptr<Expr> lhs) {
    if (!is_curr_tok_op()) return lhs; // we've run out of operators
    char op = unknown_val;
    int op_pri = binop_pri(op);
    if (op_pri <= previous_pri) return lhs;
    get_next_tok(); // eat operator
    auto rhs = parse_primary();
    if (!rhs) return nullptr;
    if (!is_curr_tok_op()) return make_binexp(op, std::move(lhs), std::move(rhs));
    char next_op = unknown_val;
    int next_pri = binop_pri(next_op);
    if (next_pri > op_pri) {
        rhs = parse_binop_rhs(op_pri, std::move(rhs));
    }
    return parse_binop_rhs(0, make_binexp(op, std::move(lhs), std::move(rhs)));
}

// expr ::= primary binop_rhs
std::unique_ptr<Expr> parse_expr() {
    auto lhs = parse_primary();
    if (!lhs) return nullptr;
    return parse_binop_rhs(0, std::move(lhs));
}



