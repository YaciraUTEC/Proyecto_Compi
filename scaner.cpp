#include <iostream>
#include <cstring>
#include "token.h"
#include "scanner.h"

using namespace std;

// Gramática - Kotlin

// kotlinFile ::= {topLevelOject}
// topLevelOject ::= declaration [';']
// declaration ::= functionDeclaration
// functionDeclaration ::= 'fun' Identifier functionValueParameters [':' type] functionBody
// Identifier ::= (Letter | '_ ') {Letter | '_' | UnicodeDigit} | '´' QuotedSymbol {QuotedSymbol} '`'
// functionValueParameters ::= '(' [functionValueParameter {',' functionValueParameter}] ')'
// functionValueParameter ::= parameter ['=' expression]
// parameter ::= Identifier ':' type
// functionBody ::= block | ('=' expression)
// block ::= '{' statements '}'
// statements ::= [statement {';' statement}] [';']
// statement ::= (declaration | assignment | loopStatement | expression)
// assignment ::= Identifier '=' expression
// loopStatement ::= forStatement | whileStatement
// forStatement ::= 'for' '(' variableDeclaration 'in' expression ')' [controlStructureBody]
// variableDeclaration ::= Identifier [':' type]
// controlStructureBody ::= block | statement
// whileStatement ::= 'while' '(' expression ')' (controlStructureBody | ';')
// expression ::= disjunction
// disjunction ::= conjunction {'||' conjunction}
// conjunction ::= equality {'&&' equality}
// equality ::= comparison {('!=' | '==') comparison}
// comparison ::= genericCallLikeComparison {('<' | '>' | '<=' | '>=') genericCallLikeComparison}
// genericCallLikeComparison ::= rangeExpression
// rangeExpression ::= additiveExpression {('..' | '..<') additiveExpression}
// additiveExpression ::= multiplicativeExpression {('+' | '-') multiplicativeExpression}
// multiplicativeExpression ::= primaryExpression {('*' | '/') primaryExpression}
// primaryExpression ::= Identifier | '(' expression ')' | stringLiteral | ifExpression | literalConstant
// ifExpression ::= '(' expression ')' (controlStructureBody | ([controlStructureBody] [';'] 'else' (controlStructureBody | ';')) | ';')
// literalConstant ::= BooleanLiteral | IntegerLiteral | CharacterLiteral

Scanner::Scanner(const char* s):input(s),first(0), current(0) { }

bool is_white_space(char c) {
    return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

Token* Scanner::nextToken() {
    Token* token;
    while (current < input.length() && is_white_space(input[current])) current++;
    if (current >= input.length()) return new Token(Token::END);
    char c = input[current];
    first = current;

    if (isdigit(c)) {
        current++;
        while (current < input.length() && isdigit(input[current])) current++;
        token = new Token(Token::NUMBER, input, first, current - first);
    }

    else if (isalpha(c)) {
        current++;
        while (current < input.length() && (isalnum(input[current]) || input[current] == '_')) current++;
        string word = input.substr(first, current - first);
        
        if (word == "println") token = new Token(Token::PRINT, word, 0, word.length()); // ¡ Revisar
        else if (word == "fun") token = new Token(Token::FUN, word, 0, word.length());
        else if (word == "return") token = new Token(Token::RETURN, word, 0, word.length());
        else if (word == "for") token = new Token(Token::FOR, word, 0, word.length());
        else if (word == "in") token = new Token(Token::IN, word, 0, word.length());
        else if (word == "while") token = new Token(Token::WHILE, word, 0, word.length());
        else if (word == "if") token = new Token(Token::IF, word, 0, word.length());
        else if (word == "else") token = new Token(Token::ELSE, word, 0, word.length());
        else if (word == "var") token = new Token(Token::VAR, word, 0, word.length());
        else if (word == "val") token = new Token(Token::VAL, word, 0, word.length());
        else if (word == "true") token = new Token(Token::TRUE, word, 0, word.length());
        else if (word == "false") token = new Token(Token::FALSE, word, 0, word.length());
        else token = new Token(Token::IDENTIFIER, word, 0, word.length());
    } 
    else if (strchr("+-*/(){}=,;:<>.", c)) {
        switch(c) {
            case '+': token = new Token(Token::PLUS, c); break;
            case '-': token = new Token(Token::MINUS, c); break;
            case '*': token = new Token(Token::MUL, c); break;
            case '/': token = new Token(Token::DIV, c); break;
            case ':': token = new Token(Token::COLON, c); break;
            case ',': token = new Token(Token::COMMA, c); break;
            case ';': token = new Token(Token::PCOMMA, c); break;
            case '(': token = new Token(Token::LEFT_PAREN, c); break;
            case ')': token = new Token(Token::RIGHT_PAREN, c); break;
            case '{': token = new Token(Token::LEFT_BRACE, c); break;
            case '}': token = new Token(Token::RIGHT_BRACE, c); break;
            case '=':
                if (current + 1 < input.length() && input[current + 1] == '=') {
                    token = new Token(Token::EQ, "==", 0, 2);
                    current++;
                } else {
                    token = new Token(Token::ASSIGN, c);
                }
                break;
            case '<':
                if (current + 1 < input.length() && input[current + 1] == '=') {
                    token = new Token(Token::LE, "<=", 0, 2);
                    current++;
                } else {
                    token = new Token(Token::LT, c);
                }
                break;
            case '>':
                if (current + 1 < input.length() && input[current + 1] == '=') {
                    token = new Token(Token::GE, ">=", 0, 2);
                    current++;
                } else {
                    token = new Token(Token::GT, c);
                }
                break;
            case '.':
                if (current + 1 < input.length() && input[current + 1] == '.') {
                    token = new Token(Token::RANGE_INCL, "..", 0, 2);
                    current++;
                } else {
                    token = new Token(Token::ERROR, c);
                }
                break;

            default:
                cout << "No debería llegar aquí" << endl;
                token = new Token(Token::ERROR, c);
        }
        current++;
    }
    else {
        token = new Token(Token::ERROR, c);
        current++;
    }
    return token;
}

void Scanner::reset() {
    first = 0;
    current = 0;
}

Scanner::~Scanner() { }

void test_scanner(Scanner* scanner) {
    Token* current;
    cout << "Iniciando Scanner:" << endl<< endl;
    while ((current = scanner->nextToken())->type != Token::END) {
        if (current->type == Token::ERROR) {
            cout << "Error en scanner - carácter inválido: " << current->text << endl;
            break;
        } else {
            cout << *current << endl;
        }
        delete current;
    }
    cout << "TOKEN(END)" << endl;
    delete current;
}