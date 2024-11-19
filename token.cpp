#include <iostream>
#include "token.h"

using namespace std;

Token::Token(Type type) : type(type), text("") {}

Token::Token(Type type, char c):type(type) { text = string(1, c); }

Token::Token(Type type, const string& source, int first, int last):type(type) {
    text = source.substr(first, last);
}

std::ostream& operator << ( std::ostream& outs, const Token & tok ) {
    switch (tok.type) {
        case Token::FUN: outs << "TOKEN(FUN)"; break;
        case Token::RETURN: outs << "TOKEN(RETURN)"; break;
        case Token::IDENTIFIER: outs << "TOKEN(IDENTIFIER)"; break;
        case Token::NUMBER: outs << "TOKEN(NUMBER)"; break;
        case Token::COLON: outs << "TOKEN(COLON)"; break;
        case Token::COMMA: outs << "TOKEN(COMMA)"; break;
        case Token::PCOMMA: outs << "TOKEN(PCOMMA)"; break;
        case Token::ASSIGN: outs << "TOKEN(ASSIGN)"; break;
        case Token::LEFT_PAREN: outs << "TOKEN(LEFT_PAREN)"; break;
        case Token::RIGHT_PAREN: outs << "TOKEN(RIGHT_PAREN)"; break;
        case Token::LEFT_BRACE: outs << "TOKEN(LEFT_BRACE)"; break;
        case Token::RIGHT_BRACE: outs << "TOKEN(RIGHT_BRACE)"; break;
        case Token::FOR: outs << "TOKEN(FOR)"; break;
        case Token::WHILE: outs << "TOKEN(WHILE)"; break;
        case Token::IF: outs << "TOKEN(IF)"; break;
        case Token::ELSE: outs << "TOKEN(ELSE)"; break;
        case Token::IN: outs << "TOKEN(IN)"; break;
        case Token::PRINT: outs << "TOKEN(PRINTLN)"; break;
        case Token::PLUS: outs << "TOKEN(PLUS)"; break;
        case Token::MINUS: outs << "TOKEN(MINUS)"; break;
        case Token::MUL: outs << "TOKEN(MUL)"; break;
        case Token::DIV: outs << "TOKEN(DIV)"; break;
        case Token::LT: outs << "TOKEN(LT)"; break;
        case Token::GT: outs << "TOKEN(GT)"; break;
        case Token::LE: outs << "TOKEN(LE)"; break;
        case Token::GE: outs << "TOKEN(GE)"; break;
        case Token::EQ: outs << "TOKEN(EQ)"; break;
        case Token::OR: outs << "TOKEN(OR)"; break;
        case Token::AND: outs << "TOKEN(AND)"; break;
        case Token::NOT: outs << "TOKEN(NOT)"; break;
        case Token::RANGE_INCL: outs << "TOKEN(RANGE_INCL)"; break;
        case Token::TRUE: outs << "TOKEN(TRUE)"; break;
        case Token::FALSE: outs << "TOKEN(FALSE)"; break;
        case Token::VAR: outs << "TOKEN(VAR)"; break;
        case Token::VAL: outs << "TOKEN(VAL)"; break;
        case Token::END: outs << "TOKEN(END)"; break;
        case Token::STRING: outs << "TOKEN(STRING)"; break;
        case Token::INT: outs << "TOKEN(INT)"; break;
        case Token::BOOLEAN: outs << "TOKEN(BOOLEAN)"; break;
        case Token::ERROR: outs << "TOKEN(ERROR)"; break;
        default: outs << "TOKEN(UNKNOWN)"; break;
    }
    return outs;
}

std::ostream& operator << ( std::ostream& outs, const Token* tok ) {
    return outs << *tok;
}