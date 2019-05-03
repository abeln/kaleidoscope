#include <iostream>
#include "lexer.h"
#include "parser.h"

void handle_def() {
    auto def = parse_def();
    if (!def) {
        get_next_tok();
        return;
    }
    fprintf(stderr, "%s\n", def->pprint(0).c_str());
}

void handle_extern() {
    auto ext = parse_extern();
    if (!ext) {
        get_next_tok();
        return;
    }
    fprintf(stderr, "%s\n", ext->pprint(0).c_str());
}

void handle_top_level() {
    auto def = parse_top_level();
    if (!def) {
        get_next_tok();
        return;
    }
    fprintf(stderr, "%s\n", def->pprint(0).c_str());
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
                handle_extern();
                break;
            default:
                if (curr_tok == Token::tok_unknown && unknown_val == ';') {
                    get_next_tok();
                    break;
                }
                handle_top_level();
                break;
        }
    }

    return 0;
}