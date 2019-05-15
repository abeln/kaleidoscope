#include <iostream>
#include "lexer.h"
#include "parser.h"
#include "codegen.h"

static Ctx ctx;

void handle_def() {
    auto def = parse_def();
    if (!def) {
        get_next_tok();
        return;
    }
    fprintf(stderr, "%s\n", def->pprint(0).c_str());
    auto *def_ir = codegen(ctx, *def);
    def_ir->print(llvm::errs());
    fprintf(stderr, "\n");
}

void handle_extern() {
    auto ext = parse_extern();
    if (!ext) {
        get_next_tok();
        return;
    }
    fprintf(stderr, "%s\n", ext->pprint(0).c_str());
    auto *ext_ir = codegen(ctx, *ext);
    ext_ir->print(llvm::errs());
    fprintf(stderr, "\n");
}

void handle_top_level() {
    auto def = parse_top_level();
    if (!def) {
        get_next_tok();
        return;
    }
    fprintf(stderr, "%s\n", def->pprint(0).c_str());
    auto *fn_ir = codegen(ctx, *def);
    fn_ir->print(llvm::errs());
    fprintf(stderr, "\n");
}

void init_ctx() {
    ctx.module = llvm::make_unique<llvm::Module>("kaleidoscope module", ctx.context);
}

int main() {
    init_ctx();
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