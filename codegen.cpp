
#include "codegen.h"

llvm::Value* log_err(const char* msg) {
    fprintf(stderr, "Codegen error: %s\n", msg);
    return nullptr;
}

void CGVisitor::visit(Num &expr) {
    res_value = llvm::ConstantFP::get(ctx.context, llvm::APFloat(expr.val));
}

void CGVisitor::visit(Var &expr) {
    res_value = ctx.sym_table[expr.name];
    if (!res_value) log_err(("unknown variable: " + expr.name).c_str());
}

void CGVisitor::visit(App &expr) {
    auto fn = ctx.module->getFunction(expr.callee);
    if (!fn) {
        res_value = nullptr;
        return;
    }
    if (fn->arg_size() != expr.args.size()) {
        res_value = log_err("invalid function call"); // TODO: improve error message
        return;
    }
    std::vector<llvm::Value*> argvs;
    for (auto & arg : expr.args) {
        arg->visit(*this);
        if (!res_value) return;
        argvs.push_back(res_value);
    }
    res_value = ctx.builder.CreateCall(fn, argvs, "calltmp");
}

void CGVisitor::visit(BinaryExpr &expr) {
    expr.left->visit(*this);
    auto left_res = res_value;
    expr.right->visit(*this);
    auto right_res = res_value;
    if (!left_res || !right_res) {
        res_value = nullptr;
        return;
    }
    llvm::Value* cmp_tmp;
    switch (expr.op) {
        case '+':
            res_value = ctx.builder.CreateFAdd(left_res, right_res, "addtmp");
            break;
        case '-':
            res_value = ctx.builder.CreateFSub(left_res, right_res, "subtmp");
            break;
        case '*':
            res_value = ctx.builder.CreateFMul(left_res, right_res, "multmp");
            break;
        case '<':
            cmp_tmp = ctx.builder.CreateFCmpULT(left_res, right_res, "lttmp");
            res_value = ctx.builder.CreateUIToFP(cmp_tmp, llvm::Type::getDoubleTy(ctx.context), "bool-to-fp-tmp");
            break;
        default:
            res_value = log_err((std::string("unknown binary op: ") + expr.op).c_str());
            break;
    }

}

CGVisitor::CGVisitor(Ctx& ctx): ctx(ctx), res_value(nullptr) {}

llvm::Function *codegen(Ctx& ctx, const Proto &proto) {
    // Create argument types
    std::vector<llvm::Type*> args(proto.args.size(), llvm::Type::getDoubleTy(ctx.context));

    // Create function type
    llvm::FunctionType* ft = llvm::FunctionType::get(llvm::Type::getDoubleTy(ctx.context), args, false);

    // Create the function itself
    llvm::Function* fun = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, proto.name, ctx.module.get());

    // Set the argument names
    int idx = 0;
    for (auto &arg : fun->args()) {
        arg.setName(proto.args[idx++]);
    }

    return fun;
}

llvm::Function *codegen(Ctx &ctx, const FunDef &fun_def) {
    llvm::Function *fun = ctx.module->getFunction(fun_def.proto->name);
    if (!fun) {
        fun = codegen(ctx, *fun_def.proto);
    }
    if (!fun) return nullptr;
    if (!fun->empty()) {
        log_err("tried to re-defined function");
        return nullptr;
    }

    llvm::BasicBlock *bb = llvm::BasicBlock::Create(ctx.context, "entry", fun);
    ctx.builder.SetInsertPoint(bb);

    ctx.sym_table.clear();
    for (auto &arg: fun->args()) {
        ctx.sym_table[arg.getName()] = &arg;
    }

    auto *res = codegen(ctx, *fun_def.body);
    if (!res) {
        fun->eraseFromParent();
        return nullptr;
    }

    ctx.builder.CreateRet(res);
    llvm::verifyFunction(*fun);
    ctx.pass_manager->run(*fun);
    return fun;
}

llvm::Value *codegen(Ctx &ctx, Expr &expr) {
    CGVisitor visitor(ctx);
    expr.visit(visitor);
    return visitor.res_value;
}
