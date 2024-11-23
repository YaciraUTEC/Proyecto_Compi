#ifndef VISITOR_H
#define VISITOR_H
#include "exp.h"

#include <list>

class Visitor {
public:
    virtual void visit(KotlinFile* kf);
    virtual void visit(Block* b);
    virtual void visit(FunctionDeclaration*fd);
    virtual void visit(PropertyDeclaration* e);
    virtual void visit(StatementList* e);
    virtual void visit(AssignmentStatement* e);
    virtual void visit(PrintlnStatement* e);
    virtual void visit(ExpressionStatement* e);
    virtual void visit(WhileStatement* e);
    virtual void visit(DeclarationStatement* e);
    virtual void visit(ForStatement* e);
    virtual int visit(BinaryExpression* e);
    virtual int visit(StringLiteral* e);
    virtual int visit(LiteralExpression* e);
    virtual int visit(IdentifierExpression* e);
    virtual int visit(FunctionCallExpression* e);
    virtual int visit(IfExpression* e);
    virtual int visit(JumpExpression* e);
};

// class PrintVisitor : public Visitor {
// public:
//     void imprimir(Program* program);
//     int visit(IFExp* exp) override;
//     int visit(BinaryExp* exp) override;
//     int visit(NumberExp* exp) override;
//     int visit(BoolExp* exp) override;
//     int visit(IdentifierExp* exp) override;
//     int visit(FCallExp* exp) override;
//     void visit(AssignStatement* stm) override;
//     void visit(PrintStatement* stm) override;
//     void visit(IfStatement* stm) override;
//     void visit(WhileStatement* stm) override;
//     void visit(ForStatement* stm) override;
//     void visit(VarDec* stm) override;
//     void visit(VarDecList* stm) override;
//     void visit(StatementList* stm) override;
//     void visit(Body* b) override;
//     void visit(Program* p) override;
//     void visit(FunDec* f) override;
//     void visit(FunDecList* f) override;
//     void visit(ReturnStatement* r) override;
//     void visit(FCallStatement* f) override;

// private:
//     int indentLevel = 0;
//     void increaseIndent() { indentLevel++; }
//     void decreaseIndent() { indentLevel--; }
//     void printIndent();
// };




#endif // VISITOR_H