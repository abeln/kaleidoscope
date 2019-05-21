#include <string>
#include "ast.h"

std::string with_tab(std::string s, int tab) {
    std::string res;
    for (int i = 0; i < tab; i++) res += ' ';
    return res + s;
}

static std::string sp("  ");
static int spsz = sp.size();

std::string Expr::pprint0() {
    return this->pprint(0);
}

std::string Num::pprint(int tab) {
    return with_tab(std::to_string(this->val), tab);
}

std::string Var::pprint(int tab) {
    return with_tab(this->name, tab);
}

std::string BinaryExpr::pprint(int tab) {
    return with_tab(
            std::string("BinOp(\n") +
            with_tab(std::string(1, this->op) + ",\n", tab + spsz) +
            this->left->pprint(tab + spsz) + ",\n" +
            this->right->pprint(tab + spsz) + ")", tab);
}

std::string App::pprint(int tab) {
    std::string res = "App(" + this->callee;
    for (int i = 0; i < this->args.size(); i++) {
        res += ", " + args[i]->pprint(0);
    }
    res += ")";
    return with_tab(res, tab);
}

std::string IfExpr::pprint(int tab) {
    return with_tab(
            "If(\n" +
            this->cond_expr->pprint(tab + spsz) + ",\n" +
            this->then_expr->pprint(tab + spsz) + ",\n" +
            this->else_expr->pprint(tab + spsz) + ")", tab);
}

std::string Proto::pprint(int tab) {
    auto res = "Proto(" + this->name;
    for (auto arg : this->args) {
        res += ", " + arg;
    }
    res += ")";
    return with_tab(res, tab);
}

std::string FunDef::pprint(int tab) {
    auto res = std::string("FunDef(") + "\n" +
            sp + this->proto->pprint(0) + ",\n" +
            this->body->pprint(tab + spsz) + ")";
    return with_tab(res, tab);
}

// Visitor pattern

void Num::visit(ExprVisitor &visitor) {
    visitor.visit(*this);
}

void Var::visit(ExprVisitor &visitor) {
    visitor.visit(*this);
}

void App::visit(ExprVisitor &visitor) {
    visitor.visit(*this);
}
void BinaryExpr::visit(ExprVisitor &visitor) {
    visitor.visit(*this);
}

void IfExpr::visit(ExprVisitor &visitor) {
    visitor.visit(*this);
}
