
#ifndef KALEIDOSCOPE_CODEGEN_H
#define KALEIDOSCOPE_CODEGEN_H

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/LegacyPassManager.h"

#include "ast.h"

struct Ctx {
    llvm::LLVMContext context;
    std::unique_ptr<llvm::Module> module;
    std::unique_ptr<llvm::legacy::FunctionPassManager> pass_manager;
    llvm::IRBuilder<> builder;
    std::map<std::string, llvm::Value*> sym_table;

    Ctx(): builder(context) {}
};

class CGVisitor : public ExprVisitor {
private:
    Ctx& ctx;

public:
    llvm::Value* res_value; // result of code generation

    explicit CGVisitor(Ctx& ctx);

    void visit(Num &expr) override;

    void visit(Var &expr) override;

    void visit(App &expr) override;

    void visit(BinaryExpr &expr) override;
};

llvm::Function* codegen(Ctx& ctx, const Proto& proto);

llvm::Function* codegen(Ctx& ctx, const FunDef& fun);

llvm::Value* codegen(Ctx& ctx, Expr& expr);

#endif //KALEIDOSCOPE_CODEGEN_H
