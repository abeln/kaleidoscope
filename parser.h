
#ifndef KALEIDOSCOPE_PARSER_H
#define KALEIDOSCOPE_PARSER_H

#include "ast.h"

extern Token curr_tok;

Token get_next_tok();
std::unique_ptr<Expr> parse_expr();
std::unique_ptr<FunDef> parse_def();
std::unique_ptr<Proto> parse_extern();
std::unique_ptr<FunDef> parse_top_level();

#endif //KALEIDOSCOPE_PARSER_H
