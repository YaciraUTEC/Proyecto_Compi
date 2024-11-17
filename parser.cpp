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

// Retorno de chat

// // parser.cpp
// #include "parser.h"

// KotlinFile* Parser::parseKotlinFile() {
//     KotlinFile* file = new KotlinFile();
//     while (!isAtEnd()) {
//         file->add(parseTopLevelObject());
//     }
//     return file;
// }

// Declaration* Parser::parseTopLevelObject() {
//     Declaration* decl = parseDeclaration();
//     match(Token::PCOMMA); // Punto y coma opcional
//     return decl;
// }

// Declaration* Parser::parseDeclaration() {
//     if (match(Token::FUN)) {
//         string identifier = current->text;
//         if (!match(Token::IDENTIFIER)) {
//             throw runtime_error("Se esperaba un identificador");
//         }
        
//         list<FunctionValueParameter*> params = parseFunctionValueParameters();
        
//         Type* returnType = nullptr;
//         if (match(Token::COLON)) {
//             returnType = parseType();
//         }
        
//         Block* body = parseFunctionBody();
//         return new FunctionDeclaration(identifier, params, returnType, body);
//     }
//     throw runtime_error("Declaración no válida");
// }

// Expression* Parser::parseExpression() {
//     return parseConjunction();
// }

// Expression* Parser::parseConjunction() {
//     Expression* expr = parseEquality();
    
//     while (match(Token::OR)) {
//         Expression* right = parseEquality();
//         expr = new BinaryExpression(expr, right, BinaryOp::OR_OP);
//     }
    
//     return expr;
// }

// Expression* Parser::parseEquality() {
//     Expression* expr = parseComparison();
    
//     while (match(Token::EQ)) {
//         Expression* right = parseComparison();
//         expr = new BinaryExpression(expr, right, BinaryOp::EQ_OP);
//     }
    
//     return expr;
// }

// Expression* Parser::parseComparison() {
//     Expression* expr = parseGenericCallLikeComparison();
    
//     while (match(Token::LT) || match(Token::GT) || 
//            match(Token::LE) || match(Token::GE)) {
//         Token* op = previous;
//         Expression* right = parseGenericCallLikeComparison();
        
//         BinaryOp binOp;
//         if (op->type == Token::LT) binOp = BinaryOp::LT_OP;
//         else if (op->type == Token::GT) binOp = BinaryOp::GT_OP;
//         else if (op->type == Token::LE) binOp = BinaryOp::LE_OP;
//         else binOp = BinaryOp::GE_OP;
        
//         expr = new BinaryExpression(expr, right, binOp);
//     }
    
//     return expr;
// }

// Expression* Parser::parseGenericCallLikeComparison() {
//     Expression* expr = parseAdditiveExpression();
    
//     while (match(Token::RANGE_INCL)) {
//         Expression* right = parseAdditiveExpression();
//         expr = new BinaryExpression(expr, right, BinaryOp::RANGE_OP);
//     }
    
//     return expr;
// }

// Expression* Parser::parseAdditiveExpression() {
//     Expression* expr = parseMultiplicativeExpression();
    
//     while (match(Token::PLUS) || match(Token::MINUS)) {
//         Token* op = previous;
//         Expression* right = parseMultiplicativeExpression();
        
//         BinaryOp binOp = (op->type == Token::PLUS) ? BinaryOp::ADD_OP : BinaryOp::SUB_OP;
//         expr = new BinaryExpression(expr, right, binOp);
//     }
    
//     return expr;
// }

// Expression* Parser::parseMultiplicativeExpression() {
//     Expression* expr = parsePrimaryExpression();
    
//     while (match(Token::MUL) || match(Token::DIV)) {
//         Token* op = previous;
//         Expression* right = parsePrimaryExpression();
        
//         BinaryOp binOp = (op->type == Token::MUL) ? BinaryOp::MUL_OP : BinaryOp::DIV_OP;
//         expr = new BinaryExpression(expr, right, binOp);
//     }
    
//     return expr;
// }

// Expression* Parser::parsePrimaryExpression() {
//     if (match(Token::IDENTIFIER)) {
//         return new IdentifierExpression(previous->text);
//     }
    
//     if (match(Token::LEFT_PAREN)) {
//         Expression* expr = parseExpression();
//         if (!match(Token::RIGHT_PAREN)) {
//             throw runtime_error("Se esperaba ')'");
//         }
//         return expr;
//     }
    
//     if (match(Token::IF)) {
//         return parseIfExpression();
//     }
    
//     // Literales
//     if (match(Token::NUMBER)) {
//         return new LiteralExpression(LiteralType::INTEGER_LITERAL, previous->text);
//     }
    
//     if (match(Token::TRUE) || match(Token::FALSE)) {
//         return new LiteralExpression(LiteralType::BOOLEAN_LITERAL, previous->text);
//     }
    
//     throw runtime_error("Se esperaba una expresión");
// }

// Statement* Parser::parseStatement() {
//     if (match(Token::VAR) || match(Token::VAL)) {
//         return parseVariableDeclaration();
//     }
    
//     if (match(Token::FOR)) {
//         return parseForStatement();
//     }
    
//     if (match(Token::WHILE)) {
//         return parseWhileStatement();
//     }
    
//     if (match(Token::IDENTIFIER) && check(Token::ASSIGN)) {
//         return parseAssignment();
//     }
    
//     Expression* expr = parseExpression();
//     return new ExpressionStatement(expr);
// }

// Block* Parser::parseBlock() {
//     if (!match(Token::LEFT_BRACE)) {
//         throw runtime_error("Se esperaba '{'");
//     }
    
//     Block* block = new Block();
//     while (!check(Token::RIGHT_BRACE) && !isAtEnd()) {
//         block->add(parseStatement());
//         match(Token::PCOMMA); // Punto y coma opcional
//     }
    
//     if (!match(Token::RIGHT_BRACE)) {
//         throw runtime_error("Se esperaba '}'");
//     }
    
//     return block;
// }

