#include <iostream>
#include "lexer.h"
#include "parser.h"
#include "codegen.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"

void handle_def(Ctx& ctx) {
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

void handle_extern(Ctx& ctx) {
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

void handle_top_level(Ctx& ctx) {
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

void init_ctx(Ctx& ctx) {
    ctx.module = llvm::make_unique<llvm::Module>("kaleidoscope module", ctx.context);
    ctx.pass_manager = llvm::make_unique<llvm::legacy::FunctionPassManager>(ctx.module.get());

    // Add used optimizations
    ctx.pass_manager->add(llvm::createInstructionCombiningPass());
    ctx.pass_manager->add(llvm::createReassociatePass());
    ctx.pass_manager->add(llvm::createGVNPass());
    ctx.pass_manager->add(llvm::createCFGSimplificationPass());
    ctx.pass_manager->doInitialization();
}

int main() {
    Ctx ctx;
    init_ctx(ctx);
    while (true) {
        fprintf(stderr, "#> ");
        get_next_tok();
        switch (curr_tok) {
            case Token::tok_eof:
                return 0;
            case Token::tok_def:
                handle_def(ctx);
                break;
            case Token::tok_extern:
                handle_extern(ctx);
                break;
            default:
                if (curr_tok == Token::tok_unknown && unknown_val == ';') {
                    get_next_tok();
                    break;
                }
                handle_top_level(ctx);
                break;
        }
    }
    return 0;
}