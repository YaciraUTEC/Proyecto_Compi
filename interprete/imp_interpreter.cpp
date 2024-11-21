#include "imp_interpreter.hh"

// Funciones accept de las clases del AST

ImpValue BinaryExpression::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

ImpValue IdentifierExpression::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

ImpValue FunctionCallExpression::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

void IfExpression::accept2(ImpValueVisitor* v) {
    return v->visit(this);
}

ImpValue IfExpression::accept(ImpValueVisitor* v) {
    ImpValue z;
    return z;
}

ImpValue StringLiteral::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

ImpValue JumpExpression::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

ImpValue LiteralExpression::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

void AssignmentStatement::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

void PrintlnStatement::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

void ExpressionStatement::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

void WhileStatement::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

void DeclarationStatement::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

void ForStatement::accept(ImpValueVisitor* v) {
    return v->visit(this);
}


void StatementList::accept(ImpValueVisitor* v) {
    return v->visit(this);
}


void FunctionDeclaration::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

void PropertyDeclaration::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

void Block::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

void KotlinFile::accept(ImpValueVisitor* v) {
    return v->visit(this);
}

// Funciones visit de ImpInterpreter

void ImpInterpreter::interpret(KotlinFile* kf) {
    env.clear();
    kf->accept(this);
    return;
}

void ImpInterpreter::visit(KotlinFile* kf) {
    
    // Revisar la implementación de este método
    env.add_level();
    fdecs.add_level();
    
    for (Declaration* d : kf->decl) {
        d->accept(this);
    }

    if (fdecs.check("main")) {
        FunctionDeclaration* main = fdecs.lookup("main");
        retcall = false;
        main->fbody->accept(this);
    } else {
        cout << "Error: No se encontro funcion main" << endl;
        exit(0);
    }

    if (!retcall) {
        cout << "Error: Funcion main no ejecuto RETURN" << endl;
        exit(0);
    }

    env.remove_level();
    fdecs.remove_level();
    
    /*
    env.add_level();
    fdecs.add_level();
    p->decl->
    p->FunctionDeclarations->accept(this);
    if (!fdecs.check("main")) {
        cout << "Error: No se encontro funcion main" << endl;
        exit(0);
    }
    FunctionDeclaration* main_dec = fdecs.lookup("main");
    retcall = false;
    main_dec->Block->accept(this);
    if (!retcall) {
        cout << "Error: Funcion main no ejecuto RETURN" << endl;
        exit(0);
    }
    */
}

void ImpInterpreter::visit(Declaration* d) {
    d->accept(this);
    return;
}

void ImpInterpreter::visit(Block* b) {
    env.add_level();
    b->slist->accept(this);
    env.remove_level();
    return;
}
/*
void ImpInterpreter::visit(FunctionDeclarationList* s) {
    list<FunctionDeclaration*>::iterator it;
    for (it = s->flist.begin(); it != s->flist.end(); ++it) {
        (*it)->accept(this);
    }
    return;
}
*/
void ImpInterpreter::visit(FunctionDeclaration* fd) {
    fdecs.add_var(fd->identifier, fd);
    return;
}

void ImpInterpreter::visit(PropertyDeclaration* p) {
    ImpValue v = p->expression->accept(this);
    env.add_var(p->variable->identifier, v);
    return;
}

void ImpInterpreter::visit(StatementList* s) {
    list<Statement*>::iterator it;
    for (it = s->statements.begin(); it != s->statements.end(); ++it) {
        (*it)->accept(this);
        if (retcall)
            break;
    }
    return;
}

void ImpInterpreter::visit(AssignmentStatement* s) {
    ImpValue v = s->expression->accept(this);
    if (!env.check(s->identifier)) {
        cout << "Variable " << s->identifier << " undefined" << endl;
        exit(0);
    }
    ImpValue lhs = env.lookup(s->identifier);
    if (lhs.type != v.type) {
        cout << "Type Error en Assign: Tipos de variable " << s->identifier;
        cout << " no coinciden" << endl;
        exit(0);
    }
    env.update(s->identifier, v);
    return;
}

void ImpInterpreter::visit(PrintlnStatement* s) {
    ImpValue v = s->expression->accept(this);
    cout << v << endl;
    return;
}

void ImpInterpreter::visit(IfExpression* s) {
    ImpValue v = s->condition->accept(this);
    if (v.type != TBOOL) {
        cout << "Type error en If: esperaba bool en condicional" << endl;
        exit(0);
    }
    if (v.bool_value) {
        s->thenBody->accept(this);
    }
    else {
        s->elseBody->accept(this);
    }
    return;
}

void ImpInterpreter::visit(WhileStatement* s) {
    ImpValue v = s->condition->accept(this);
    if (v.type != TBOOL) {
        cout << "Type error en WHILE: esperaba bool en condicional" << endl;
        exit(0);
    }
    while (s->condition->accept(this).bool_value) {
        s->wbody->accept(this);
    }
}

ImpValue ImpInterpreter::visit(JumpExpression* s) {
    if (s->returnExpression != NULL)
        retval = s->returnExpression->accept(this);
    retcall = true;
    return retval;
}

void ImpInterpreter::visit(ForStatement* s) {

    // Revisar la implementación de este método
    env.add_level();
    ImpValue v = s->expression->accept(this);

    if (v.type != TINT) {
        cout << "Error de tipos:  tienen que ser enteros" << endl;
        exit(0);
    }

    ImpValue start = s->expression->accept(this);
    ImpValue end = s->expression->accept(this);

    while (start.int_value < end.int_value) {
        s->fbody->accept(this);
        start.int_value++;
    }

    return;
    /*
    env.add_level();
    ImpValue start = s->start->accept(this);
    ImpValue end = s->end->accept(this);
    ImpValue paso = s->step->accept(this);
    if (start.type != TINT || end.type != TINT || paso.type != TINT) {
        cout << "Error de tipos:  tienen que ser enteros" << endl;
        exit(0);
    }
    int a = start.int_value;
    while (a < end.int_value) {
        s->b->accept(this);
        a += paso.int_value;
    }
    return;
    */
}

void ImpInterpreter::visit(DeclarationStatement* s) {
    s->declaration->accept(this);
    return;
}

void ImpInterpreter::visit(ExpressionStatement* s) {
    s->expression->accept(this);
    return;
}

ImpValue ImpInterpreter::visit(BinaryExpression* e) {
    ImpValue result;
    ImpValue v1 = e->left->accept(this);
    ImpValue v2 = e->right->accept(this);
    if (v1.type != TINT || v2.type != TINT) {
        cout << "Error de tipos: operandos en operacion binaria tienen que ser "
            "enteros"
            << endl;
        exit(0);
    }
    int iv, iv1, iv2;
    bool bv;
    ImpVType type = NOTYPE;
    iv1 = v1.int_value;
    iv2 = v2.int_value;
    switch (e->op) {
    case ADD_OP:
        iv = iv1 + iv2;
        type = TINT;
        break;
    case SUB_OP:
        iv = iv1 - iv2;
        type = TINT;
        break;
    case MUL_OP:
        iv = iv1 * iv2;
        type = TINT;
        break;
    case DIV_OP:
        iv = iv1 / iv2;
        type = TINT;
        break;
    case LT_OP:
        bv = (iv1 < iv2) ? 1 : 0;
        type = TBOOL;
        break;
    case LE_OP:
        bv = (iv1 <= iv2) ? 1 : 0;
        type = TBOOL;
        break;
    case EQ_OP:
        bv = (iv1 == iv2) ? 1 : 0;
        type = TBOOL;
        break;
    case NE_OP:
        bv = (iv1 != iv2) ? 1 : 0;
        type = TBOOL;
        break;
    case GT_OP:
        bv = (iv1 > iv2) ? 1 : 0;
        type = TBOOL;
        break;
    case GE_OP:
        bv = (iv1 >= iv2) ? 1 : 0;
        type = TBOOL;
        break;
    case AND_OP:
        bv = (v1.bool_value && v2.bool_value) ? 1 : 0;
        type = TBOOL;
        break;
    case OR_OP:
        bv = (v1.bool_value || v2.bool_value) ? 1 : 0;
        type = TBOOL;
        break;
    case RANGE_OP:
        // después
        break;
    }
    if (type == TINT)
        result.int_value = iv;
    else
        result.bool_value = bv;
    result.type = type;
    return result;
}

ImpValue ImpInterpreter::visit(LiteralExpression* e) {

    ImpValue v;
    /*
    v.set_default_value(TINT);
    v.int_value = e->value;
     */
    return v;
}

/*
ImpValue ImpInterpreter::visit(BoolExp* e) {
    ImpValue v;
    v.set_default_value(TBOOL);
    v.int_value = e->value;
    return v;
}
*/

ImpValue ImpInterpreter::visit(IdentifierExpression* e) {
    if (env.check(e->identifier))
        return env.lookup(e->identifier);
    else {
        cout << "Variable indefinida: " << e->identifier << endl;
        exit(0);
    }
}

ImpValue ImpInterpreter::visit(StringLiteral* e) {
    ImpValue v;
    /*
    v.set_default_value(TSTRING);
    v.string_value = e->value;
     */
    return v;
}

/*
ImpValue ImpInterpreter::visit(FCallExp* e) {
    FunctionDeclaration* fdec = fdecs.lookup(e->fname);
    env.add_level();
    list<Exp*>::iterator it;
    list<string>::iterator varit;
    list<string>::iterator vartype;
    ImpVType tt;
    if (fdec->vars.size() != e->args.size()) {
        cout << "Error: Numero de parametros incorrecto en llamada a "
            << fdec->fname << endl;
        exit(0);
    }
    for (it = e->args.begin(), varit = fdec->vars.begin(),
        vartype = fdec->types.begin();
        it != e->args.end(); ++it, ++varit, ++vartype) {
        tt = ImpValue::get_basic_type(*vartype);
        ImpValue v = (*it)->accept(this);
        if (v.type != tt) {
            cout << "Error FCall: Tipos de param y arg no coinciden. Funcion "
                << fdec->fname << " param " << *varit << endl;
            exit(0);
        }
        env.add_var(*varit, v);
    }
    retcall = false;
    fdec->Block->accept(this);
    if (!retcall) {
        cout << "Error: Funcion " << e->fname << " no ejecuto RETURN" << endl;
        exit(0);
    }
    retcall = false;
    env.remove_level();
    tt = ImpValue::get_basic_type(fdec->rtype);
    if (tt != retval.type) {
        cout << "Error: Tipo de retorno incorrecto de funcion " << fdec->fname
            << endl;
        exit(0);
    }
    return retval;
}
*/