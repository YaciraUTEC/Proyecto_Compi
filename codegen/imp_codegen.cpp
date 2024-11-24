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

    // Generar código inicial
    codegen("start", "skip");
    
    // Procesar declaraciones globales para contar variables
    for(auto d : p->decl) {
        d->accept(this);
    }
    
    // Actualizar espacio para variables globales
    mem_globals = current_dir;
    codegen(nolabel, "enter", mem_globals + 1); // +1 para el frame base
    codegen(nolabel, "alloc", mem_globals);
    
    process_global = false;

    // Llamar a main
    codegen(nolabel, "mark");
    codegen(nolabel, "pusha", get_flabel("main"));
    codegen(nolabel, "call");
    codegen(nolabel, "halt");
    
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
  // Si hay una expresión, evaluarla
  if (vd->expression != nullptr) {
      // La expresión podría ser una llamada a función o cualquier otra expresión
      vd->expression->accept(this);
      
      // Como es un statement, descartamos el resultado si lo hubiera
      // (por ejemplo, si es una llamada a función que retorna valor)
      codegen(nolabel, "pop");
  }
}

void ImpCodeGen::visit(FunctionDeclaration* fd) {
    FEntry fentry = analysis->ftable.lookup(fd->identifier);
    
    current_dir = 0;
    direcciones.add_level();
    
    // Generar etiqueta de función
    codegen(get_flabel(fd->identifier), "skip");
    
    // Calcular tamaño del frame incluyendo espacio para variables locales
    int frame_size = fd->parameters.size() + 3; // +3 para fp, ep y pc
    codegen(nolabel, "enter", frame_size);
    
    // Reservar espacio para variables locales
    codegen(nolabel, "alloc", fentry.mem_locals);
    
    // Procesar parámetros
    int param_count = 0;
    for(auto param : fd->parameters) {
        VarEntry ventry;
        ventry.is_global = false;
        ventry.dir = param_count + 1; // Dirección relativa positiva
        direcciones.add_var(param->parameter->identifier, ventry);
        param_count++;
    }
    
    // Procesar cuerpo
    fd->fbody->accept(this);
    
    // Return implícito para Unit
    if(fd->returnType == "Unit") {
        codegen(nolabel, "return", fd->parameters.size() + 3);
    }
    
    direcciones.remove_level();
}

void ImpCodeGen::visit(DeclarationStatement* fd) {
    // Procesar la declaración contenida
    if (fd->declaration != nullptr) {
        // Delegar el procesamiento a la visita específica del tipo de declaración
        fd->declaration->accept(this);
    }

    // Actualizar el número de parámetros si es necesario
    if (auto funcDecl = dynamic_cast<FunctionDeclaration*>(fd->declaration)) {
        num_params = funcDecl->parameters.size();
    }
}

void ImpCodeGen::visit(StatementList* s) {
  // Procesar cada statement en la lista
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
    
    // Evaluar condición
    s->condition->accept(this);
    codegen(nolabel, "jmpz", else_label);
    
    // Cuerpo del then
    if (s->thenBody != nullptr) {
        s->thenBody->accept(this);
    }
    codegen(nolabel, "goto", end_label);
    
    // Cuerpo del else
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
  // Manejar return expressions
  if (s->returnExpression != nullptr) {
      s->returnExpression->accept(this);
  }
  codegen(nolabel, "return", num_params + 3);
  return 0;
}

void ImpCodeGen::visit(ForStatement* s) {
  string start_label = next_label();
  string end_label = next_label();
  
  // Reservar espacio para la variable del loop
  current_dir++;
  VarEntry ventry;
  ventry.dir = current_dir;
  ventry.is_global = false;
  direcciones.add_var(s->variable->identifier, ventry);
  
  // Obtener rango (start..end)
  BinaryExpression* range = dynamic_cast<BinaryExpression*>(s->expression);
  if (range && range->op == RANGE_OP) {
      // Inicializar contador
      range->left->accept(this);
      codegen(nolabel, "storer", ventry.dir);
      
      // Inicio del loop
      codegen(start_label, "skip");
      
      // Cargar contador y comparar con fin
      codegen(nolabel, "loadr", ventry.dir);
      range->right->accept(this);
      codegen(nolabel, "gt");
      codegen(nolabel, "jmpz", end_label);
      
      // Ejecutar cuerpo
      s->fbody->accept(this);
      
      // Incrementar contador
      codegen(nolabel, "loadr", ventry.dir);
      codegen(nolabel, "push", "1");
      codegen(nolabel, "add");
      codegen(nolabel, "storer", ventry.dir);
      
      // Volver al inicio
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
    VarEntry ventry = direcciones.lookup(e->identifier);
    if (ventry.is_global) {
        codegen(nolabel, "load", ventry.dir);
    } else {
        codegen(nolabel, "loadr", ventry.dir);
    }
    return 0;
}

int ImpCodeGen::visit(FunctionCallExpression* e) {
  // Obtener información de la función
  // FEntry fentry = analysis->ftable.lookup(e->identifier);
  
  // // Reservar espacio para valor de retorno si no es Unit
  // if (!fentry.ftype.match(analysis->unittype)) {
  //     codegen(nolabel, "alloc", 1);
  // }
  
  // // Evaluar y apilar argumentos
  // for (auto arg : e->arguments) {
  //     arg->accept(this);
  // }
  
  // // Llamar a la función
  // codegen(nolabel, "mark");
  // codegen(nolabel, "pusha", get_flabel(e->identifier));
  // codegen(nolabel, "call");
  
  return 0;
}
