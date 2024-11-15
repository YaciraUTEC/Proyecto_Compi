// exp.cpp
#include "exp.h"
#include "visitor.h"

using namespace std;

// Clase Type
Type::Type(const string& name) : name(name) {}
Type::~Type() {}

void Type::accept(Visitor* visitor) {
    visitor->visit(this);
}

// Clase Parameter
Parameter::Parameter(const string& id, Type* type)
    : identifier(id), type(type) {}

Parameter::~Parameter() {
    delete type;
}

// Clase FunctionValueParameter
FunctionValueParameter::FunctionValueParameter(Parameter* param, Expression* defaultValue)
    : parameter(param), defaultValue(defaultValue) {}

FunctionValueParameter::~FunctionValueParameter() {
    delete parameter;
    delete defaultValue;
}

// Clase FunctionBody
FunctionBody::~FunctionBody() {}

// Clase Block
Block::~Block() {
    for (Statement* stmt : statements) {
        delete stmt;
    }
}

void Block::addStatement(Statement* stmt) {
    statements.push_back(stmt);
}

void Block::accept(Visitor* visitor) {
    visitor->visit(this);
}

// Clase ExpressionBody
ExpressionBody::ExpressionBody(Expression* expr)
    : expression(expr) {}

ExpressionBody::~ExpressionBody() {
    delete expression;
}

void ExpressionBody::accept(Visitor* visitor) {
    visitor->visit(this);
}

// Clase FunctionDeclaration
FunctionDeclaration::FunctionDeclaration(const string& id, const list<FunctionValueParameter*>& params, Type* returnType, FunctionBody* body)
    : identifier(id), parameters(params), returnType(returnType), body(body) {}

FunctionDeclaration::~FunctionDeclaration() {
    for (FunctionValueParameter* param : parameters) {
        delete param;
    }
    delete returnType;
    delete body;
}

void FunctionDeclaration::accept(Visitor* visitor) {
    visitor->visit(this);
}

// Clase VariableDeclaration
VariableDeclaration::VariableDeclaration(const string& id, Type* type)
    : identifier(id), type(type) {}

VariableDeclaration::~VariableDeclaration() {
    delete type;
}

// Clase ControlStructureBody
ControlStructureBody::~ControlStructureBody() {}

// Clase StatementBody
StatementBody::StatementBody(Statement* stmt)
    : statement(stmt) {}

StatementBody::~StatementBody() {
    delete statement;
}

void StatementBody::accept(Visitor* visitor) {
    visitor->visit(this);
}

// Clase Statement
Statement::~Statement() {}

// Clase DeclarationStatement
DeclarationStatement::DeclarationStatement(Declaration* decl)
    : declaration(decl) {}

DeclarationStatement::~DeclarationStatement() {
    delete declaration;
}

void DeclarationStatement::accept(Visitor* visitor) {
    visitor->visit(this);
}

// Clase Assignment
Assignment::Assignment(const string& id, Expression* expr)
    : identifier(id), expression(expr) {}

Assignment::~Assignment() {
    delete expression;
}

void Assignment::accept(Visitor* visitor) {
    visitor->visit(this);
}

// Clase LoopStatement
LoopStatement::~LoopStatement() {}

// Clase ForStatement
ForStatement::ForStatement(VariableDeclaration* var, Expression* expr, ControlStructureBody* body)
    : variable(var), expression(expr), body(body) {}

ForStatement::~ForStatement() {
    delete variable;
    delete expression;
    delete body;
}

void ForStatement::accept(Visitor* visitor) {
    visitor->visit(this);
}

// Clase WhileStatement
WhileStatement::WhileStatement(Expression* cond, ControlStructureBody* body)
    : condition(cond), body(body) {}

WhileStatement::~WhileStatement() {
    delete condition;
    delete body;
}

void WhileStatement::accept(Visitor* visitor) {
    visitor->visit(this);
}

// Clase ExpressionStatement
ExpressionStatement::ExpressionStatement(Expression* expr)
    : expression(expr) {}

ExpressionStatement::~ExpressionStatement() {
    delete expression;
}

void ExpressionStatement::accept(Visitor* visitor) {
    visitor->visit(this);
}

// Clase Expression
Expression::~Expression() {}

// Clase BinaryExpression
BinaryExpression::BinaryExpression(Expression* lhs, Expression* rhs, BinaryOperator op)
    : left(lhs), right(rhs), op(op) {}

BinaryExpression::~BinaryExpression() {
    delete left;
    delete right;
}

void BinaryExpression::accept(Visitor* visitor) {
    visitor->visit(this);
}

// Clase PrimaryExpression
PrimaryExpression::~PrimaryExpression() {}

// Clase IdentifierExpression
IdentifierExpression::IdentifierExpression(const string& id)
    : identifier(id) {}

IdentifierExpression::~IdentifierExpression() {}

void IdentifierExpression::accept(Visitor* visitor) {
    visitor->visit(this);
}

// Clase LiteralExpression
LiteralExpression::LiteralExpression(LiteralType type, const string& value)
    : type(type), value(value) {}

LiteralExpression::~LiteralExpression() {}

void LiteralExpression::accept(Visitor* visitor) {
    visitor->visit(this);
}

// Clase IfExpression
IfExpression::IfExpression(Expression* cond, ControlStructureBody* thenBody, ControlStructureBody* elseBody)
    : condition(cond), thenBody(thenBody), elseBody(elseBody) {}

IfExpression::~IfExpression() {
    delete condition;
    delete thenBody;
    delete elseBody;
}

void IfExpression::accept(Visitor* visitor) {
    visitor->visit(this);
}

// Clase StringLiteral
StringLiteral::StringLiteral(const string& value)
    : value(value) {}

StringLiteral::~StringLiteral() {}

void StringLiteral::accept(Visitor* visitor) {
    visitor->visit(this);
}

// Clase KotlinFile
void KotlinFile::accept(Visitor& visitor) {
    visitor.visit(this);
}

KotlinFile::~KotlinFile() {}

// Clase TopLevelObject
TopLevelObject::~TopLevelObject() {}

// Clase Declaration
void Declaration::accept(Visitor& visitor) {
    visitor.visit(this);
}

// Clase FunctionBody
FunctionBody::~FunctionBody() {}

// Clase Visitor (dummy implementations)
Visitor::~Visitor() {}