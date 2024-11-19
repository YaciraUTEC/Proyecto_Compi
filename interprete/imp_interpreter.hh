#ifndef IMP_INTERPRETER
#define IMP_INTERPRETER

#include <unordered_map>

#include "../exp.h"
#include "imp_value_visitor.hh"
#include "environment.hh"

using namespace std;

class ImpInterpreter : public ImpValueVisitor {
private:
  Environment<ImpValue> env;
  Environment<FunctionDeclaration*> fdecs;
  ImpValue retval;
  bool retcall;

public:
  void interpret(KotlinFile*);
  void visit(KotlinFile*);
  void visit(Declaration*);
  void visit(Block*);
  void visit(FunctionDeclaration*);
  void visit(PropertyDeclaration*);
  void visit(StatementList*);
  void visit(AssignmentStatement*);
  void visit(PrintlnStatement*);
  void visit(DeclarationStatement*);
  void visit(ExpressionStatement*);
  void visit(WhileStatement*);
  void visit(ForStatement*);
  ImpValue visit(BinaryExpression* e);
  ImpValue visit(IdentifierExpression* e);
  void visit(IfExpression* e);
  ImpValue visit(JumpExpression* e);
  ImpValue visit(LiteralExpression* e);
  ImpValue visit(StringLiteral* e);
};


#endif