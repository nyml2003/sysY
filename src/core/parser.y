%skeleton "lalr1.cc"

%code requires {
    #include "AST.hpp"
    namespace Compiler::Core {
        class Driver;
    }
    using Driver = Compiler::Core::Driver;
}

%code {
    #include "Driver.hpp"
}

%language "c++"
%locations
%param { Driver &driver }

%define api.value.type variant
%define api.token.raw
%define api.token.constructor
%define api.token.prefix {TOKEN_}
%define parse.assert

%define parse.error detailed
%define parse.lac full
%define parse.trace

%type < Compiler::AST::NodePtr > CompUnit Decl FuncDef Type Block BlockItem Stmt Exp Cond Lval InitVal Def PrimaryExp Number UnaryExp MulExp AddExp RelExp EqExp LAndExp LOrExp Ident
%type < std::vector< Compiler::AST::NodePtr > > DefList CompUnitContainer BlockItemList InitValList DimList
%type < Compiler::AST::Operator > UNARYOP
%token<std::string> IDENT
%token RETURN CONST
%token TYPE_INT TYPE_VOID TYPE_FLOAT
%token<std::int32_t> INT_CONST
%token LPAREN RPAREN LBRACE RBRACE SEMICOLON COMMA LBRACKET RBRACKET
%token PLUS MINUS STAR SLASH PERCENT AND OR EQ NE LT GT LE GE NOT ASSIGN CONTINUE BREAK IF ELSE WHILE


%%
%start CompUnit;

CompUnit:
    CompUnitContainer { auto compUnit = std::make_unique< Compiler::AST::CompUnit >(); compUnit->attach(std::move($1)); driver.result = std::move(compUnit); }
    ;

CompUnitContainer: 
      Decl { $$ = std::vector< Compiler::AST::NodePtr >(); $$.push_back(std::move($1)); }
    | FuncDef { $$ = std::vector< Compiler::AST::NodePtr >(); $$.push_back(std::move($1)); }
    | CompUnitContainer Decl { $1.push_back(std::move($2)); $$ = std::move($1); }
    | CompUnitContainer FuncDef { $1.push_back(std::move($2)); $$ = std::move($1); }
    ;

Decl : 
    Type DefList SEMICOLON { 
        $$ = std::make_unique< Compiler::AST::Decl >(std::move($1), std::move($2));
    }
    | CONST Decl {
        dynamic_cast< Compiler::AST::Decl* >($2.get())->addDecorator(Compiler::AST::Decorator::CONSTANT);
        $$ = std::move($2);
    }
    ;

Type:
    TYPE_INT { $$ = std::make_unique< Compiler::AST::Type >(Compiler::AST::InnerType::INT); }
    | TYPE_FLOAT { $$ = std::make_unique< Compiler::AST::Type >(Compiler::AST::InnerType::FLOAT); }
    | TYPE_VOID { $$ = std::make_unique< Compiler::AST::Type >(Compiler::AST::InnerType::VOID); }
    ;

Def:
    Lval ASSIGN InitVal { $$ = std::make_unique< Compiler::AST::Def >(std::move($1), std::move($3)); }
    | Lval { $$ = std::move($1); }
    ;

InitVal:
    Exp { $$ = std::make_unique< Compiler::AST::InitVal >(std::move($1)); }
    | LBRACE InitValList RBRACE { $$ = std::make_unique< Compiler::AST::InitVal >(std::move($2)); }
    ;

InitValList:
    InitVal { $$ = std::vector< Compiler::AST::NodePtr >(); $$.push_back(std::move($1)); }
    | InitValList COMMA InitVal { $1.push_back(std::move($3)); $$ = std::move($1); }
    ;

DefList:
    Def { $$ = std::vector< Compiler::AST::NodePtr >(); $$.push_back(std::move($1)); }
    | DefList COMMA Def { $1.push_back(std::move($3)); $$ = std::move($1); }
    ;

FuncDef:
    Type Ident LPAREN RPAREN Block {
        $$ = std::make_unique< Compiler::AST::FuncDef >(std::move($1), std::move($2), std::move($5));
    }
    ;

Ident :
    IDENT { $$ = std::make_unique< Compiler::AST::Ident >($1); }
    ;

Block:
    LBRACE RBRACE { $$ = std::make_unique< Compiler::AST::Block >(); }
    | LBRACE BlockItemList RBRACE { $$ = std::make_unique< Compiler::AST::Block >(std::move($2)); }
    ;

BlockItemList:
    BlockItem { $$ = std::vector< Compiler::AST::NodePtr >(); $$.push_back(std::move($1)); }
    | BlockItemList BlockItem { $1.push_back(std::move($2)); $$ = std::move($1); }
    ;

BlockItem:
    Decl { $$ = std::move($1); }
    | Stmt { $$ = std::move($1); }
    ;

Stmt:
    Lval ASSIGN Exp SEMICOLON { $$ = std::make_unique< Compiler::AST::AssignStmt >(std::move($1), std::move($3)); }
    | Exp SEMICOLON { $$ = std::make_unique< Compiler::AST::ExpStmt >(std::move($1)); }
    | IF LPAREN Cond RPAREN Stmt ELSE Stmt { $$ = std::make_unique< Compiler::AST::IfStmt >(std::move($3), std::move($5), std::move($7)); }
    | IF LPAREN Cond RPAREN Stmt { $$ = std::make_unique< Compiler::AST::IfStmt >(std::move($3), std::move($5)); }
    | WHILE LPAREN Cond RPAREN Stmt { $$ = std::make_unique< Compiler::AST::WhileStmt >(std::move($3), std::move($5)); }
    | RETURN Exp SEMICOLON { $$ = std::make_unique< Compiler::AST::ReturnStmt >(std::move($2)); }
    | BREAK SEMICOLON { $$ = std::make_unique< Compiler::AST::BreakStmt >(); }
    | CONTINUE SEMICOLON { $$ = std::make_unique< Compiler::AST::ContinueStmt >(); }
    | Block { $$ = std::move($1); }
    ;

Exp:
    AddExp { $$ = std::move($1); }
    ;

Cond:
    LOrExp { $$ = std::move($1); }
    ;

Lval: 
    Ident { $$ = std::make_unique< Compiler::AST::Lval >(std::move($1)); }
    | Ident DimList { $$ = std::make_unique< Compiler::AST::Lval >(std::move($1), std::move($2)); }
    ;

DimList:
      LBRACKET RBRACKET { $$ = std::vector< Compiler::AST::NodePtr >(); $$.push_back(std::make_unique< Compiler::AST::Int32 >(0)); }
    | LBRACKET Exp RBRACKET { $$ = std::vector< Compiler::AST::NodePtr >(); $$.push_back(std::move($2)); }
    | DimList LBRACKET RBRACKET { $1.push_back(std::make_unique< Compiler::AST::Int32 >(0)); $$ = std::move($1); }
    | DimList LBRACKET Exp RBRACKET { $1.push_back(std::move($3)); $$ = std::move($1); }
    ;

PrimaryExp:
    Lval { $$ = std::move($1); }
    | LPAREN Exp RPAREN { $$ = std::move($2); }
    | Number { $$ = std::move($1); }
    ;

Number:
    INT_CONST { $$ = std::make_unique< Compiler::AST::Int32 >($1); }
    ;

UNARYOP:
    MINUS { $$ = Compiler::AST::Operator::MINUS; }
    | NOT { $$ = Compiler::AST::Operator::NOT; }
    | PLUS { $$ = Compiler::AST::Operator::PLUS; }
    ;

UnaryExp:
    PrimaryExp { $$ = std::move($1); }
    | UNARYOP UnaryExp { $$ = std::make_unique< Compiler::AST::UnaryExp >($1, std::move($2)); }
    ;

MulExp:
    UnaryExp { $$ = std::move($1); }
    | MulExp STAR UnaryExp { $$ = std::make_unique< Compiler::AST::BinaryExp >(std::move($1), Compiler::AST::Operator::MUL, std::move($3)); }
    | MulExp SLASH UnaryExp { $$ = std::make_unique< Compiler::AST::BinaryExp >(std::move($1), Compiler::AST::Operator::DIV, std::move($3)); }
    | MulExp PERCENT UnaryExp { $$ = std::make_unique< Compiler::AST::BinaryExp >(std::move($1), Compiler::AST::Operator::MOD, std::move($3)); }
    ;

AddExp:
    MulExp { $$ = std::move($1); }
    | AddExp PLUS MulExp { $$ = std::make_unique< Compiler::AST::BinaryExp >(std::move($1), Compiler::AST::Operator::PLUS, std::move($3)); }
    | AddExp MINUS MulExp { $$ = std::make_unique< Compiler::AST::BinaryExp >(std::move($1), Compiler::AST::Operator::MINUS, std::move($3)); }
    ;

RelExp:
    AddExp { $$ = std::move($1); }
    | RelExp LT AddExp { $$ = std::make_unique< Compiler::AST::BinaryExp >(std::move($1), Compiler::AST::Operator::LT, std::move($3)); }
    | RelExp GT AddExp { $$ = std::make_unique< Compiler::AST::BinaryExp >(std::move($1), Compiler::AST::Operator::GT, std::move($3)); }
    | RelExp LE AddExp { $$ = std::make_unique< Compiler::AST::BinaryExp >(std::move($1), Compiler::AST::Operator::LE, std::move($3)); }
    | RelExp GE AddExp { $$ = std::make_unique< Compiler::AST::BinaryExp >(std::move($1), Compiler::AST::Operator::GE, std::move($3)); }
    ;

EqExp:
    RelExp { $$ = std::move($1); }
    | EqExp EQ RelExp { $$ = std::make_unique< Compiler::AST::BinaryExp >(std::move($1), Compiler::AST::Operator::EQ, std::move($3)); }
    | EqExp NE RelExp { $$ = std::make_unique< Compiler::AST::BinaryExp >(std::move($1), Compiler::AST::Operator::NE, std::move($3)); }
    ;

LAndExp:
    EqExp { $$ = std::move($1); }
    | LAndExp AND EqExp { $$ = std::make_unique< Compiler::AST::BinaryExp >(std::move($1), Compiler::AST::Operator::AND, std::move($3)); }
    ;

LOrExp:
    LAndExp { $$ = std::move($1); }
    | LOrExp OR LAndExp { $$ = std::make_unique< Compiler::AST::BinaryExp >(std::move($1), Compiler::AST::Operator::OR, std::move($3)); }
    ;

%%

void yy::parser::error(const location_type& l, const std::string& m) {
     driver.error(l, m);
}

