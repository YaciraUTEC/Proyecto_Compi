#ifndef IMP_CODEGEN
#define IMP_CODEGEN

#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <fstream>

#include "../exp.h"
#include "../visitor.h"
#include "../interprete/environment.h"
#include "imp_type_checker.hh"

class VarEntry {
public:
  int dir;
  bool is_global;
};

class ImpCodeGen : public Visitor {
public:
  ImpCodeGen(ImpTypeChecker*);
  void codegen(KotlinFile*, string outfname);
  void visit(Block* b);
  void visit(KotlinFile* kf);
  void visit(FunctionDeclaration*fd);
  void visit(PropertyDeclaration* e);
  void visit(StatementList* e);
  void visit(AssignmentStatement* e);
  void visit(PrintlnStatement* e);
  void visit(ExpressionStatement* e);
  void visit(WhileStatement* e);
  void visit(DeclarationStatement* e);
  void visit(ForStatement* e);

  int visit(BinaryExpression* e);
  int visit(StringLiteral* e);
  int visit(LiteralExpression* e);
  int visit(IdentifierExpression* e);
  int visit(FunctionCallExpression* e);
  int visit(IfExpression* e);
  int visit(JumpExpression* e);

private:
  ImpTypeChecker* analysis;
  std::ostringstream code;
  string nolabel;
  int current_label;
  Environment<VarEntry> direcciones;
  int current_dir;
  int max_stack, mem_locals, mem_globals;
  bool process_global;
  int num_params; // nuevo
  void codegen(string label, string instr);
  void codegen(string label, string instr, int arg);
  void codegen(string label, string instr, string jmplabel);
  string next_label();
  string get_flabel(string fname);
};


#endif
