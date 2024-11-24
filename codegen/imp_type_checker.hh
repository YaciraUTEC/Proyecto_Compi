#ifndef IMP_TYPECHECKER
#define IMP_TYPECHECKER

#include <unordered_map>
#include <vector>

#include "../exp.h"
#include "type_visitor.h"
#include "../interprete/environment.h"

using namespace std;


class FEntry {
public:
  string fname;
  ImpType ftype;
  int mem_locals, max_stack;
};

class ImpTypeChecker : public TypeVisitor {
public:
  ImpTypeChecker();
  Environment<FEntry> ftable;  
private:
  Environment<ImpType> env;
  bool has_main;
  ImpType booltype;
  ImpType inttype;
  ImpType longtype;
  ImpType unittype;
  ImpType maintype;

  vector<string> fnames; 
  int sp, max_sp;
  int dir, max_dir;
  void sp_decr(int n);
  void sp_incr(int n);

public:
  void typecheck(KotlinFile*);

  void visit(KotlinFile* kf);
  void visit(Declaration* d);
  void visit(Block* b);
  void visit(FunctionDeclaration*fd);
  void visit(PropertyDeclaration* e);
  void visit(StatementList* e);
  void visit(AssignmentStatement* e);
  void visit(PrintlnStatement* e);
  void visit(ExpressionStatement* e);
  void visit(WhileStatement* e);
  void visit(DeclarationStatement* e);
  void visit(ForStatement* e);
  ImpType visit(BinaryExpression* e);
  ImpType visit(StringLiteral* e);
  ImpType visit(LiteralExpression* e);
  ImpType visit(IdentifierExpression* e);
  ImpType visit(FunctionCallExpression* e);
  ImpType visit(IfExpression* e);
  ImpType visit(JumpExpression* e);

};


#endif
