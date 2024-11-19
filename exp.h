#ifndef EXP_H
#define EXP_H

#include <string>
#include <unordered_map>
#include <list>
#include "visitor.h"
#include <iostream>

#pragma region Interprete_imports
#include "interprete/imp_value.hh"
#include "interprete/imp_type.hh"
#pragma endregion Interprete_imports


using namespace std;

#pragma region Interprete_def
class ImpValueVisitor;
#pragma endregion Interprete_def

class FunctionDeclaration;
class Expression;
class Declaration;
class Statement;
class FunctionValueParameter;
class Block;
class VariableDeclaration;
class StatementList;

class Type {
public:
    string name;
    Type(string name);
    ~Type();
    void print();
};

// KOTLINFILE
class KotlinFile {
public:
    list<Declaration*> decl;
    KotlinFile();
    void add(Declaration* decl);
    void print();
    // -------------------------------------------------------
    void accept(ImpValueVisitor* kf);
    // -------------------------------------------------------
};

// DECLARATIONS
class Declaration {
public:
    virtual void print() = 0;
    // -------------------------------------------------------
    virtual void accept(ImpValueVisitor* v) = 0;
    // -------------------------------------------------------
    virtual ~Declaration() = 0;
};

class FunctionDeclaration : public Declaration {
public:
    string identifier;
    list<FunctionValueParameter*> parameters;
    Type* returnType;
    Block* fbody;
    // -------------------------------------------------------
    void accept(ImpValueVisitor* v);
    // -------------------------------------------------------
    FunctionDeclaration(string id, list<FunctionValueParameter*> params, Type* rtype, Block* fbody);
    ~FunctionDeclaration();
    void print();
};

class PropertyDeclaration : public Declaration {
public:
    string ptype; // var o val
    VariableDeclaration* variable;
    Expression* expression;
    // -------------------------------------------------------
    void accept(ImpValueVisitor* v);
    // -------------------------------------------------------
    PropertyDeclaration(string ptype, VariableDeclaration* var, Expression* expr);
    ~PropertyDeclaration();
    void print();
};

// PARAMETER
class Parameter {
public:
    string identifier;
    Type* type;
    // -------------------------------------------------------
    void accept(ImpValueVisitor* v);
    // -------------------------------------------------------
    Parameter(string id, Type* type);
    ~Parameter();
    void print();
};

class FunctionValueParameter {
public:
    Parameter* parameter;
    // -------------------------------------------------------
    void accept(ImpValueVisitor* v);
    // -------------------------------------------------------
    FunctionValueParameter(Parameter* param);
    ~FunctionValueParameter();
    void print();
};

// BLOCK
class Block { // esto es un statementlist
public:
    StatementList* slist;
    // -------------------------------------------------------
    void accept(ImpValueVisitor* v);
    // -------------------------------------------------------
    Block(StatementList* stms);
    void print();
    ~Block();
};

class StatementList {
public:
    list<Statement*> statements;
    StatementList();
    void add(Statement* stmt);
    // -------------------------------------------------------
    void accept(ImpValueVisitor* v);
    // -------------------------------------------------------
    void print();
    ~StatementList();
};

class VariableDeclaration {
public:
    string identifier;
    Type* type;
    // -------------------------------------------------------
    void accept(ImpValueVisitor* v);
    // -------------------------------------------------------
    VariableDeclaration(string id, Type* type);
    ~VariableDeclaration();
    void print();
};

// ================================================================================================= //

// STATEMENTS
class Statement {
public:
    // -------------------------------------------------------
    virtual void accept(ImpValueVisitor* v) = 0;
    // -------------------------------------------------------
    virtual ~Statement() = 0;
    virtual void print() = 0;
};

class DeclarationStatement : public Statement {
public:
    Declaration* declaration;
    DeclarationStatement(Declaration* decl);
    // -------------------------------------------------------
    void accept(ImpValueVisitor* v);
    // -------------------------------------------------------
    ~DeclarationStatement();
    void print();
};

class AssignmentStatement : public Statement {
public:
    string identifier;
    Expression* expression;
    AssignmentStatement(string id, Expression* expr);
    // -------------------------------------------------------
    void accept(ImpValueVisitor* v);
    // -------------------------------------------------------
    ~AssignmentStatement();
    void print();
};

class ForStatement : public Statement {
public:
    VariableDeclaration* variable;
    Expression* expression;
    Block* fbody;
    ForStatement(VariableDeclaration* var, Expression* expr, Block* fbody);
    // -------------------------------------------------------
    void accept(ImpValueVisitor* v);
    // -------------------------------------------------------
    ~ForStatement();
    void print();
};

class WhileStatement : public Statement {
public:
    Expression* condition;
    Block* wbody;
    WhileStatement(Expression* cond, Block* wbody);
    // -------------------------------------------------------
    void accept(ImpValueVisitor* v);
    // -------------------------------------------------------
    ~WhileStatement();
    void print();
};

class ExpressionStatement : public Statement {
public:
    Expression* expression;
    ExpressionStatement(Expression* expr);
    // -------------------------------------------------------
    void accept(ImpValueVisitor* v) = 0;
    // -------------------------------------------------------
    ~ExpressionStatement();
    void print();
};

class PrintlnStatement : public Statement {
public:
    Expression* expression;
    PrintlnStatement(Expression* expr);
    // -------------------------------------------------------
    void accept(ImpValueVisitor* v);
    // -------------------------------------------------------
    ~PrintlnStatement();
    void print();
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
    // -------------------------------------------------------
    virtual ImpValue accept(ImpValueVisitor* v) = 0;
    // -------------------------------------------------------
    virtual ~Expression() = 0;
    virtual void print() = 0;
    static string binopToChar(BinaryOp op);
};

class BinaryExpression : public Expression {
public:
    Expression* left;
    Expression* right;
    BinaryOp op;
    BinaryExpression(Expression* lhs, Expression* rhs, BinaryOp op);
    // -------------------------------------------------------
    ImpValue accept(ImpValueVisitor* v);
    // -------------------------------------------------------
    ~BinaryExpression();
    void print();
};

class IdentifierExpression : public Expression {
public:
    string identifier;
    IdentifierExpression(string id);
    // -------------------------------------------------------
    ImpValue accept(ImpValueVisitor* v);
    // -------------------------------------------------------
    ~IdentifierExpression();
    void print();
};

class IfExpression : public Expression {
public:
    Expression* condition;
    Block* thenBody;
    Block* elseBody;
    IfExpression(Expression* cond, Block* thenBody, Block* elseBody);
    // -------------------------------------------------------
    void accept2(ImpValueVisitor* v);
    ImpValue accept(ImpValueVisitor* v);
    // -------------------------------------------------------
    ~IfExpression();
    void print();
};

class JumpExpression : public Expression {
public:
    Expression* returnExpression;
    JumpExpression(Expression* ret);
    // -------------------------------------------------------
    ImpValue accept(ImpValueVisitor* v);
    // -------------------------------------------------------
    ~JumpExpression();
    void print();
};

enum LiteralType { BOOLEAN_LITERAL, INTEGER_LITERAL, CHARACTER_LITERAL, STRING_LITERAL };

class LiteralExpression : public Expression {
public:
    LiteralType type;
    string value;
    LiteralExpression(LiteralType type, string value);
    // -------------------------------------------------------
    ImpValue accept(ImpValueVisitor* v);
    // -------------------------------------------------------
    ~LiteralExpression();
    void print();
};

class StringLiteral : public Expression {
public:
    string value;
    StringLiteral(string value);
    // -------------------------------------------------------
    ImpValue accept(ImpValueVisitor* v);
    // -------------------------------------------------------
    ~StringLiteral();
    void print();
};

#endif // EXP_H