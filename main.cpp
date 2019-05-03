#include <iostream>
#include "lexer.h"
#include "parser.h"

void handle_def() {
    auto def = parse_def();
    if (!def) get_next_tok();
    fprintf(stderr, "parsed function def\n");
}

int main() {
    while (true) {
        fprintf(stderr, "#> ");
        get_next_tok();
        switch (curr_tok) {
            case Token::tok_eof:
                return 0;
            case Token::tok_def:
                handle_def();
                break;
            case Token::tok_extern:
                break;
            default:
                fprintf(stderr, "unknown token\n");
                break;
        }
    }

    return 0;
}