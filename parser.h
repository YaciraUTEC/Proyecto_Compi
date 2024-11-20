#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "exp.h"

class Parser {
private:
    Scanner* scanner;
    Token* current, * previous;
    bool match(Token::Type ttype);
    bool check(Token::Type ttype);
    bool advance();
    bool isAtEnd();

    // Funciones de parsing según la gramática

    list<Statement*> parseStmList();
    Expression* parseExpression();
    Expression* parseDisjunction();
    Expression* parseConjunction();
    Expression* parseEquality();
    Expression* parseComparison();
    Expression* parseRangeExpression();
    Expression* parseAdditiveExpression(); // esto es como un parseexpression
    Expression* parseMultiplicativeExpression(); // esto es como un parseterm
    Expression* parsePostfixUnaryExpression();
    Expression* parseIfExpression();
    Expression* parseJumpExpression();
    Expression* parsePrimaryExpression(); // esto es como un parsefactor


public:
    Parser(Scanner* scanner);
    KotlinFile* parseKotlinFile();
    Declaration* parseDeclaration();

    FunctionDeclaration* parseFunctionDeclaration();
    Block* parseBlock(); // esto es como un parse statementlist
    Statement* parseStatement();
    StatementList* parseStatementList();
    VariableDeclaration* parseVariableDeclaration();
    PropertyDeclaration* parsePropertyDeclaration();

};

#endif // PARSER_H