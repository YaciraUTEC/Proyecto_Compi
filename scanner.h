//
// Created by denz on 21/11/24.
//

#ifndef PROYECTO_COMPI_SCANNER_H
#define PROYECTO_COMPI_SCANNER_H


#include <string>
#include "token.h"

class Scanner {
private:
    std::string input;
    int first, current;
public:
    Scanner(const char* in_s);
    Token* nextToken();
    void reset();
    ~Scanner();
};

void test_scanner(Scanner* scanner);

#endif //PROYECTO_COMPI_SCANNER_H
