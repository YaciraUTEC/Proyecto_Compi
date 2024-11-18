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
    Expression* parseGenericCallLikeComparison();
    Expression* parseRangeExpression();
    Expression* parseAdditiveExpression(); // esto es como un parseexpression
    Expression* parseMultiplicativeExpression(); // esto es como un parseterm 
    Expression* parseIfExpression();
    Expression* parsePrimaryExpression(); // esto es como un parsefactor


public:
    Parser(Scanner* scanner);
    KotlinFile* parseKotlinFile();
    Declaration* parseDeclaration();
    //FunctionDeclarationList* parseFunctionDeclarationList();
    FunctionDeclaration* parseFunctionDeclaration();
    Block* parseBlock(); // esto es como un parse statementlist
    Statement* parseStatement();
   // StatementList* parseStatementList();
};

#endif // PARSER_H