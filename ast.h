#include <string>
#include <vector>

#ifndef KALEIDOSCOPE_AST_H
#define KALEIDOSCOPE_AST_H

class ExprVisitor;

class Expr {
public:
    virtual ~Expr() = default;
    std::string pprint0();
    virtual std::string pprint(int tab) = 0;
    virtual void visit(ExprVisitor& visitor) = 0;
};

class Num : public Expr {
public:
    double val;
    explicit Num(double val): val(val) {}
    virtual std::string pprint(int tab) override;
    void visit(ExprVisitor& visitor) override;
};

class Var : public Expr {
public:
    std::string name;
    Var(std::string name): name(std::move(name)) {}
    virtual std::string pprint(int tab) override;
    void visit(ExprVisitor& visitor) override;
};

class BinaryExpr : public Expr {
public:
    char op;
    std::unique_ptr<Expr> left, right;

    BinaryExpr(char op, std::unique_ptr<Expr> left, std::unique_ptr<Expr> right) :
        op(op), left(std::move(left)), right(std::move(right))
    {}
    virtual std::string pprint(int tab) override;
    void visit(ExprVisitor& visitor) override;
};

class App : public Expr {
public:
    std::string callee;
    std::vector<std::unique_ptr<Expr>> args;

    App(std::string callee, std::vector<std::unique_ptr<Expr>> args) :
        callee(std::move(callee)), args(std::move(args))
    {}
    virtual std::string pprint(int tab) override;
    void visit(ExprVisitor& visitor) override;
};

// The classes below are _not_ expressions.

// The prototype of a function includes its name and argument names (and number).
class Proto {
public:
    std::string name;
    std::vector<std::string> args;

    Proto(std::string name, std::vector<std::string> args): name(std::move(name)), args(std::move(args)) {}

    virtual std::string pprint(int tab);
};

class FunDef {
public:
    std::unique_ptr<Proto> proto;
    std::unique_ptr<Expr> body;

    FunDef(std::unique_ptr<Proto> proto, std::unique_ptr<Expr> body): proto(std::move(proto)), body(std::move(body)) {}
    virtual std::string pprint(int tab);
};

class ExprVisitor {
public:
    virtual void visit(Num& expr) = 0;
    virtual void visit(Var& expr) = 0;
    virtual void visit(App& expr) = 0;
    virtual void visit(BinaryExpr& expr) = 0;
};

#endif //KALEIDOSCOPE_AST_H



