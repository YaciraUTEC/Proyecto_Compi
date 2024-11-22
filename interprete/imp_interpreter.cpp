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
    cout << "Inicio de interpretación de KotlinFile" << endl;
    env.clear();
    kf->accept(this);

    cout << "Fin de interpretación de KotlinFile" << endl;

    return;
}

void ImpInterpreter::visit(KotlinFile* kf) {
        env.add_level();
    fdecs.add_level();
    
    cout << "Procesando declaraciones ..." << endl;
    for (Declaration* d : kf->decl) {
        d->accept(this);
    }

    cout << "Buscando función main ..." << endl;
    if (fdecs.check("main")) {
        FunctionDeclaration* main = fdecs.lookup("main");
        retcall = false;

        cout << "Ejecutando cuerpo de main ..." << endl;
        main->fbody->accept(this);

        if (!retcall && main->returnType == "main") {
            retcall = true;
            retval.type = NOTYPE;
        }
    } else {
        cout << "Error: No se encontro funcion main" << endl;
        exit(0);
    }

    env.remove_level();
    fdecs.remove_level();
}

void ImpInterpreter::visit(Declaration* d) {
    d->accept(this);
    return;
}

void ImpInterpreter::visit(Block* b) {
    cout << "Entranado a bloque" << endl;
    env.add_level();
    b->slist->accept(this);
    env.remove_level();
    cout << "Saliendo de bloque" << endl;
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
    ImpValue v;
    
    // Inicializar con valor por defecto según el tipo
    if (p->variable->type == "Int") {
        v.type = TINT;
        v.int_value = 0;
    } else if (p->variable->type == "Long") {
        v.type = TLONG;
        v.long_value = 0;
    } else if (p->variable->type == "Boolean") {
        v.type = TBOOL;
        v.bool_value = false;
    }
    
    // Si hay expresión, evaluarla
    if (p->expression != nullptr) {
        v = p->expression->accept(this);
    }
    
    env.add_var(p->variable->identifier, v);
    return;
}

void ImpInterpreter::visit(StatementList* s) {
    cout << "Procesando lista de statements ..." << endl;
    list<Statement*>::iterator it;
    for (it = s->statements.begin(); it != s->statements.end(); ++it) {
        cout << "Ejecutando statement ..." << endl;
        (*it)->accept(this);
        if (retcall)
            break;
    }
    return;
}

void ImpInterpreter::visit(AssignmentStatement* s) {
    ImpValue v = s->expression->accept(this);
    if (!env.check(s->identifier)) {
        cout << "Variable " << s->identifier << " no definida" << endl;
        exit(0);
    }
    ImpValue lhs = env.lookup(s->identifier);
    
    // Permitir asignación si los tipos coinciden exactamente
    if (!(lhs.type == v.type || (lhs.type == TLONG && v.type == TINT))) {
        cout << "Type Error en Assign: Tipos de variable " << s->identifier;
        cout << " no coinciden" << endl;
        exit(0);
    }
    
    // Si es necesario, convertir INT a LONG
    if (lhs.type == TLONG && v.type == TINT) {
        v.long_value = (long)v.int_value;
        v.type = TLONG;
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
    cout << "Evaluando IfExpression" << endl; // Debug
    // 1. Evaluar la condición
    ImpValue v = s->condition->accept(this);
    
    cout << "Evaluando condición if: " << v.bool_value << endl; // Debug

    // 2. Verificar que la condición sea booleana
    if (v.type != TBOOL) {
        cout << "Type error en If: esperaba bool en condicional" << endl;
        exit(0);
    }

    // 3. Ejecutar el bloque correspondiente
    if (v.bool_value) {
        cout << "Ejecutando bloque then" << endl; // Debug
        if (s->thenBody != nullptr) {
            env.add_level(); // Agregar nuevo nivel de ambiente
            s->thenBody->accept(this);
            env.remove_level(); // Remover nivel al salir
        }
    } else {
        cout << "Ejecutando bloque else" << endl; // Debug
        if (s->elseBody != nullptr) {
            env.add_level(); // Agregar nuevo nivel de ambiente
            s->elseBody->accept(this);
            env.remove_level(); // Remover nivel al salir
        }
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
    cout << "Evaluando BinaryExpression" << endl; // Debug
    ImpValue result;
    ImpValue v1 = e->left->accept(this);
    ImpValue v2 = e->right->accept(this);

    cout << "Operador izquierdo: " << v1.int_value << endl; // Debug
    cout << "Operador derecho: " << v2.int_value << endl; // Debug

    if ((v1.type != TINT && v1.type != TLONG) || 
        (v2.type != TINT && v2.type != TLONG)) {
        cout << "Error de tipos: operandos en operacion binaria tienen que ser numericos" << endl;
        exit(0);
    }

    // Si alguno es Long, el resultado es Long
    bool isLongOperation = (v1.type == TLONG || v2.type == TLONG);

    if (!isLongOperation) {
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
    } else {
        long lv, lv1, lv2;
        bool bv;
        ImpVType type = NOTYPE;
        lv1 = v1.long_value;
        lv2 = v2.long_value;
        switch (e->op) {
            case ADD_OP:
                lv = lv1 + lv2;
                type = TLONG;
                break;
            case SUB_OP:
                lv = lv1 - lv2;
                type = TLONG;
                break;
            case MUL_OP:
                lv = lv1 * lv2;
                type = TLONG;
                break;
            case DIV_OP:
                lv = lv1 / lv2;
                type = TLONG;
                break;
            case LT_OP:
                bv = (lv1 < lv2) ? 1 : 0;
                type = TBOOL;
                break;
            case LE_OP:
                bv = (lv1 <= lv2) ? 1 : 0;
                type = TBOOL;
                break;
            case EQ_OP:
                bv = (lv1 == lv2) ? 1 : 0;
                type = TBOOL;
                break;
            case NE_OP:
                bv = (lv1 != lv2) ? 1 : 0;
                type = TBOOL;
                break;
            case GT_OP:
                bv = (lv1 > lv2) ? 1 : 0;
                type = TBOOL;
                break;
            case GE_OP:
                bv = (lv1 >= lv2) ? 1 : 0;
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
        if (type == TLONG)
            result.long_value = lv;
        else
            result.bool_value = bv;
        result.type = type;
    }
    return result;
}

ImpValue ImpInterpreter::visit(LiteralExpression* e) {

    ImpValue v;
    switch(e->type) {
        case INTEGER_LITERAL:
            v.type = TINT;
            v.int_value = stoi(e->value);
            break;
        case LONG_LITERAL:
            v.type = TLONG;
            v.long_value = stol(e->value);
            break;
        case BOOLEAN_LITERAL:
            v.type = TBOOL;
            v.bool_value = (e->value == "true");
            break;
        case CHARACTER_LITERAL:
            cout << "Error: Caracteres no soportados" << endl;
            break;
    }
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

ImpValue ImpInterpreter::visit(FunctionCallExpression* e) {
    // -------------------------------------------------------
    // Revisar la implementación de este método
    // -------------------------------------------------------

    // Verificar que la función exista
    if (!fdecs.check(e->identifier)) {
        cout << "Error: Función " << e->identifier << " no definida" << endl;
        exit(0);
    }

    FunctionDeclaration* fd = fdecs.lookup(e->identifier);
    env.add_level();

    // Verificar número de argumentos
    if (fd->parameters.size() != e->arguments.size()) {
        cout << "Error: Número incorrecto de argumentos para " << e->identifier << endl;
        exit(0);
    }

    // Evaluar y asignar argumentos
    auto paramIt = fd->parameters.begin();
    auto argIt = e->arguments.begin();
    for (; paramIt != fd->parameters.end(); ++paramIt, ++argIt) {
        ImpValue v = (*argIt)->accept(this);
        env.add_var((*paramIt)->parameter->identifier, v);
    }

    // Ejecutar cuerpo de la función
    retcall = false;
    fd->fbody->accept(this);
    
    if (!retcall) {
        cout << "Error: Función " << e->identifier << " no ejecutó return" << endl;
        exit(0);
    }

    env.remove_level();
    return retval;    
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