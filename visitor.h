#ifndef VISITOR_H
#define VISITOR_H
#include "exp.h"

#include <list>

class Visitor {
public:
    virtual void visit(KotlinFile* file) = 0;
    virtual void visit(FunctionDeclaration* funcDecl) = 0;
    virtual void visit(FunctionValueParameter* funcParam) = 0;
    virtual void visit(Parameter* param) = 0;
    virtual void visit(Block* block) = 0;
    virtual void visit(ExpressionBody* exprBody) = 0;
    virtual void visit(DeclarationStatement* declStmt) = 0;
    virtual void visit(Assignment* assignment) = 0;
    virtual void visit(ForStatement* forStmt) = 0;
    virtual void visit(WhileStatement* whileStmt) = 0;
    virtual void visit(ExpressionStatement* exprStmt) = 0;
    virtual void visit(BinaryExpression* binExpr) = 0;
    virtual void visit(IdentifierExpression* idExpr) = 0;
    virtual void visit(LiteralExpression* litExpr) = 0;
    virtual void visit(IfExpression* ifExpr) = 0;
    virtual void visit(Type* type) = 0;
    virtual void visit(StatementBody* stmtBody) = 0;
    virtual void visit(StringLiteral* strLit) = 0;
    virtual void visit(Declaration* decl) = 0;
    virtual ~Visitor() = 0;
};




#endif // VISITOR_H