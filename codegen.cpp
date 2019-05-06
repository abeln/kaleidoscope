
#include "codegen.h"

llvm::Value* log_err(const char* msg) {
    fprintf(stderr, "Codegen error: %s\n", msg);
    return nullptr;
}

void CGVisitor::visit(Num &expr) {
    res_value = llvm::ConstantFP::get(context, llvm::APFloat(expr.val));
}

void CGVisitor::visit(Var &expr) {
    res_value = sym_table[expr.name];
    if (!res_value) log_err(("unknown variable: " + expr.name).c_str());
}

void CGVisitor::visit(App &expr) {
    auto fn = module->getFunction(expr.callee);
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
    res_value = builder.CreateCall(fn, argvs, "calltmp");
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
            res_value = builder.CreateFAdd(left_res, right_res, "addtmp");
            break;
        case '-':
            res_value = builder.CreateFSub(left_res, right_res, "subtmp");
            break;
        case '*':
            res_value = builder.CreateFMul(left_res, right_res, "multmp");
            break;
        case '<':
            cmp_tmp = builder.CreateFCmpULT(left_res, right_res, "lttmp");
            res_value = builder.CreateUIToFP(cmp_tmp, llvm::Type::getDoubleTy(context), "bool-to-fp-tmp");
            break;
        default:
            res_value = log_err((std::string("unknown binary op: ") + expr.op).c_str());
            break;
    }

}


