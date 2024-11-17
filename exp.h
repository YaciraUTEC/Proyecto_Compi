#ifndef EXP_H
#define EXP_H

#include <string>
#include <unordered_map>
#include <list>
#include "visitor.h"
#include <iostream>

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

class FunctionDeclaration;
class Expression;

class Type {
public:
    string name;
    void accept(Visitor* visitor);
    Type( string name);
    ~Type();
};

// KOTLINFILE
class KotlinFile {
public:
    list<Declaration*> decl;
    void add(Declaration* decl);
    void print();
    // int eval();
};

// DECLARATIONS

class Declaration {
public:
    virtual void print() = 0;
    virtual int eval() = 0;
    virtual ~Declaration() = 0;
};

// FUN

class FunctionDeclaration : public Declaration {
public:
    string identifier;
    list<FunctionValueParameter*> parameters;
    Type* returnType;
    Block* fbody;

    FunctionDeclaration(string id, list<FunctionValueParameter*> params, Type* rtype, Block* fbody);
    ~FunctionDeclaration();
    void print();
    int eval();
};

class PropertyDeclaration : public Declaration {
public:
    string ptype; // var o val
    VariableDeclaration* variable;
    Expression* expression;

    PropertyDeclaration(string ptype, VariableDeclaration* var, Expression* expr);
    ~PropertyDeclaration();
    void print();
    int eval();
};

// PARAMETER

class Parameter {
public:
    string identifier;
    Type* type;

    Parameter(string id, Type* type);
    ~Parameter();
};

class FunctionValueParameter {
public:
    Parameter* parameter;
    Expression* defaultValue;

    FunctionValueParameter(Parameter* param, Expression* defaultValue);
    ~FunctionValueParameter();
};

// ENDFUN

// BLOCK

class Block { // esto es un statementlist
public:
    list<Statement*> statements;

    Block();
    void add(Statement* stmt);
    void accept(Visitor* visitor);
    void print();
    ~Block();
};



class VariableDeclaration {
public:
    string identifier;
    Type* type;

    VariableDeclaration( string id, Type* type);
    ~VariableDeclaration();
};

// STATEMENTS

// **
class Statement {
public:
    // virtual void accept(Visitor* visitor) = 0;
    virtual ~Statement() = 0;
    virtual void print() = 0;
    virtual int eval() = 0;
};

class DeclarationStatement : public Statement {
public:
    Declaration* declaration;

    DeclarationStatement(Declaration* decl);
    // void accept(Visitor* visitor);
    ~DeclarationStatement();
    void print();
    int eval();
};

class AssignmentStatement : public Statement {
public:
    string identifier;
    Expression* expression;

    AssignmentStatement(string id, Expression* expr);
    //void accept(Visitor* visitor);
    ~AssignmentStatement();
    void print();
    int eval();
};

class ForStatement : public Statement {
public:
    VariableDeclaration* variable;
    Expression* expression;
    Block* body;

    ForStatement(VariableDeclaration* var, Expression* expr, Block* body);
    //void accept(Visitor* visitor);
    ~ForStatement();
    void print();
    int eval();
};

class WhileStatement : public Statement {
public:
    Expression* condition;
    Block* body;

    WhileStatement(Expression* cond, Block* body);
    //void accept(Visitor* visitor);
    ~WhileStatement();
    void print();
    int eval();
};

class ExpressionStatement : public Statement {
public:
    Expression* expression;

    ExpressionStatement(Expression* expr);
    // void accept(Visitor* visitor);
    ~ExpressionStatement();
    void print();
    int eval();
};

// **


// EXPRESSIONS

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


class IdentifierExpression : public Expression {
public:
    string identifier;

    IdentifierExpression( string id);
    void accept(Visitor* visitor);
    ~IdentifierExpression();
};

enum LiteralType { BOOLEAN_LITERAL, INTEGER_LITERAL, CHARACTER_LITERAL, STRING_LITERAL };

class LiteralExpression : public Expression {
public:
    LiteralType type;
    string value;

    LiteralExpression(LiteralType type,  string value);
    void accept(Visitor* visitor);
    ~LiteralExpression();
};

class IfExpression : public Expression {
public:
    Expression* condition;
    Block* thenBody;
    Block* elseBody;

    IfExpression(Expression* cond, Block* thenBody, Block* elseBody);
    void accept(Visitor* visitor);
    ~IfExpression();
};

class StringLiteral : public Expression {
public:
    string value;

    StringLiteral( string value);
    void accept(Visitor* visitor);
    ~StringLiteral();
};

#endif // EXP_H