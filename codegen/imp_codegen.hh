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
  virtual void visit(KotlinFile* kf);
  virtual void visit(Block* b);
  virtual void visit(FunctionDeclaration*fd);
  virtual void visit(PropertyDeclaration* e);
  virtual void visit(StatementList* e);
  virtual void visit(AssignmentStatement* e);
  virtual void visit(PrintlnStatement* e);
  virtual void visit(ExpressionStatement* e);
  virtual void visit(WhileStatement* e);
  virtual void visit(DeclarationStatement* e);
  virtual void visit(ForStatement* e);
  virtual int visit(BinaryExpression* e);
  virtual int visit(StringLiteral* e);
  virtual int visit(LiteralExpression* e);
  virtual int visit(IdentifierExpression* e);
  virtual int visit(FunctionCallExpression* e);
  virtual int visit(IfExpression* e);
  virtual int visit(JumpExpression* e);

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
