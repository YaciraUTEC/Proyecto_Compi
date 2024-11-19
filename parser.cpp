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

Parser::Parser(Scanner* sc) :scanner(sc) {
    previous = NULL;
    current = scanner->nextToken();
    if (current->type == Token::ERROR) {
        cout << "Error en el primer token: " << current->text << endl;
        exit(1);
    }
}

// =======================================
// Parsing del archivo principal de Kotlin
// =======================================

KotlinFile* Parser::parseKotlinFile() {
    KotlinFile* kotlinFile = new KotlinFile();

    while (!isAtEnd()) {
        Declaration* decl = parseDeclaration();
        if (decl != nullptr) {
            kotlinFile->add(decl);
        }
    }

    return kotlinFile;
}

// ================================
// Parsing de declaraciones
// ================================

Declaration* Parser::parseDeclaration() {
    if (match(Token::Type::FUN)) {
        return parseFunctionDeclaration();
    }
    if (match(Token::Type::VAR) || match(Token::Type::VAL)) {
        return parsePropertyDeclaration();
    }
    return nullptr; // Error si no se encuentra una declaración válida
}

FunctionDeclaration* Parser::parseFunctionDeclaration() {
    if (!match(Token::Type::IDENTIFIER)) {
        cout << "Error: Se esperaba un identificador de función" << endl;
        exit(1);
    }

    string id = previous->text;

    if (!match(Token::Type::LEFT_PAREN)) {
        cout << "Error: Se esperaba un '(' después del identificador de función" << endl;
        exit(1);
    }

    list<FunctionValueParameter*> parameters;
    while (!check(Token::Type::RIGHT_PAREN)) {
        if (!match(Token::Type::IDENTIFIER)) {
            cout << "Error: Se esperaba un identificador de parámetro de función" << endl;
            exit(1);
        }
        string id = previous->text;

        if (!match(Token::Type::COLON)) {
            cout << "Error: Se esperaba ':' después del identificador del parámetro de función" << endl;
            exit(1);
        }

        if (!match(Token::Type::INT) && !match(Token::Type::BOOLEAN) && !match(Token::Type::STRING)) {
            cout << "Error: Se esperaba un tipo después de ':'" << endl;
            exit(1);
        }

        string type = previous->text;

        Parameter* param = new Parameter(id, type);

        parameters.push_back(new FunctionValueParameter(param));

        if (!match(Token::Type::COMMA) && !check(Token::Type::RIGHT_PAREN)) {
            cout << "Error: Se esperaba una ',' después de un parámetro de función" << endl;
            exit(1);
        }
    }

    match(Token::Type::RIGHT_PAREN); // Consume ')'

    string returnType = "";
    if (match(Token::Type::COLON)) {
        if (!match(Token::Type::INT) && !match(Token::Type::BOOLEAN) && !match(Token::Type::STRING)) {
            cout << "Error: Se esperaba un tipo de retorno después de ':'" << endl;
            exit(1);
        }
        returnType = previous->text;
    }

    Block* fbody = parseBlock();

    return new FunctionDeclaration(id, parameters, returnType, fbody);
}

PropertyDeclaration* Parser::parsePropertyDeclaration() {
    string ptype = previous->type == Token::Type::VAR ? "var" : "val";

    VariableDeclaration* variable = parseVariableDeclaration();

    Expression* expression = nullptr;
    if (match(Token::Type::ASSIGN)) {
        expression = parseExpression();
    }

    return new PropertyDeclaration(ptype, variable, expression);
}

VariableDeclaration* Parser::parseVariableDeclaration() {
    if (!match(Token::Type::IDENTIFIER)) {
        cout << "Error: Se esperaba un identificador de variable" << endl;
        exit(1);
    }

    string id = previous->text;

    if (!match(Token::Type::COLON)) {
        cout << "Error: Se esperaba ':' después del identificador de variable" << endl;
        exit(1);
    }

    if (!match(Token::Type::INT) && !match(Token::Type::BOOLEAN) && !match(Token::Type::STRING)) {
        cout << "Error: Se esperaba un tipo después de ':'" << endl;
        exit(1);
    }

    string type = previous->text;

    return new VariableDeclaration(id, type);
}

// ================================
// Parsing de bloques y statements
// ================================

Block* Parser::parseBlock() {
    if (!match(Token::Type::LEFT_BRACE)) {
        cout << "Error: Se esperaba un '{' para iniciar un bloque" << endl;
        exit(1);
    }

    StatementList* stmtList = parseStatementList();

    if (!match(Token::Type::RIGHT_BRACE)) {
        cout << "Error: Se esperaba un '}' para finalizar un bloque" << endl;
        exit(1);
    }

    return new Block(stmtList);
}

StatementList* Parser::parseStatementList() {
    StatementList* stmtList = new StatementList();

    while (!check(Token::Type::RIGHT_BRACE) && !isAtEnd()) {
        Statement* stmt = parseStatement();
        if (stmt != nullptr) {
            stmtList->add(stmt);
        }
    }

    return stmtList;
}

Statement* Parser::parseStatement() {
    if (current == NULL) {
        cout << "Error: No hay tokens" << endl;
        exit(1);
    }
    // PARA DECLARATION QUE SON PROPERTY O FUNCTION
    if (match(Token::Type::VAR) || match(Token::Type::VAL)) {
        Declaration* decl = parsePropertyDeclaration();
        return new DeclarationStatement(decl);
    }
    else if (match(Token::Type::FUN)) {
        Declaration* decl = parseFunctionDeclaration();
        return new DeclarationStatement(decl);
    }
    // PARA STATEMENTS QUE SON IF, WHILE, FOR, ASSIGNMENT
    else if (match(Token::Type::IF)) {
        return new ExpressionStatement(parseIfExpression());
    }
    else if(match(Token::Type::RETURN)){
        return new ExpressionStatement(parseJumpExpression());
    }
    else if (match(Token::Type::WHILE)) {
        if (!match(Token::Type::LEFT_PAREN)) {
            cout << "Error: Se esperaba un '(' después del while" << endl;
            exit(1);
        }
        Expression* condition = parseExpression();
        if (!match(Token::Type::RIGHT_PAREN)) {
            cout << "Error: Se esperaba un ')' después de la condición del while" << endl;
            exit(1);
        }
        Block* wbody = parseBlock();
        return new WhileStatement(condition, wbody);
    }
    else if (match(Token::Type::FOR)) {
        if (!match(Token::Type::LEFT_PAREN)) {
            cout << "Error: Se esperaba un '(' después del for" << endl;
            exit(1);
        }
        VariableDeclaration* var = parseVariableDeclaration();
        if (!match(Token::Type::IN)) {
            cout << "Error: Se esperaba un 'in' después de la variable del for" << endl;
            exit(1);
        }
        Expression* range = parseExpression();
        Block* body = parseBlock();
        return new ForStatement(var, range, body);
    }
    // PARA ASSIGNS
    else if (match(Token::Type::IDENTIFIER)) {
        string id = previous->text;
        if (match(Token::Type::ASSIGN)) {
            Expression* expr = parseExpression();
            if(match(Token::Type::PCOMMA)){
                return new AssignmentStatement(id, expr); // Si encuentra bien
            }
            else{
                return new AssignmentStatement(id, expr); // Si no, también xd
            }

        }
    }else if (match(Token::Type::PRINT)) {
        if (!match(Token::Type::LEFT_PAREN)) {
            cout << "Error: Se esperaba un '(' después de 'print'" << endl;
            exit(1);
        }
        Expression* expr = parseExpression();
        if (!match(Token::Type::RIGHT_PAREN)) {
            cout << "Error: Se esperaba un ')' después de la expresión de 'print'" << endl;
            exit(1);
        }
        return new PrintlnStatement(expr);
    }
    else {
        cout << "Error: Se esperaba un identificador o 'print', pero se encontró: " << *current << endl;
        exit(1);
    }
    return nullptr;
}

// ================================
// Parsing de expresiones
// ================================

Expression* Parser::parseExpression() {
    return parseDisjunction();
}

Expression* Parser::parseDisjunction() {
    Expression* expr = parseConjunction();

    while (match(Token::Type::OR)) {
        expr = new BinaryExpression(expr, parseConjunction(), OR_OP);
    }

    return expr;
}

Expression* Parser::parseConjunction() {
    Expression* expr = parseEquality();

    while (match(Token::Type::AND)) {
        expr = new BinaryExpression(expr, parseEquality(), AND_OP);
    }

    return expr;
}

Expression* Parser::parseEquality() {
    Expression* expr = parseComparison();

    while (match(Token::Type::EQ) || match(Token::Type::NE)) {
        BinaryOp op = previous->type == Token::Type::EQ ? EQ_OP : NE_OP;
        expr = new BinaryExpression(expr, parseComparison(), op);
    }

    return expr;
}

Expression* Parser::parseComparison() {
    Expression* expr = parseRangeExpression();

    while (match(Token::Type::LT) || match(Token::Type::GT) ||
        match(Token::Type::LE) || match(Token::Type::GE)) {
        BinaryOp op;
        if (previous->type == Token::Type::LT) op = LT_OP;
        else if (previous->type == Token::Type::GT) op = GT_OP;
        else if (previous->type == Token::Type::LE) op = LE_OP;
        else op = GE_OP;

        expr = new BinaryExpression(expr, parseRangeExpression(), op);
    }

    return expr;
}

Expression* Parser::parseRangeExpression() {
    Expression* expr = parseAdditiveExpression();

    while (match(Token::Type::RANGE_INCL)) {
        expr = new BinaryExpression(expr, parseAdditiveExpression(), RANGE_OP);
    }

    return expr;
}

Expression* Parser::parseAdditiveExpression() {
    Expression* expr = parseMultiplicativeExpression();

    while (match(Token::Type::PLUS) || match(Token::Type::MINUS)) {
        BinaryOp op = previous->type == Token::Type::PLUS ? ADD_OP : SUB_OP;
        expr = new BinaryExpression(expr, parseMultiplicativeExpression(), op);
    }

    return expr;
}

Expression* Parser::parseMultiplicativeExpression() {
    Expression* expr = parsePrimaryExpression();

    while (match(Token::Type::MUL) || match(Token::Type::DIV)) {
        BinaryOp op = previous->type == Token::Type::MUL ? MUL_OP : DIV_OP;
        expr = new BinaryExpression(expr, parsePrimaryExpression(), op);
    }

    return expr;
}

Expression* Parser::parsePrimaryExpression() {
    if (match(Token::Type::NUMBER)) {
        return new LiteralExpression(INTEGER_LITERAL, previous->text);
    }
    else if (match(Token::Type::TRUE) || match(Token::Type::FALSE)) {
        return new LiteralExpression(BOOLEAN_LITERAL, previous->text);
    }
    else if (match(Token::Type::IDENTIFIER)) {
        return new IdentifierExpression(previous->text);
    }
    else if (match(Token::Type::LEFT_PAREN)) {
        Expression* expr = parseExpression();
        if (!match(Token::Type::RIGHT_PAREN)) {
            throw std::runtime_error("Expected ')' after expression");
        }
        return expr;
    }else if (match(Token::Type::STRING)) {
        return new LiteralExpression(STRING_LITERAL, previous->text);
    }
    cout << "Error: Se esperaba un número, un identificador o un paréntesis, pero se encontró: " << *current << endl;
    exit(0);
}

Expression* Parser::parseIfExpression() {
    Expression* condition = parseExpression();
    Block* ifBody = parseBlock();

    Block* elseBody = nullptr;
    if (match(Token::Type::ELSE)) {
        elseBody = parseBlock();
    }

    return new IfExpression(condition, ifBody, elseBody);
}

Expression* Parser::parseJumpExpression(){
    Expression* e = parseExpression();
    if(e){
        return new JumpExpression(e);
    }else{
        return new JumpExpression(nullptr);
    }
}