#ifndef TOKEN_H
#define TOKEN_H

#include <string>

class Token {
public:
    enum Type {
        FUN,              // fun
        RETURN,           // return
        FOR,              // for
        WHILE,            // while
        IF,               // if
        ELSE,             // else
        IN,               // in
        PRINT,            // print !!        
        TRUE,             // true
        FALSE,            // false
        VAR,              // var
        VAL,              // val (si se utiliza para variables inmutables)
        END,              // end: fin de un bloque de código        
        IDENTIFIER,       // Identificadores (nombres de variables, funciones, etc.)
        NUMBER,           // Números
        COLON,            // :
        COMMA,            // ,
        PCOMMA,           // ;
        ASSIGN,           // =
        LEFT_PAREN,       // (
        RIGHT_PAREN,      // )
        LEFT_BRACE,       // {
        RIGHT_BRACE,      // }
        PLUS,             // +
        MINUS,            // -
        MUL,              // *
        DIV,              // /
        LT,               // <
        GT,               // >
        LE,               // <=
        GE,               // >=
        EQ,               // ==
        OR,               // ||
        AND,              // &&
        NOT,              // !
        NE,               // !=
        RANGE_INCL,       // ..
        ERROR,            // Error
    };

    Type type;
    std::string text;

    Token(Type type);
    Token(Type type, char c);
    Token(Type type, const std::string& source, int first, int last);

    friend std::ostream& operator<<(std::ostream& outs, const Token& tok);
    friend std::ostream& operator<<(std::ostream& outs, const Token* tok);
};

#endif // TOKEN_H