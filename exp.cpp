#include "exp.h"
#include "visitor.h"

// Implementaciones base
Declaration::~Declaration() {}
Expression::~Expression() {}
Statement::~Statement() {}

// Type
Type::Type( string name) : name(name) {}
Type::~Type() {}

void Type::accept(Visitor* visitor) {
    visitor->visit(this);
}

// KotlinFile
void KotlinFile::add(Declaration* declaration) {
    decl.push_back(declaration);
}

void KotlinFile::print() {
    for(auto d : decl) {
        d->print();
    }
}

// FunctionDeclaration
FunctionDeclaration::FunctionDeclaration(string id, list<FunctionValueParameter*> params, 
                                       Type* rtype, Block* body)
    : identifier(id), parameters(params), returnType(rtype), fbody(body) {}

FunctionDeclaration::~FunctionDeclaration() {
    delete returnType;
    delete fbody;
    for(auto param : parameters) {
        delete param;
    }
}

void FunctionDeclaration::print() {
    // Implementación de impresión
}

int FunctionDeclaration::eval() {
    // Implementación de evaluación
    return 0;
}

// PropertyDeclaration
PropertyDeclaration::PropertyDeclaration(string ptype, VariableDeclaration* var, Expression* expr)
    : ptype(ptype), variable(var), expression(expr) {}

PropertyDeclaration::~PropertyDeclaration() {
    delete variable;
    delete expression;
}

// Parameter
Parameter::Parameter(string id, Type* type) : identifier(id), type(type) {}
Parameter::~Parameter() {
    delete type;
}

// FunctionValueParameter
FunctionValueParameter::FunctionValueParameter(Parameter* param, Expression* defaultValue)
    : parameter(param), defaultValue(defaultValue) {}

FunctionValueParameter::~FunctionValueParameter() {
    delete parameter;
    delete defaultValue;
}

// VariableDeclaration
VariableDeclaration::VariableDeclaration( string id, Type* type) 
    : identifier(id), type(type) {}
VariableDeclaration::~VariableDeclaration() {
    delete type;
}


// Block
Block::Block() {}

Block::~Block() {
    for(auto stmt : statements) {
        delete stmt;
    }
}

void Block::add(Statement* stmt) {
    statements.push_back(stmt);
}

void Block::accept(Visitor* visitor) {
    visitor->visit(this);
}

void Block::print() {
    // ¡
    for(auto stmt : statements) {
        stmt->print();
    }
}

// Statements
DeclarationStatement::DeclarationStatement(Declaration* decl) : declaration(decl) {}

DeclarationStatement::~DeclarationStatement() {
    delete declaration;
}

void DeclarationStatement::print() {
    declaration->print();
}

int DeclarationStatement::eval() {
    return declaration->eval();
}

AssignmentStatement::AssignmentStatement(string id, Expression* expr) 
    : identifier(id), expression(expr) {}

AssignmentStatement::~AssignmentStatement() {
    delete expression;
}

void AssignmentStatement::print() {
    // Implementación de impresión
}

int AssignmentStatement::eval() {
    // Implementación de evaluación
    return 0;
}

// Expressions
BinaryExpression::BinaryExpression(Expression* lhs, Expression* rhs, BinaryOperator op)
    : left(lhs), right(rhs), op(op) {}

BinaryExpression::~BinaryExpression() {
    delete left;
    delete right;
}

void BinaryExpression::accept(Visitor* visitor) {
    visitor->visit(this);
}

IdentifierExpression::IdentifierExpression( string id) : identifier(id) {}

IdentifierExpression::~IdentifierExpression() {}

void IdentifierExpression::accept(Visitor* visitor) {
    visitor->visit(this);
}

// LiteralExpression
LiteralExpression::LiteralExpression(LiteralType type,  string value)
    : type(type), value(value) {}

LiteralExpression::~LiteralExpression() {}

void LiteralExpression::accept(Visitor* visitor) {
    visitor->visit(this);
}

// IfExpression
IfExpression::IfExpression(Expression* cond, Block* thenBody, Block* elseBody)
    : condition(cond), thenBody(thenBody), elseBody(elseBody) {}

IfExpression::~IfExpression() {
    delete condition;
    delete thenBody;
    delete elseBody;
}

void IfExpression::accept(Visitor* visitor) {
    visitor->visit(this);
}

// StringLiteral
StringLiteral::StringLiteral( string value) : value(value) {}

StringLiteral::~StringLiteral() {}

// void StringLiteral::accept(Visitor* visitor) {
//     visitor->visit(this);
// }
