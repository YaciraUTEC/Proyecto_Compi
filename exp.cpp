#include "exp.h"
#include "visitor.h"

// Implementaciones base
Declaration::~Declaration() {}
Expression::~Expression() {}
Statement::~Statement() {}

// Type
Type::Type(string name) : name(name) {}
Type::~Type() {}

void Type::print() {
    cout << name;
}

/*
void Type::accept(Visitor* visitor) {
    visitor->visit(this);
}
*/

// KotlinFile

KotlinFile::KotlinFile() {}

void KotlinFile::add(Declaration* declaration) {
    this->decl.push_back(declaration);
}

void KotlinFile::print() {
    for (auto d : decl) {
        d->print();
    }
}

// FunctionDeclaration
FunctionDeclaration::FunctionDeclaration(string id, list<FunctionValueParameter*> params,
    Type* rtype, Block* body)
    : identifier(id), parameters(params), returnType(rtype), fbody(body) {
}

FunctionDeclaration::~FunctionDeclaration() {
    delete returnType;
    delete fbody;
    for (auto param : parameters) {
        delete param;
    }
}

void FunctionDeclaration::print() {
    cout << "fun " << this->identifier << "(";
    for (auto param : this->parameters) {
        param->print();
    }
    cout << ")";
    if (this->returnType != nullptr) {
        cout << ": ";
        this->returnType->print();
    }
    this->fbody->print();
}

int FunctionDeclaration::eval() {
    // Implementación de evaluación
    return 0;
}



// PropertyDeclaration
PropertyDeclaration::PropertyDeclaration(string ptype, VariableDeclaration* var, Expression* expr)
    : ptype(ptype), variable(var), expression(expr) {
}

PropertyDeclaration::~PropertyDeclaration() {
    delete variable;
    delete expression;
}

void PropertyDeclaration::print() {
    cout << ptype << " ";
    variable->print();
    if (expression != nullptr) {
        cout << " = ";
        expression->print();
    }
    cout << ";" << endl;
}

int PropertyDeclaration::eval() {
    // Implementación de evaluación
    return 0;
}

// Parameter
Parameter::Parameter(string id, Type* type) : identifier(id), type(type) {}
Parameter::~Parameter() {
    delete type;
}

void Parameter::print() {
    cout << identifier << ": ";
    type->print();
}

// FunctionValueParameter
FunctionValueParameter::FunctionValueParameter(Parameter* param, Expression* defaultValue)
    : parameter(param), defaultValue(defaultValue) {
}

FunctionValueParameter::~FunctionValueParameter() {
    delete parameter;
    delete defaultValue;
}



void FunctionValueParameter::print() {
    parameter->print();
    if (defaultValue != nullptr) {
        cout << " = ";
        defaultValue->print();
    }
}

// VariableDeclaration
VariableDeclaration::VariableDeclaration(string id, Type* type)
    : identifier(id), type(type) {
}
VariableDeclaration::~VariableDeclaration() {
    delete type;
}

void VariableDeclaration::print() {
    cout << identifier << ": ";
    type->print();
}

// Statementlist
StatementList::StatementList() {}
void StatementList::add(Statement* stmt) {
    statements.push_back(stmt);
}

void StatementList::print() {
    for (auto stmt : statements) {
        stmt->print();
    }
}

StatementList::~StatementList() {
    for (auto stmt : statements) {
        delete stmt;
    }
}

// Block
Block::Block(StatementList* stms) : slist(stms) {}

Block::~Block() {
    delete slist;
}

/*
void Block::accept(Visitor* visitor) {
    visitor->visit(this);
}
*/

void Block::print() {
    cout << "{" << endl;
    if (slist != nullptr) {
        slist->print();
    }
    cout << "}" << endl;
}

// Statements
DeclarationStatement::DeclarationStatement(Declaration* decl) : declaration(decl) {}

DeclarationStatement::~DeclarationStatement() {
    delete declaration;
}

void DeclarationStatement::print() {
    declaration->print();
}

int DeclarationStatement::eval() {
    return declaration->eval();
}

AssignmentStatement::AssignmentStatement(string id, Expression* expr)
    : identifier(id), expression(expr) {
}

AssignmentStatement::~AssignmentStatement() {
    delete expression;
}

void AssignmentStatement::print() {
    cout << identifier << " = ";
    expression->print();
    cout << ";" << endl;
}

int AssignmentStatement::eval() {
    // Implementación de evaluación
    return 0;
}

PrintlnStatement::PrintlnStatement(Expression* expr) : expression(expr) {}

PrintlnStatement::~PrintlnStatement() {
    delete expression;
}

void PrintlnStatement::print() {
    cout << "println(";
    expression->print();
    cout << ")" << endl;
}

int PrintlnStatement::eval() {
    // Implementación de evaluación
    return 0;
}

ExpressionStatement::ExpressionStatement(Expression* expr) : expression(expr) {}

void ExpressionStatement::print() {
    expression->print();
    cout << ";" << endl;
}

ExpressionStatement::~ExpressionStatement() {
    delete expression;
}

int ExpressionStatement::eval() {
    return expression->eval();
}

ForStatement::ForStatement(VariableDeclaration* var, Expression* expr, Block* fbody)
    : variable(var), expression(expr), fbody(fbody) {
}

ForStatement::~ForStatement() {
    delete variable;
    delete expression;
    delete fbody;
}

void ForStatement::print() {
    cout << "for (";
    this->variable->print();
    cout << " in ";
    this->expression->print();
    cout << ")";
    this->fbody->print();
}

int ForStatement::eval() {
    // Implementación de evaluación
    return 0;
}

WhileStatement::WhileStatement(Expression* cond, Block* wbody)
    : condition(cond), wbody(wbody) {
}

WhileStatement::~WhileStatement() {
    delete condition;
    delete wbody;
}

void WhileStatement::print() {
    cout << "while (";
    condition->print();
    cout << ")";
    wbody->print();
}

int WhileStatement::eval() {
    // Implementación de evaluación
    return 0;
}

// Expressions
BinaryExpression::BinaryExpression(Expression* lhs, Expression* rhs, BinaryOp op)
    : left(lhs), right(rhs), op(op) {
}

BinaryExpression::~BinaryExpression() {
    delete left;
    delete right;
}
/*
void BinaryExpression::accept(Visitor* visitor) {
    visitor->visit(this);
}
*/

IdentifierExpression::IdentifierExpression(string id) : identifier(id) {}

IdentifierExpression::~IdentifierExpression() {}

void IdentifierExpression::print() {
    cout << identifier;
}

int IdentifierExpression::eval() {
    return 0; // falta
}

/*
void IdentifierExpression::accept(Visitor* visitor) {
    visitor->visit(this);
}
*/

// LiteralExpression
LiteralExpression::LiteralExpression(LiteralType type, string value)
    : type(type), value(value) {
}


LiteralExpression::~LiteralExpression() {}

void LiteralExpression::print() {
    cout << value;
}

int LiteralExpression::eval() {
    int result;
    switch (type) {
    case BOOLEAN_LITERAL: result = value == "true"; break;
    case INTEGER_LITERAL: result = stoi(value); break;
    case CHARACTER_LITERAL: result = value[0]; break;
    case STRING_LITERAL: result = 0; break;
    default: result = 0;
    }
    return result;
}

/*
void LiteralExpression::accept(Visitor* visitor) {
    visitor->visit(this);
}
*/

// IfExpression
IfExpression::IfExpression(Expression* cond, Block* thenBody, Block* elseBody)
    : condition(cond), thenBody(thenBody), elseBody(elseBody) {
}

IfExpression::~IfExpression() {
    delete condition;
    delete thenBody;
    delete elseBody;
}

void IfExpression::print() {
    cout << "if (";
    condition->print();
    cout << ")";
    thenBody->print();
    if (elseBody != nullptr) {
        cout << "else";
        elseBody->print();
    }
    cout << endl;
}

int IfExpression::eval() {
    // Implementación de evaluación
    return 0;
}
/*
void IfExpression::accept(Visitor* visitor) {
    visitor->visit(this);
}
*/

// StringLiteral
StringLiteral::StringLiteral(string value) : value(value) {}

StringLiteral::~StringLiteral() {}

void StringLiteral::print() {
    cout << value;
}

int StringLiteral::eval() {
    return 0; // falta
}

// void StringLiteral::accept(Visitor* visitor) {
//     visitor->visit(this);
// }

void BinaryExpression::print() {
    left->print();
    cout << " " << binopToChar(op) << " ";
    right->print();
};

int BinaryExpression::eval() {
    int result;
    int v1 = left->eval();
    int v2 = right->eval();
    switch (op) {
    case ADD_OP: result = v1 + v2; break;
    case SUB_OP: result = v1 - v2; break;
    case MUL_OP: result = v1 * v2; break;
    case DIV_OP: result = v1 / v2; break;
    case LT_OP: result = v1 < v2; break;
    case GT_OP: result = v1 > v2; break;
    case LE_OP: result = v1 <= v2; break;
    case GE_OP: result = v1 >= v2; break;
    case EQ_OP: result = v1 == v2; break;
    case NE_OP: result = v1 != v2; break;
    case AND_OP: result = v1 && v2; break;
    case OR_OP: result = v1 || v2; break;
    case RANGE_OP: result = 0; break;
    default:
        cout << "Operador desconocido" << endl;
        result = 0;
    }
    return result;
};



string Expression::binopToChar(BinaryOp op) {
    string  c;
    switch (op) {
    case ADD_OP: c = "+"; break;
    case SUB_OP: c = "-"; break;
    case MUL_OP: c = "*"; break;
    case DIV_OP: c = "/"; break;
    case LT_OP: c = "<"; break;
    case GT_OP: c = ">"; break;
    case LE_OP: c = "<="; break;
    case GE_OP: c = ">="; break;
    case EQ_OP: c = "=="; break;
    case NE_OP: c = "!="; break;
    case AND_OP: c = "&&"; break;
    case OR_OP: c = "||"; break;
    case RANGE_OP: c = ".."; break;
    default: c = "$";
    }
    return c;
}