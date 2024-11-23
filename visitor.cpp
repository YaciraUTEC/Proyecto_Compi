#include <iostream>
#include "exp.h"
#include "visitor.h"
#include <unordered_map>
using namespace std;

///////////////////////////////////////////////////////////////////////////////////
int BinaryExpression::accept(Visitor* e) {
    e->visit(this);
    return 0;
}

int IdentifierExpression::accept(Visitor* v) {
    v->visit(this);
    return 0;
}

int FunctionCallExpression::accept(Visitor* v) {
    v->visit(this);
    return 0;
}

int IfExpression::accept(Visitor* v) {
    v->visit(this);
    return 0;
}

int StringLiteral::accept(Visitor* v) {
    v->visit(this);
    return 0;
}

int JumpExpression::accept(Visitor* v) {
    v->visit(this);
    return 0;
}

int LiteralExpression::accept(Visitor* v) {
    v->visit(this);
    return 0;
}

void AssignmentStatement::accept(Visitor* v) {
    return v->visit(this);
}

void PrintlnStatement::accept(Visitor* v) {
    return v->visit(this);
}

void ExpressionStatement::accept(Visitor* v) {
    return v->visit(this);
}

void WhileStatement::accept(Visitor* v) {
    return v->visit(this);
}

void DeclarationStatement::accept(Visitor* v) {
    return v->visit(this);
}

void ForStatement::accept(Visitor* v) {
    return v->visit(this);
}

void StatementList::accept(Visitor* v) {
    return v->visit(this);
}

void FunctionDeclaration::accept(Visitor* v) {
    return v->visit(this);
}

void PropertyDeclaration::accept(Visitor* v) {
    return v->visit(this);
}

void Block::accept(Visitor* v) {
    return v->visit(this);
}

void KotlinFile::accept(Visitor* v) {
    return v->visit(this);
}


