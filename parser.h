#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "exp.h"

class Parser {
private:
    Scanner* scanner;
    Token *current, *previous;
    bool match(Token::Type ttype);
    bool check(Token::Type ttype);
    bool advance();
    bool isAtEnd();

    // Funciones de parsing según la gramática
    KotlinFile* parseKotlinFile();
    Declaration* parseTopLevelObject();
    Declaration* parseDeclaration();
    FunctionDeclaration* parseFunctionDeclaration();

    list<FunctionValueParameter*> parseFunctionValueParameters();
    FunctionValueParameter* parseFunctionValueParameter();

    Parameter* parseParameter();
    Block* parseFunctionBody();
    Block* parseBlock();
    
    list<Statement*> parseStatements();
    Statement* parseStatement();
    
    AssignmentStatement* parseAssignment();
    
    Statement* parseLoopStatement();
    
    ForStatement* parseForStatement();
    WhileStatement* parseWhileStatement();
    VariableDeclaration* parseVariableDeclaration();
    Block* parseControlStructureBody();
    
    Expression* parseExpression();
    Expression* parseDisjunction();
    
    Expression* parseConjunction();
    Expression* parseEquality();
    
    Expression* parseComparison();

    Expression* parseGenericCallLikeComparison();
    
    Expression* parseRangeExpression();
    
    Expression* parseAdditiveExpression();
    Expression* parseMultiplicativeExpression();
    Expression* parsePrimaryExpression();
    IfExpression* parseIfExpression();

    LiteralExpression* parseLiteralConstant();

public:
    Parser(Scanner* scanner);
    ~Parser();
    KotlinFile* parse();
};

#endif // PARSER_H