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
            sp + std::to_string(this->op) + ",\n" +
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