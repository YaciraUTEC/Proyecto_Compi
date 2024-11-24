#ifndef VISITOR_H
#define VISITOR_H
#include "exp.h"

#include <list>

class KotlinFile;
class Block;
class FunctionDeclaration;
class PropertyDeclaration;
class StatementList;
class AssignmentStatement;
class PrintlnStatement;
class ExpressionStatement;
class WhileStatement;
class DeclarationStatement;
class ForStatement;
class BinaryExpression;
class StringLiteral;
class LiteralExpression;
class IdentifierExpression;
class FunctionCallExpression;
class IfExpression;
class JumpExpression;

class Visitor {
public:
    virtual void visit(KotlinFile* kf) = 0;
    virtual void visit(Block* b) = 0;
    virtual void visit(FunctionDeclaration*fd) = 0;
    virtual void visit(PropertyDeclaration* e) = 0;
    virtual void visit(StatementList* e) = 0;
    virtual void visit(AssignmentStatement* e) = 0;
    virtual void visit(PrintlnStatement* e) = 0;
    virtual void visit(ExpressionStatement* e) = 0;
    virtual void visit(WhileStatement* e) = 0;
    virtual void visit(DeclarationStatement* e) = 0;
    virtual void visit(ForStatement* e) = 0;
    virtual int visit(BinaryExpression* e) = 0;
    virtual int visit(StringLiteral* e) = 0;
    virtual int visit(LiteralExpression* e) = 0;
    virtual int visit(IdentifierExpression* e) = 0;
    virtual int visit(FunctionCallExpression* e) = 0;
    virtual int visit(IfExpression* e) = 0;
    virtual int visit(JumpExpression* e) = 0;
};


#endif // VISITOR_H