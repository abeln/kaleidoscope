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
    double val;

public:
    explicit Num(double val): val(val) {}
    virtual std::string pprint(int tab) override;
    void visit(ExprVisitor& visitor) override;
};

class Var : public Expr {
    std::string name;

public:
    Var(std::string name): name(std::move(name)) {}
    virtual std::string pprint(int tab) override;
    void visit(ExprVisitor& visitor) override;
};

class BinaryExpr : public Expr {
    char op;
    std::unique_ptr<Expr> left, right;

public:
    BinaryExpr(char op, std::unique_ptr<Expr> left, std::unique_ptr<Expr> right) :
        op(op), left(std::move(left)), right(std::move(right))
    {}
    virtual std::string pprint(int tab) override;
    void visit(ExprVisitor& visitor) override;
};

class App : public Expr {
    std::string callee;
    std::vector<std::unique_ptr<Expr>> args;

public:
    App(std::string callee, std::vector<std::unique_ptr<Expr>> args) :
        callee(std::move(callee)), args(std::move(args))
    {}
    virtual std::string pprint(int tab) override;
    void visit(ExprVisitor& visitor) override;
};

// The classes below are _not_ expressions.

// The prototype of a function includes its name and argument names (and number).
class Proto {
    std::string name;
    std::vector<std::string> args;

public:

    Proto(std::string name, std::vector<std::string> args): name(std::move(name)), args(std::move(args)) {}

    const std::string& get_name() const { return name; }
    virtual std::string pprint(int tab);
};

class FunDef {
    std::unique_ptr<Proto> proto;
    std::unique_ptr<Expr> body;

public:
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



