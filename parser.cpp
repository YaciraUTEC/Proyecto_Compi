#include <iostream>
#include <stdexcept>
#include "token.h"
#include "scanner.h"
#include "exp.h"
#include "parser.h"

using namespace std;

bool Parser::match(Token::Type ttype) {
    if (check(ttype)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(Token::Type ttype) {
    if (isAtEnd()) return false;
    return current->type == ttype;
}

bool Parser::advance() {
    if (!isAtEnd()) {
        Token* temp = current;
        if (previous) delete previous;
        current = scanner->nextToken();
        previous = temp;
        if (check(Token::ERROR)) {
            cout << "Error de análisis, carácter no reconocido: " << current->text << endl;
            exit(1);
        }
        return true;
    }
    return false;
}

bool Parser::isAtEnd() {
    return (current->type == Token::END);
}

Parser::Parser(Scanner* sc):scanner(sc) {
    previous = NULL;
    current = scanner->nextToken();
    if (current->type == Token::ERROR) {
        cout << "Error en el primer token: " << current->text << endl;
        exit(1);
    }
}

// ================================================================================================= //

KotlinFile* Parser::parseKotlinFile() {
    KotlinFile* file = new KotlinFile();
    while (!isAtEnd()) {
        file->add(parseTopLevelObject());
    }
    return file;
}

Declaration* Parser::parseTopLevelObject() {
    Declaration* decl = parseDeclaration();
    // Revisar el caso del punto y coma opcional
    match(Token::PCOMMA); // Punto y coma opcional
    return decl;
}

Declaration* Parser::parseDeclaration() {
    if (match(Token::FUN)) {
        string identifier = current->text;
        if (!match(Token::IDENTIFIER)) {
            cout << "Se esperaba un identificador" << endl;
        }
        
        list<FunctionValueParameter*> params = parseFunctionValueParameters();
        
        Type* returnType = nullptr;
        if (match(Token::COLON)) {
            // Revisar este caso
            returnType = parseType();
        }
        
        Block* body = parseFunctionBody();
        return new FunctionDeclaration(identifier, params, returnType, body);
    }
    throw runtime_error("Declaración no válida");
}


// ===

Expression* Parser::parseExpression() {
    return parseConjunction();
}

Expression* Parser::parseConjunction() {
    Expression* expr = parseEquality();
    
    while (match(Token::OR)) {
        Expression* right = parseEquality();
        expr = new BinaryExpression(expr, right, BinaryOp::OR_OP);
    }
    
    return expr;
}



