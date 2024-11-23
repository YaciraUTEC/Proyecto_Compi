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

// void IfExpression::accept2(ImpValueVisitor* v) {
//     return v->visit(this);
// }

ImpValue IfExpression::accept(ImpValueVisitor* v) {
    return v->visit(this);
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
        env.add_level();
    fdecs.add_level();
    
    for (Declaration* d : kf->decl) {
        d->accept(this);
    }

    if (fdecs.check("main")) {
        FunctionDeclaration* main = fdecs.lookup("main");
        retcall = false;

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
    env.add_level();
    b->slist->accept(this);
    env.remove_level();

    return;
}

void ImpInterpreter::visit(FunctionDeclaration* fd) {
    fdecs.add_var(fd->identifier, fd);
    return;
}

void ImpInterpreter::visit(PropertyDeclaration* p) {
    ImpValue v;

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

ImpValue ImpInterpreter::visit(IfExpression* s) {

    // Evaluar la condición
    ImpValue condition = s->condition->accept(this);

    // Verificar que la condición sea booleana
    if (condition.type != TBOOL) {
        cout << "Error: La condición del if debe ser booleana" << endl;
        exit(0);
    }

    ImpValue result;
    result.type = NOTYPE;  // Inicializar resultado

    // Ejecutar el bloque correspondiente según la condición
    env.add_level();
    
    if (condition.bool_value) {
        if (s->thenBody != nullptr) {
            s->thenBody->accept(this);
        }
    } else {
        if (s->elseBody != nullptr) {
            s->elseBody->accept(this);
        }
    }
    
    env.remove_level();
    return result;
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
    env.add_level();
    // Obtener los valores inicial y final directamente de la expresión binaria
    BinaryExpression* rangeExp = dynamic_cast<BinaryExpression*>(s->expression);
    if (!rangeExp || rangeExp->op != RANGE_OP) {
        cout << "Error: Se esperaba una expresión de rango (..)" << endl;
        exit(0);
    }

    ImpValue start = rangeExp->left->accept(this);
    ImpValue end = rangeExp->right->accept(this);

    // Verificar que ambos valores sean enteros
    if (start.type != TINT || end.type != TINT) {
        cout << "Error: Los límites del rango deben ser enteros" << endl;
        exit(0);
    }

    // Iterar sobre el rango
    for (int i = start.int_value; i <= end.int_value; i++) {
        // Crear y asignar la variable de iteración
        ImpValue iteratorValue;
        iteratorValue.type = TINT;
        iteratorValue.int_value = i;
        env.add_var(s->variable->identifier, iteratorValue);

        // Ejecutar el cuerpo del for
        s->fbody->accept(this);

        // Si hay un return, salir del bucle
        if (retcall) break;
    }

    env.remove_level();
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
    ImpValue result;
    ImpValue v1 = e->left->accept(this);
    ImpValue v2 = e->right->accept(this);

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

ImpValue ImpInterpreter::visit(IdentifierExpression* e) {
    if (env.check(e->identifier))
        return env.lookup(e->identifier);
    else {
        cout << "Variable indefinida: " << e->identifier << endl;
        exit(0);
    }
}

ImpValue ImpInterpreter::visit(FunctionCallExpression* e) {

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
