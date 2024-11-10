#ifndef TOKEN_H
#define TOKEN_H

#include <string>

class Token {
public:
    enum Type {
        FUN,              // fun
        IDENTIFIER,       // Identificadores (nombres de variables, funciones, etc.)
        COLON,            // :
        COMMA,            // ,
        ASSIGN,           // =
        LEFT_PAREN,       // (
        RIGHT_PAREN,      // )
        LEFT_BRACE,       // {
        RIGHT_BRACE,      // }
        FOR,              // for
        WHILE,            // while
        IF,               // if
        ELSE,             // else
        IN,               // in
        PRINT,            // print | Tener cuidado con el uso de esta palabra
        
        // Operadores aritméticos y de comparación
        PLUS,             // +
        MINUS,            // -
        MUL,              // *
        DIV,              // /
        LT,               // <
        GT,               // >
        LE,               // <=
        GE,               // >=
        EQ,               // ==
        RANGE_INCL,       // ..

        // Literales y otros tokens
        TRUE,             // true
        FALSE,            // false

        // Palabras reservadas
        VAR,              // var
        VAL,              // val (si se utiliza para variables inmutables)
        END,              // end: fin de un bloque de código
        
        // Errores
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