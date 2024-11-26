#include "imp_codegen.hh"

ImpCodeGen::ImpCodeGen(ImpTypeChecker* a):analysis(a) {

}

void ImpCodeGen::codegen(string label, string instr) {
  if (label !=  nolabel)
    code << label << ": ";
  code << instr << endl;
}

void ImpCodeGen::codegen(string label, string instr, int arg) {
  if (label !=  nolabel)
    code << label << ": ";
  code << instr << " " << arg << endl;
}

void ImpCodeGen::codegen(string label, string instr, string jmplabel) {
  if (label !=  nolabel)
    code << label << ": ";
  code << instr << " " << jmplabel << endl;
}

string ImpCodeGen::next_label() {
  string l = "L";
  string n = to_string(current_label++);
  l.append(n);
  return l;
}

string ImpCodeGen::get_flabel(string fname) {
  string l = "L";
  l.append(fname);
  return l;
}

void ImpCodeGen::codegen(KotlinFile* p, string outfname) {
  nolabel = "";
  current_label = 0;

  p->accept(this);
  ofstream outfile;
  outfile.open(outfname);
  outfile << code.str();
  outfile.close();

  return;
}

void ImpCodeGen::visit(KotlinFile* p) {
    current_dir = 0;  
    direcciones.add_level();
    process_global = true;
    mem_globals = current_dir;
    codegen("start", "skip");

    codegen(nolabel, "enter", mem_globals);
    codegen(nolabel, "alloc", mem_globals);
    codegen(nolabel, "mark");
    codegen(nolabel, "pusha", get_flabel("main"));
    codegen(nolabel, "call");
    codegen(nolabel, "halt");
    
    for(auto d : p->decl) {
        d->accept(this);
    }

    direcciones.remove_level();
}

void ImpCodeGen::visit(Block* b) {
  direcciones.add_level();
  int saved_dir = current_dir;
  
  b->slist->accept(this);
  
  current_dir = saved_dir;
  direcciones.remove_level();
}

void ImpCodeGen::visit(PropertyDeclaration* s) {
    current_dir++;
    VarEntry ventry;
    ventry.dir = current_dir;
    ventry.is_global = process_global;
    direcciones.add_var(s->variable->identifier, ventry);
    
    if (s->expression != nullptr) {
        s->expression->accept(this);
        if (ventry.is_global) {
            codegen(nolabel, "store", ventry.dir);
        } else {
            codegen(nolabel, "storer", ventry.dir);
        }
    }
}

void ImpCodeGen::visit(ExpressionStatement* vd) {
  if (vd->expression != nullptr) {
      vd->expression->accept(this);
  }
}

void ImpCodeGen::visit(FunctionDeclaration* fd) {
    FEntry fentry = analysis->ftable.lookup(fd->identifier);
    
    current_dir = 0;
    direcciones.add_level();
    
    bool prev_process_global = process_global;
    process_global = false;
    
    num_params = fd->parameters.size();
  
    codegen(get_flabel(fd->identifier), "skip");
    
    int param_offset = -3;
    for (auto param : fd->parameters) {
        VarEntry ventry;
        ventry.dir = param_offset--;
        ventry.is_global = false;
        direcciones.add_var(param->parameter->identifier, ventry);
    }
    
    int frame_size = 2;  
    codegen(nolabel, "enter", frame_size + fentry.mem_locals + fentry.max_stack);
    codegen(nolabel, "alloc", fentry.mem_locals);
    
    fd->fbody->accept(this);
    
    if (fd->identifier == "main") {
        codegen(nolabel, "return", 3);
    } else {
        codegen(nolabel, "return", num_params + 3);
    }
    process_global = prev_process_global;
    direcciones.remove_level();
}

void ImpCodeGen::visit(DeclarationStatement* fd) {
    if (fd->declaration != nullptr) {
        fd->declaration->accept(this);
    }

    if (auto funcDecl = dynamic_cast<FunctionDeclaration*>(fd->declaration)) {
        num_params = funcDecl->parameters.size();
    }
}

void ImpCodeGen::visit(StatementList* s) {
  for (auto stmt : s->statements) {
      stmt->accept(this);
  }
}

void ImpCodeGen::visit(AssignmentStatement* s) {
    s->expression->accept(this);
    VarEntry ventry = direcciones.lookup(s->identifier);
    
    if (ventry.is_global) {
        codegen(nolabel, "store", ventry.dir);
    } else {
        codegen(nolabel, "storer", ventry.dir);
    }
}

void ImpCodeGen::visit(PrintlnStatement* s) {
  s->expression->accept(this);
  codegen(nolabel, "print");
}

int ImpCodeGen::visit(IfExpression* s) {
    string else_label = next_label();
    string end_label = next_label();
    
    s->condition->accept(this);
    codegen(nolabel, "jmpz", else_label);
    
    if (s->thenBody != nullptr) {
        s->thenBody->accept(this);
    }
    codegen(nolabel, "goto", end_label);
    
    codegen(else_label, "skip");
    if (s->elseBody != nullptr) {
        s->elseBody->accept(this);
    }
    
    codegen(end_label, "skip");
    return 0;
}

void ImpCodeGen::visit(WhileStatement* s) {
  string start_label = next_label();
  string end_label = next_label();
  
  codegen(start_label, "skip");
  s->condition->accept(this);
  codegen(nolabel, "jmpz", end_label);
  
  s->wbody->accept(this);
  codegen(nolabel, "goto", start_label);
  
  codegen(end_label, "skip");
}

int ImpCodeGen::visit(JumpExpression* s) {
  if (s->returnExpression != nullptr) {
      s->returnExpression->accept(this);
      codegen(nolabel, "storer", -1 * (num_params + 3));
  }
  codegen(nolabel, "return", num_params + 3);
  return 0;
}

void ImpCodeGen::visit(ForStatement* s) {
    string start_label = next_label();
    string end_label = next_label();
    
    current_dir++;
    VarEntry ventry;
    ventry.dir = current_dir;
    ventry.is_global = false;
    direcciones.add_var(s->variable->identifier, ventry);

    codegen(nolabel, "alloc", 1);
    
    BinaryExpression* range = dynamic_cast<BinaryExpression*>(s->expression);
    if (range && range->op == RANGE_OP) {
        range->left->accept(this);
        codegen(nolabel, "storer", ventry.dir);
        
        codegen(start_label, "skip");
        
        codegen(nolabel, "loadr", ventry.dir);
        range->right->accept(this);
        codegen(nolabel, "gt");
        codegen(nolabel, "jmpz", end_label);
        
        s->fbody->accept(this);
        
        codegen(nolabel, "loadr", ventry.dir);
        codegen(nolabel, "push", "1");
        codegen(nolabel, "add");
        codegen(nolabel, "storer", ventry.dir);
        
        codegen(nolabel, "goto", start_label);
        codegen(end_label, "skip");
    }
}

int ImpCodeGen::visit(BinaryExpression* e) {
  e->left->accept(this);
  e->right->accept(this);
  
  string op;
  switch(e->op) {
      case ADD_OP: op = "add"; break;
      case SUB_OP: op = "sub"; break;
      case MUL_OP: op = "mul"; break;
      case DIV_OP: op = "div"; break;
      case LT_OP:  op = "lt"; break;
      case GT_OP:  op = "gt"; break;
      case LE_OP:  op = "le"; break;
      case GE_OP:  op = "ge"; break;
      case EQ_OP:  op = "eq"; break;
      case NE_OP:  op = "ne"; break;
      case AND_OP: op = "and"; break;
      case OR_OP:  op = "or"; break;
      default:
          cout << "Operador no soportado" << endl;
          exit(1);
  }
  codegen(nolabel, op);
  return 0;
}

int ImpCodeGen::visit(StringLiteral* e) {

return 0;
}

int ImpCodeGen::visit(LiteralExpression* e) {
  switch(e->type) {
    case INTEGER_LITERAL:
    case LONG_LITERAL:
        codegen(nolabel, "push", e->value);
        break;
    case BOOLEAN_LITERAL:
        codegen(nolabel, "push", (e->value == "true") ? "1" : "0");
        break;
    default:
        cout << "Tipo de literal no soportado" << endl;
        exit(1);
  }
  return 0;
}

int ImpCodeGen::visit(IdentifierExpression* e) {
    if (!process_global) {
        VarEntry ventry = direcciones.lookup(e->identifier);
        
        if (ventry.dir < 0) {
            codegen(nolabel, "loadr", ventry.dir);
            return 0;
        }
    }
    
    VarEntry ventry = direcciones.lookup(e->identifier);
    if (ventry.is_global) {
        codegen(nolabel, "load", ventry.dir);
    } else {
        codegen(nolabel, "loadr", ventry.dir);
    }
    return 0;
}

int ImpCodeGen::visit(FunctionCallExpression* e) {
  FEntry fentry = analysis->ftable.lookup(e->identifier);
  
  codegen(nolabel, "alloc", 1);
  
  for (auto arg : e->arguments) {
      arg->accept(this);
  }
  
  codegen(nolabel, "mark");
  codegen(nolabel, "pusha", get_flabel(e->identifier));
  codegen(nolabel, "call");
  return 0;
}
