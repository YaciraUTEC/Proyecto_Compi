#ifndef PROYECTO_COMPI_TYPE_VISITOR_H
#define PROYECTO_COMPI_TYPE_VISITOR_H

#include "../exp.h"
#include "imp_type.hh"

class TypeVisitor {
    virtual void visit(KotlinFile* kf) = 0;
    virtual void visit(Block* b) = 0;
    
    virtual void visit(FunctionDeclaration*e);

    virtual void visit(PropertyDeclaration* e) = 0;
    virtual void visit(StatementList* e) = 0;
    virtual void visit(AssignmentStatement* e) = 0;
    virtual void visit(PrintlnStatement* e) = 0;
    virtual void visit(ExpressionStatement* e) = 0;
    virtual void visit(WhileStatement* e) = 0;
    virtual void visit(DeclarationStatement* e) = 0;
    virtual void visit(ForStatement* e) = 0;
    virtual ImpValue visit(BinaryExpression* e) = 0;
    virtual ImpValue visit(StringLiteral* e) = 0;
    virtual ImpValue visit(LiteralExpression* e) = 0;
    virtual ImpValue visit(IdentifierExpression* e) = 0;
    virtual void visit(IfExpression* e) = 0;
    virtual ImpValue visit(JumpExpression* e) = 0;

};



#endif //PROYECTO_COMPI_TYPE_VISITOR_H
