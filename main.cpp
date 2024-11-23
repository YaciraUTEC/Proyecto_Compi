#include <iostream>
#include <fstream>
#include <string>
#include "scanner.h"
#include "parser.h"
// #include "visitor.h"
#include "interprete/imp_interpreter.hh"
#include "interprete/imp_type.hh"
// #include "codegen/imp_type_checker.hh"
// #include "codegen/imp_codegen.hh"


using namespace std;

int main(int argc, const char* argv[]) {
    if (argc != 2) {
        cout << "Numero incorrecto de argumentos. Uso: " << argv[0] << " <archivo_de_entrada>" << endl;
        exit(1);
    }

    ifstream infile(argv[1]);
    if (!infile.is_open()) {
        cout << "No se pudo abrir el archivo: " << argv[1] << endl;
        exit(1);
    }

    string input;
    string line;
    while (getline(infile, line)) {
        input += line + '\n';
    }
    infile.close();

    Scanner scanner(input.c_str());

    string input_copy = input;
    Scanner scanner_test(input_copy.c_str());
    test_scanner(&scanner_test);

    cout << "Scanner exitoso" << endl;

    Parser parser(&scanner);
    try {
        KotlinFile* kotlinFile = parser.parseKotlinFile();
        cout << "parsing exitoso" << endl;
        kotlinFile->print();
        cout << "printing exitoso" << endl;
        cout << "Interpretando..." << endl;
        ImpInterpreter interpreter;

        interpreter.interpret(kotlinFile);

        // ImpTypeChecker typeChecker;


        // ImpCodeGen codegen(&typeChecker);
    }
    catch (const std::runtime_error& e) {
        cout << e.what() << endl;
        return 1;
    }


    return 0;
}