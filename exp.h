#ifndef EXP_H
#define EXP_H

#include <string>
#include <unordered_map>
#include <list>
#include "visitor.h"
#include <iostream>

using namespace std;

class FunctionDeclaration;
class Expression;

class Type {
public:
    string name;
    void accept(Visitor* visitor);
    Type(string name);
    ~Type();
    void print();
    int eval();
};

// KOTLINFILE
class KotlinFile {
public:
    list<Declaration*> decl;
    KotlinFile();
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
    void print();
    int eval();
};

class FunctionValueParameter {
public:
    Parameter* parameter;
    Expression* defaultValue;

    FunctionValueParameter(Parameter* param, Expression* defaultValue);
    ~FunctionValueParameter();
    void print();
    int eval();
};

// BLOCK
class Block { // esto es un statementlist
public:
    StatementList* slist;

    Block(StatementList* stms);
    void accept(Visitor* visitor);
    void print();
    int eval();
    ~Block();
};

class StatementList {
public:
    list<Statement*> statements;

    StatementList();
    void add(Statement* stmt);
    void accept(Visitor* visitor);
    void print();
    int eval();
    ~StatementList();
};

class VariableDeclaration {
public:
    string identifier;
    Type* type;

    VariableDeclaration(string id, Type* type);
    ~VariableDeclaration();
    void print();
    int eval();
};

// ================================================================================================= //

// STATEMENTS
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
    Block* fbody;

    ForStatement(VariableDeclaration* var, Expression* expr, Block* fbody);
    //void accept(Visitor* visitor);
    ~ForStatement();
    void print();
    int eval();
};

class WhileStatement : public Statement {
public:
    Expression* condition;
    Block* wbody;

    WhileStatement(Expression* cond, Block* wbody);
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


// ================================================================================================= //

enum BinaryOp {
    ADD_OP,
    SUB_OP,
    MUL_OP,
    DIV_OP,
    LT_OP,
    GT_OP,
    LE_OP,
    GE_OP,
    EQ_OP,
    NE_OP,
    AND_OP,
    OR_OP,
    RANGE_OP
};

// EXPRESSIONS
class Expression {
public:
    virtual void accept(Visitor* visitor) = 0;
    virtual ~Expression() = 0;
    virtual void print() = 0;
    virtual int eval() = 0;
    static string binopToChar(BinaryOp op);
};

class BinaryExpression : public Expression {
public:
    Expression* left;
    Expression* right;
    BinaryOp op;

    BinaryExpression(Expression* lhs, Expression* rhs, BinaryOp op);
    void accept(Visitor* visitor);
    ~BinaryExpression();
    void print();
    int eval();
};

class IdentifierExpression : public Expression {
public:
    string identifier;

    IdentifierExpression(string id);
    void accept(Visitor* visitor);
    ~IdentifierExpression();
    void print();
    int eval();
};

class IfExpression : public Expression {
public:
    Expression* condition;
    Block* thenBody;
    Block* elseBody;

    IfExpression(Expression* cond, Block* thenBody, Block* elseBody);
    void accept(Visitor* visitor);
    ~IfExpression();
    void print();
    int eval();
};

enum LiteralType { BOOLEAN_LITERAL, INTEGER_LITERAL, CHARACTER_LITERAL, STRING_LITERAL };

class LiteralExpression : public Expression {
public:
    LiteralType type;
    string value;

    LiteralExpression(LiteralType type, string value);
    void accept(Visitor* visitor);
    ~LiteralExpression();
    void print();
    int eval();
};

class StringLiteral : public Expression {
public:
    string value;

    StringLiteral(string value);
    void accept(Visitor* visitor);
    ~StringLiteral();
    void print();
    int eval();
};

#endif // EXP_H