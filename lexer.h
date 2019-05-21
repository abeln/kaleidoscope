
#ifndef KALEIDOSCOPE_LEXER_H
#define KALEIDOSCOPE_LEXER_H

#include <string>

enum class Token {
    tok_eof,

    // commands
    tok_def,
    tok_extern,

    // primary
    tok_id,
    tok_num,
    tok_if,
    tok_then,
    tok_else,
    tok_unknown
};

extern std::string id_val; // Filled in if tok_id lexed
extern char unknown_val;   // Filled in if tok_unknown lexed
extern double num_val;     // Filled in if tok_num lexed

Token get_token();

#endif //KALEIDOSCOPE_LEXER_H
