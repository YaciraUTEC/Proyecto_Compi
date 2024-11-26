#include"imp_type_checker.hh"

ImpTypeChecker::ImpTypeChecker():inttype(), booltype(), longtype(), unittype() {
    inttype.set_basic_type("Int");
    longtype.set_basic_type("Long");
    booltype.set_basic_type("Boolean");
    unittype.set_basic_type("Unit");
    list<string> noparams;
    maintype.set_fun_type(noparams,"Unit");
    has_main = false;
    sp = 0;
    max_sp = 0;
    dir = 0;
    max_dir = 0;
}

void ImpTypeChecker::sp_incr(int n) {
    sp++;
    if (sp > max_sp) max_sp = sp;
}

void ImpTypeChecker::sp_decr(int n) {
  sp--;
  if (sp < 0) {
    cout << "stack less than 0" << endl;
    exit(0);
  }
}

void ImpTypeChecker::typecheck(KotlinFile* p) {
  env.clear();
  p->accept(this);
}

void ImpTypeChecker::visit(KotlinFile* kf) {
    env.add_level();
    ftable.add_level();

    for (Declaration* d : kf->decl) {
        d->accept(this);
    }

    if (!has_main) {
        cout << "Error: Programa no tiene función main" << endl;
        exit(0);
    }

    for(int i = 0; i < fnames.size(); i++) {
        cout << "-- Function: " << fnames[i] << endl;
        FEntry fentry = ftable.lookup(fnames[i]);
        cout << fentry.fname << " : " << fentry.ftype << endl;
        cout << "max stack height: " << fentry.max_stack << endl;
        cout << "mem local variables: " << fentry.mem_locals << endl;
    }
    env.remove_level();
}

void ImpTypeChecker::visit(Block* b) {
    // !
    env.add_level();
    int saved_dir = dir;
    int saved_sp = sp;
    b->slist->accept(this);
    dir = saved_dir;
    sp = saved_sp;
    env.remove_level();
}

void ImpTypeChecker::visit(PropertyDeclaration* vd) {
    ImpType type;
    type.set_basic_type(vd->variable->type);
    
    if (type.ttype == ImpType::NOTYPE) {
        cout << "Error: Tipo invalido para variable " << vd->variable->identifier << " (" << vd->variable->type << ")" << endl;
        exit(0);
    }
    
    if (vd->expression != nullptr) {
        ImpType expr_type = vd->expression->accept(this);
        
        if (!expr_type.match(type) && 
            !(type.ttype == ImpType::LONG && expr_type.ttype == ImpType::INT)) {
            cout << "Error: Tipo de expresión no coincide con tipo declarado para " << vd->variable->identifier << endl;
            cout << "Esperado: " << type << ", Encontrado: " << expr_type << endl;
            exit(0);
        }
        sp_decr(1);
    }
    
    env.add_var(vd->variable->identifier, type);
    dir++;
    if (dir > max_dir) max_dir = dir;
}

void ImpTypeChecker::visit(FunctionDeclaration* fd) {
    ImpType funtype;
    list<string> paramTypes;

    int saved_dir = dir;
    int saved_sp = sp;
    int saved_max_sp = max_sp;
    
    dir = 0;
    sp = 0;
    max_sp = 0;
    
    for (auto p : fd->parameters) {
        paramTypes.push_back(p->parameter->type);
    }
    
    if (fd->identifier == "main") {
        if (!paramTypes.empty()) {
            cout << "Error: main no debe tener parámetros" << endl;
            exit(0);
        }
        
        fd->returnType = "Unit";
        
        if (!funtype.set_fun_type(list<string>(), "Unit")) {
            cout << "Error al crear tipo de función main" << endl;
            exit(0);
        }
        has_main = true;
    } else {
        if (fd->returnType.empty()) {
            fd->returnType = "Unit";
        }

        if (!funtype.set_fun_type(paramTypes, fd->returnType)) {
            cout << "Error al crear tipo de función" << endl;
            exit(0);
        }
    }
    
    env.add_var(fd->identifier, funtype);
    env.add_level();

    for (auto p : fd->parameters) {
        ImpType ptype;
        if (!ptype.set_basic_type(p->parameter->type)) {
            cout << "Error: tipo de parámetro inválido: " << p->parameter->type << endl;
            exit(0);
        }
        env.add_var(p->parameter->identifier, ptype);
        dir++;
    }

    ImpType returnType;
    returnType.set_basic_type(fd->returnType);
    env.add_var("return", returnType);
    
    fd->fbody->accept(this);
    
    FEntry fentry;
    fentry.fname = fd->identifier;
    fentry.ftype = funtype;
    fentry.max_stack = max_sp;
    fentry.mem_locals = max_dir;
    
    fnames.push_back(fd->identifier);
    ftable.add_var(fd->identifier, fentry);
    
    // Restaurar estado
    dir = saved_dir;
    sp = saved_sp;
    max_sp = saved_max_sp;
    
    env.remove_level();
}

void ImpTypeChecker::visit(AssignmentStatement* s) {
    if (!env.check(s->identifier)) {
        cout << "Error: Variable " << s->identifier << " no definida" << endl;
        exit(0);
    }
    
    ImpType varType = env.lookup(s->identifier);
    
    ImpType exprType = s->expression->accept(this);
    
    if (!exprType.match(varType)) {
        if (!(varType.ttype == ImpType::LONG && exprType.ttype == ImpType::INT)) {
            cout << "Error: Tipo de expresión no coincide con tipo de variable " 
                    << s->identifier << endl;
            exit(0);
        }
    }

    sp_decr(1);
}

void ImpTypeChecker::visit(StatementList* s) {
    for (auto stmt : s->statements) {
        stmt->accept(this);
    }
}

void ImpTypeChecker::visit(PrintlnStatement* s) {
    s->expression->accept(this);
    sp_decr(1);
}

void ImpTypeChecker::visit(ExpressionStatement* e) {
    e->expression->accept(this);
    sp_decr(1);
}

void ImpTypeChecker::visit(DeclarationStatement* e) {
    e->declaration->accept(this);
}

ImpType ImpTypeChecker::visit(JumpExpression* s) {
    ImpType rtype = env.lookup("return");
    ImpType etype;
    
    if (s->returnExpression != nullptr) {
        etype = s->returnExpression->accept(this);
    } else {
        etype.set_basic_type("Unit");
    }
    
    if (!etype.match(rtype)) {
        cout << "Error: tipo de retorno no coincide" << endl;
        cout << "Esperado: " << rtype << ", Encontrado: " << etype << endl;
        exit(0);
    }
    
    return etype;
}

ImpType ImpTypeChecker::visit(LiteralExpression* e) {
    ImpType t;
    switch(e->type) {
        case INTEGER_LITERAL:
            t.set_basic_type("Int");
            break;
        case LONG_LITERAL:
            t.set_basic_type("Long");
            break;
        case BOOLEAN_LITERAL:
            t.set_basic_type("Boolean");
            break;
        case CHARACTER_LITERAL:
            cout << "Error: Caracteres no soportados" << endl;
            exit(0);
    }
    sp_incr(1);
    return t;
}

ImpType ImpTypeChecker::visit(IdentifierExpression* e) {
    if (!env.check(e->identifier)) {
        cout << "Error: variable " << e->identifier << " no definida" << endl;
        exit(0);
    }
    sp_incr(1);
    return env.lookup(e->identifier);
}

ImpType ImpTypeChecker::visit(IfExpression* s) {
    ImpType cond_type = s->condition->accept(this);
    if (!cond_type.match(booltype)) {
        cout << "Error: condición de if debe ser booleana" << endl;
        cout << "Tipo encontrado: " << cond_type << endl;
        exit(0);
    }
    
    env.add_level();
    
    if (s->thenBody != nullptr) {
        s->thenBody->accept(this);
    }
    
    if (s->elseBody != nullptr) {
        s->elseBody->accept(this);
    }
    
    env.remove_level();
    
    ImpType result;
    result.set_basic_type("Unit");
    return result;
}


void ImpTypeChecker::visit(WhileStatement* s) {
    if (!s->condition->accept(this).match(booltype)) {
        cout << "Expresion conditional en IF debe de ser bool" << endl;
        exit(0);
    }
    sp_decr(1);
    s->wbody->accept(this);
}

void ImpTypeChecker::visit(ForStatement* s) {
    env.add_level();
    
    ImpType var_type;
    var_type.set_basic_type("Int");
    env.add_var(s->variable->identifier, var_type);
    
    BinaryExpression* rangeExp = dynamic_cast<BinaryExpression*>(s->expression);
    if (!rangeExp || rangeExp->op != RANGE_OP) {
        cout << "Error: Se esperaba una expresión de rango (..)" << endl;
        exit(0);
    }
    
    ImpType start_type = rangeExp->left->accept(this);
    if (!start_type.match(inttype)) {
        cout << "Error: expresión de rango debe ser entera" << endl;
        exit(0);
    }
    sp_decr(1);
    
    ImpType end_type = rangeExp->right->accept(this);
    if (!end_type.match(inttype)) {
        cout << "Error: expresión de rango debe ser entera" << endl;
        exit(0);
    }
    sp_decr(1);
    
    s->fbody->accept(this);
    
    env.remove_level();
}


ImpType ImpTypeChecker::visit(BinaryExpression* e) {
    ImpType t1 = e->left->accept(this);
    ImpType t2 = e->right->accept(this);
    
    if (e->op == RANGE_OP) {
        if (!t1.match(inttype) || !t2.match(inttype)) {
            cout << "Los operandos del rango deben ser enteros" << endl;
            exit(0);
        }
        sp_decr(1);
        return inttype;
    }
    
    ImpType result;
    
    if (e->op == ADD_OP || e->op == SUB_OP || e->op == MUL_OP || e->op == DIV_OP) {
        if ((!t1.match(inttype) && !t1.match(longtype)) || 
            (!t2.match(inttype) && !t2.match(longtype))) {
            cout << "Error: Operaciones aritméticas requieren tipos numéricos" << endl;
            cout << "Tipo 1: " << t1 << ", Tipo 2: " << t2 << endl;
            exit(0);
        }
        
        if (t1.match(longtype) || t2.match(longtype)) {
            result = longtype;
        } else {
            result = inttype;
        }
    }
    else if (e->op == LT_OP || e->op == LE_OP || e->op == GT_OP || e->op == GE_OP || e->op == EQ_OP || e->op == NE_OP) {
        if ((!t1.match(inttype) && !t1.match(longtype)) || 
            (!t2.match(inttype) && !t2.match(longtype))) {
            cout << "Error: Comparaciones requieren tipos numéricos" << endl;
            cout << "Tipo 1: " << t1 << ", Tipo 2: " << t2 << endl;
            exit(0);
        }
        result = booltype;
    }
    else if (e->op == AND_OP || e->op == OR_OP) {
        if (!t1.match(booltype) || !t2.match(booltype)) {
            cout << "Error: Operaciones lógicas requieren tipos booleanos" << endl;
            cout << "Tipo 1: " << t1 << ", Tipo 2: " << t2 << endl;
            exit(0);
        }
        result = booltype;
    }
    else {
        cout << "Error: Operador binario no soportado" << endl;
        exit(0);
    }
    
    sp_decr(1);
    return result;
}

ImpType ImpTypeChecker::visit(StringLiteral* e) {
    ImpType t;
    t.set_basic_type("String");
    sp_incr(1);
    return t;
}

ImpType ImpTypeChecker::visit(FunctionCallExpression* e) {
    if (!env.check(e->identifier)) {
        cout << "Error: función " << e->identifier << " no definida" << endl;
        exit(0);
    }
    
    ImpType funtype = env.lookup(e->identifier);
    if (funtype.ttype != ImpType::FUN) {
        cout << "Error: " << e->identifier << " no es una función" << endl;
        exit(0);
    }
    
    if (funtype.types.size()-1 != e->arguments.size()) {
        cout << "Error: número incorrecto de argumentos para " << e->identifier << endl;
        exit(0);
    }
    
    int i = 0;
    for (auto arg : e->arguments) {
        ImpType argtype = arg->accept(this);
        ImpType paramtype;
        paramtype.set_basic_type(funtype.types[i]);
        if (!argtype.match(paramtype)) {
            cout << "Error: tipo de argumento incorrecto en llamada a " << e->identifier << endl;
            exit(0);
        }
        i++;
    }
    
    sp_decr(e->arguments.size());
    
    ImpType rtype;
    rtype.set_basic_type(funtype.types.back());
    if (!rtype.match(unittype)) {
        sp_incr(1);
    }
    return rtype;
}
