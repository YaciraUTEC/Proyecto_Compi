#include "exp.h"

// Implementaciones base
Declaration::~Declaration() {}
Expression::~Expression() {}
Statement::~Statement() {}

// KotlinFile
KotlinFile::KotlinFile() {}
void KotlinFile::add(Declaration* declaration) {
    this->decl.push_back(declaration);
}
void KotlinFile::print() {
    for (auto d : decl) {
        d->print();
        cout<<endl;
    }
}

// FunctionDeclaration
FunctionDeclaration::FunctionDeclaration(string id, list<FunctionValueParameter*> params,
    string rtype, Block* body)
    : identifier(id), parameters(params), returnType(rtype), fbody(body) {
}
FunctionDeclaration::~FunctionDeclaration() {
    delete fbody;
    for (auto param : parameters) {
        delete param;
    }
}
void FunctionDeclaration::print() {
    cout << "fun " << this->identifier << "(";
    // hagamoslo con punteros para que no coloque coma en el último
    for (auto param = this->parameters.begin(); param != this->parameters.end(); param++) {
        (*param)->print();
        if (next(param) != this->parameters.end()) {
            cout << ", ";
        }
    }
    cout << ")";
    if (this->returnType != "") {
        cout << ": ";
        cout<<returnType;
    }
    this->fbody->print();
    cout << endl;
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
    cout << ptype << " "; // con un tab de espacio
    variable->print();
    if (expression != nullptr) {
        cout << " = ";
        expression->print();
    }
    // cout << ";" << endl; (opcional)
    cout<<endl;
}

// Parameter
Parameter::Parameter(string id, string type) : identifier(id), type(type) {}
Parameter::~Parameter() {}

void Parameter::print() {
    cout << identifier << ": ";
    cout << type;
}

// FunctionValueParameter
FunctionValueParameter::FunctionValueParameter(Parameter* param)
    : parameter(param) {
}

FunctionValueParameter::~FunctionValueParameter() {
    delete parameter;
}



void FunctionValueParameter::print() {
    parameter->print();
}

// VariableDeclaration
VariableDeclaration::VariableDeclaration(string id, string type)
    : identifier(id), type(type) {
}
VariableDeclaration::~VariableDeclaration() {}

void VariableDeclaration::print() {
    cout << identifier;
    if (type != "") {
        cout << ": ";
        cout << type;
    }
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

void Block::print() {
    cout << "{" << endl;
    if (slist != nullptr) {
        slist->print();
    }
    cout << "} ";
}

// Statements
DeclarationStatement::DeclarationStatement(Declaration* decl) : declaration(decl) {}

DeclarationStatement::~DeclarationStatement() {
    delete declaration;
}

void DeclarationStatement::print() {
    declaration->print();
}

AssignmentStatement::AssignmentStatement(string id, Expression* expr)
    : identifier(id), expression(expr) {
}

AssignmentStatement::~AssignmentStatement() {
    delete expression;
}

void AssignmentStatement::print() {
    cout <<identifier << " = ";
    expression->print();
    // cout << ";" << endl; (opcional)
    cout<<endl;
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

ExpressionStatement::ExpressionStatement(Expression* expr) : expression(expr) {}

void ExpressionStatement::print() {
    expression->print();
    cout<<endl;
}

ExpressionStatement::~ExpressionStatement() {
    delete expression;
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
    cout<<endl;
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
    cout<<endl;
}

// Expressions
BinaryExpression::BinaryExpression(Expression* lhs, Expression* rhs, BinaryOp op)
    : left(lhs), right(rhs), op(op) {
}

BinaryExpression::~BinaryExpression() {
    delete left;
    delete right;
}

IdentifierExpression::IdentifierExpression(string id) : identifier(id) {}

IdentifierExpression::~IdentifierExpression() {}

void IdentifierExpression::print() {
    cout << identifier;
}

// LiteralExpression
LiteralExpression::LiteralExpression(LiteralType type, string value)
    : type(type), value(value) {
}


LiteralExpression::~LiteralExpression() {}

void LiteralExpression::print() {
    cout << value;
}

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
}

FunctionCallExpression::FunctionCallExpression(string id, list<Expression *> args) : identifier(id), arguments(args) {}
FunctionCallExpression::~FunctionCallExpression() {
    for (auto arg : arguments) {
        delete arg;
    }
}
void FunctionCallExpression::print() {
    cout << this->identifier << "(";
    for (auto arg = arguments.begin(); arg != arguments.end(); arg++) {
        (*arg)->print();
        if (next(arg) != arguments.end()) {
            cout << ", ";
        }
    }
    cout << ")";
}

// JumpExpression
JumpExpression::JumpExpression(Expression* ret): returnExpression(ret) {}
JumpExpression::~JumpExpression() {
    delete returnExpression;
}
void JumpExpression::print() {
    cout << "return ";
    returnExpression->print();
}

// StringLiteral
StringLiteral::StringLiteral(string value) : value(value) {}

StringLiteral::~StringLiteral() {}

void StringLiteral::print() {
    cout << value;
}

void BinaryExpression::print() {
    left->print();
    cout << " " << binopToChar(op) << " ";
    right->print();
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