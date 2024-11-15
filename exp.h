#ifndef EXP_H
#define EXP_H

#include <string>
#include <unordered_map>
#include <list>
#include "visitor.h"
using namespace std;

enum BinaryOperator {
    ADD, 
    SUB, 
    MUL, 
    DIV,
    LT,
    GT, 
    LE, 
    GE,
    EQ, 
    NE,
    AND, 
    OR,
    RANGE
};

// KotlinFile class to represent the entire file
class KotlinFile {
public:
    list<TopLevelObject> topLevelObjects;
    void accept(Visitor& visitor);
};

// TopLevelObject class to represent any top-level element in the Kotlin file
class TopLevelObject {
public:
    virtual ~TopLevelObject() = default;
    virtual void accept(Visitor& visitor) = 0;
};

// Declaration class to represent declarations
class Declaration : public TopLevelObject {
public:
    void accept(Visitor& visitor) override;
};


// ---
class Type {
public:
    string name;
    void accept(Visitor* visitor);
    Type(const string& name);
    ~Type();
};

class Parameter {
public:
    string identifier;
    Type* type;

    Parameter(const string& id, Type* type);
    ~Parameter();
};

class FunctionValueParameter {
public:
    Parameter* parameter;
    Expression* defaultValue;

    FunctionValueParameter(Parameter* param, Expression* defaultValue);
    ~FunctionValueParameter();
};


class FunctionBody {
public:
    virtual void accept(Visitor* visitor) = 0;
    virtual ~FunctionBody() = 0;
};

class Block : public FunctionBody {
public:
    list<Statement*> statements;

    void addStatement(Statement* stmt);
    void accept(Visitor* visitor);
    ~Block();
};

class ExpressionBody : public FunctionBody {
public:
    Expression* expression;

    ExpressionBody(Expression* expr);
    void accept(Visitor* visitor);
    ~ExpressionBody();
};

class FunctionDeclaration : public Declaration {
public:
    string identifier;
    list<FunctionValueParameter*> parameters;
    Type* returnType;
    FunctionBody* body;

    FunctionDeclaration(const string& id, const list<FunctionValueParameter*>& params, Type* returnType, FunctionBody* body);
    void accept(Visitor* visitor);
    ~FunctionDeclaration();
};

class VariableDeclaration {
public:
    string identifier;
    Type* type;

    VariableDeclaration(const string& id, Type* type);
    ~VariableDeclaration();
};

class ControlStructureBody {
public:
    virtual void accept(Visitor* visitor) = 0;
    virtual ~ControlStructureBody() = 0;
};

// Statement --

class Statement {
public:
    virtual void accept(Visitor* visitor) = 0;
    virtual ~Statement() = 0;
};

class DeclarationStatement : public Statement {
public:
    Declaration* declaration;

    DeclarationStatement(Declaration* decl);
    void accept(Visitor* visitor);
    ~DeclarationStatement();
};

class Assignment : public Statement {
public:
    string identifier;
    Expression* expression;

    Assignment(const string& id, Expression* expr);
    void accept(Visitor* visitor);
    ~Assignment();
};

class LoopStatement : public Statement {
public:
    virtual ~LoopStatement() = 0;
};

class ForStatement : public LoopStatement {
public:
    VariableDeclaration* variable;
    Expression* expression;
    ControlStructureBody* body;

    ForStatement(VariableDeclaration* var, Expression* expr, ControlStructureBody* body);
    void accept(Visitor* visitor);
    ~ForStatement();
};

class WhileStatement : public LoopStatement {
public:
    Expression* condition;
    ControlStructureBody* body;

    WhileStatement(Expression* cond, ControlStructureBody* body);
    void accept(Visitor* visitor);
    ~WhileStatement();
};

class ExpressionStatement : public Statement {
public:
    Expression* expression;

    ExpressionStatement(Expression* expr);
    void accept(Visitor* visitor);
    ~ExpressionStatement();
};

class StatementBody : public ControlStructureBody {
public:
    Statement* statement;

    StatementBody(Statement* stmt);
    void accept(Visitor* visitor);
    ~StatementBody();
};

// Expresiones
class Expression {
public:
    virtual void accept(Visitor* visitor) = 0;
    virtual ~Expression() = 0;
};

class BinaryExpression : public Expression {
public:
    Expression* left;
    Expression* right;
    BinaryOperator op;

    BinaryExpression(Expression* lhs, Expression* rhs, BinaryOperator op);
    void accept(Visitor* visitor);
    ~BinaryExpression();
};

class PrimaryExpression : public Expression {
public:
    virtual ~PrimaryExpression() = 0;
};

class IdentifierExpression : public PrimaryExpression {
public:
    string identifier;

    IdentifierExpression(const string& id);
    void accept(Visitor* visitor);
    ~IdentifierExpression();
};

enum LiteralType { BOOLEAN_LITERAL, INTEGER_LITERAL, CHARACTER_LITERAL, STRING_LITERAL };

class LiteralExpression : public PrimaryExpression {
public:
    LiteralType type;
    string value;

    LiteralExpression(LiteralType type, const string& value);
    void accept(Visitor* visitor);
    ~LiteralExpression();
};

class IfExpression : public PrimaryExpression {
public:
    Expression* condition;
    ControlStructureBody* thenBody;
    ControlStructureBody* elseBody;

    IfExpression(Expression* cond, ControlStructureBody* thenBody, ControlStructureBody* elseBody);
    void accept(Visitor* visitor);
    ~IfExpression();
};

class StringLiteral : public PrimaryExpression {
public:
    string value;

    StringLiteral(const string& value);
    void accept(Visitor* visitor);
    ~StringLiteral();
};

#endif // EXP_H