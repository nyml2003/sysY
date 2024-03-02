%skeleton "lalr1.cc"

%code requires {
    #include "Program.hpp"
    #include "Int32.hpp"
    #include "UnaryExpression.hpp"
    #include "BinaryExpression.hpp"
    #include "Definition.hpp"
    #include "Declaration.hpp"
    #include "Statement.hpp"
    #include "FunctionDefinition.hpp"
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

%type < ProgramPtr > Program
%type < BlockItemList > CompUnit CompUnitList
%type < FuncDefPtr > FuncDef
%type < BlockPtr > Block
%type < StmtPtr > Stmt
%type < Int32Ptr > Number
%type < IdentPtr > Ident LVal
%type < BlockItemList > VarDecl ConstDecl Decl  BlockItem BlockItemList VarDefList ConstDefList
%type < DefPtr > VarDef ConstDef
%type < ExprPtr > ConstInitVal ConstExp InitVal
%type < ExprPtr > Exp PrimaryExp UnaryExp MulExp AddExp RelExp EqExp LAndExp LOrExp
%type < Operator > UNARYOP
%token<std::string> IDENT
%token RETURN CONST
%token TYPE_INT TYPE_VOID TYPE_FLOAT
%token<std::int32_t> INT_CONST
%token LPAREN RPAREN LBRACE RBRACE SEMICOLON COMMA PLUS MINUS STAR SLASH PERCENT AND OR EQ NE LT GT LE GE NOT ASSIGN 


%%
%start Program;


Program: CompUnitList { $$ = std::make_unique< Program >(); $$->block->append(std::move($1)); driver.program = std::move($$); }
       ;

CompUnitList: CompUnit { $$ = std::vector< std::unique_ptr< BlockItem > >(); $$.insert($$.end(), std::make_move_iterator($1.begin()), std::make_move_iterator($1.end())); }
             | CompUnitList CompUnit { $$ = std::move($1); $$.insert($$.end(), std::make_move_iterator($2.begin()), std::make_move_iterator($2.end())); }
             ;

CompUnit: FuncDef {
                $$ = std::vector< std::unique_ptr< BlockItem > >();
                $$.push_back(std::move($1));
            }
        | Decl {
             $$ = std::move($1);
            }
        ;

FuncDef: TYPE_INT Ident LPAREN RPAREN Block {
                $$ = std::make_unique< FuncDef >(Type::INT, std::move($2), std::move($5));
            }
       | TYPE_VOID Ident LPAREN RPAREN Block {
                $$ = std::make_unique< FuncDef >(Type::VOID, std::move($2), std::move($5));
            }
        | TYPE_FLOAT Ident LPAREN RPAREN Block {
                $$ = std::make_unique< FuncDef >(Type::FLOAT, std::move($2), std::move($5));
            }
        ;

Block: LBRACE RBRACE { $$ = std::make_unique< Block >(); }
     | LBRACE BlockItemList RBRACE { $$ = std::make_unique< Block >(std::move($2)); }
    ;

BlockItem: Stmt { $$ = std::vector< std::unique_ptr< BlockItem > >(); $$.push_back(std::move($1)); }
         | Decl { $$ = std::move($1); }
         ;

BlockItemList: BlockItem { $$ = std::move($1); }
             | BlockItemList BlockItem { $$ = std::move($1); $$.insert($$.end(), std::make_move_iterator($2.begin()), std::make_move_iterator($2.end())); }
             ;

Stmt: LVal ASSIGN Exp SEMICOLON { $$ = std::make_unique< AssignStmt >(std::move($1), std::move($3)); std::cout<<"detect a assignment"<<std::endl;}
    | RETURN Exp SEMICOLON { $$ = std::make_unique< ReturnStmt >(std::move($2)); }
    ;

LVal: Ident { $$ = std::move($1); }

Number: INT_CONST { $$ = std::make_unique< Int32 >($1); }
      ;

Ident: IDENT { $$ = std::make_unique< Ident >($1); }
      ;

Exp: LOrExp { $$ = std::move($1); }
    ;

UNARYOP: MINUS { $$ = Operator::MINUS; }
       | NOT { $$ =Operator::NOT; }
       | PLUS { $$ = Operator::PLUS; }
       ;

UnaryExp: PrimaryExp { $$ = std::move($1); }
        | UNARYOP UnaryExp {
            switch($1){
                case Operator::MINUS:
                    $$ = std::make_unique< UnaryExpr >(Operator::MINUS, std::move($2));
                    break;
                case Operator::NOT:
                    $$ = std::make_unique< UnaryExpr >(Operator::NOT, std::move($2));
                    break;
                case Operator::PLUS:
                    $$ = std::move($2);
                    break;
                default:
                    break;
            }
        }
        ;

PrimaryExp: Number { $$ = std::move($1); }
          | LPAREN Exp RPAREN { $$ = std::move($2); }
          | LVal { $$ = std::move($1); }
          ;

MulExp: UnaryExp { $$ = std::move($1); }
        | MulExp STAR UnaryExp { $$ = std::make_unique< BinExpr >(std::move($1), Operator::MUL, std::move($3)); }
        | MulExp SLASH UnaryExp { $$ = std::make_unique< BinExpr >(std::move($1), Operator::DIV, std::move($3)); }
        | MulExp PERCENT UnaryExp { $$ = std::make_unique< BinExpr >(std::move($1), Operator::MOD, std::move($3)); }
        ;

AddExp: MulExp { $$ = std::move($1); }
        | AddExp PLUS MulExp { $$ = std::make_unique< BinExpr >(std::move($1),  Operator::ADD, std::move($3)); }
        | AddExp MINUS MulExp { $$ = std::make_unique< BinExpr >(std::move($1), Operator::SUB, std::move($3)); }
        ;

RelExp: AddExp { $$ = std::move($1); }
        | RelExp LT AddExp { $$ = std::make_unique< BinExpr >(std::move($1), Operator::LT, std::move($3)); }
        | RelExp GT AddExp { $$ = std::make_unique< BinExpr >(std::move($1), Operator::GT, std::move($3)); }
        | RelExp LE AddExp { $$ = std::make_unique< BinExpr >(std::move($1), Operator::LE, std::move($3)); }
        | RelExp GE AddExp { $$ = std::make_unique< BinExpr >(std::move($1), Operator::GE, std::move($3)); }
        ;

EqExp: RelExp { $$ = std::move($1); }
        | EqExp EQ RelExp { $$ = std::make_unique< BinExpr >(std::move($1),Operator::EQ, std::move($3)); }
        | EqExp NE RelExp { $$ = std::make_unique< BinExpr >(std::move($1),Operator::NE, std::move($3)); }
        ;


LAndExp: EqExp { $$ = std::move($1); }
        | LAndExp AND EqExp { $$ = std::make_unique< BinExpr >(std::move($1), Operator::AND, std::move($3)); }
        ;

LOrExp: LAndExp { $$ = std::move($1); }
        | LOrExp OR LAndExp { $$ = std::make_unique< BinExpr >(std::move($1), Operator::OR, std::move($3)); }
        ;
        

Decl: VarDecl { $$ = std::move($1); }
    | ConstDecl { $$ = std::move($1); }
    ;

ConstDecl: CONST TYPE_INT ConstDefList SEMICOLON {
    $$ = std::vector< std::unique_ptr< BlockItem > >();
    for(auto &def : $3){
        auto ident = std::make_unique< Ident >(dynamic_cast< Def* >(def.get())->ident->name);
        ident->begin = def->begin;
        ident->end = def->end;
        auto decl = std::make_unique< Decl >(Type::INT, std::move(ident));
        $$.push_back(std::move(decl));
        if (dynamic_cast< Def* >(def.get())->expr->getType() != Type::NONE){
            $$.push_back(std::move(def));
        }
    }
}
         | CONST TYPE_FLOAT ConstDefList SEMICOLON { 
    $$ = std::vector< std::unique_ptr< BlockItem > >();
    for(auto &def : $3){
        auto ident = std::make_unique< Ident >(dynamic_cast< Def* >(def.get())->ident->name);
        ident->begin = def->begin;
        ident->end = def->end;
        auto decl = std::make_unique< Decl >(Type::FLOAT, std::move(ident));
        $$.push_back(std::move(decl));
        if (dynamic_cast< Def* >(def.get())->expr->getType() != Type::NONE){
            $$.push_back(std::move(def));
        }
    }
}
         ;

ConstDefList: ConstDef { $$ = std::vector< std::unique_ptr< BlockItem > >(); $$.push_back(std::move($1)); std::cout<<"detect a const deflist"<<std::endl;}
            | ConstDefList COMMA ConstDef { $$ = std::move($1); $$.push_back(std::move($3)); }
            ;

ConstDef: Ident ASSIGN ConstInitVal { $$ = std::make_unique< Def >(std::move($1), std::move($3)); }
        ;

ConstInitVal: ConstExp { $$ = std::move($1); }
            ;

ConstExp: Exp { $$ = std::move($1); }
        ;

VarDecl: TYPE_INT VarDefList SEMICOLON {
    $$ = std::vector< std::unique_ptr< BlockItem > >();
    for(auto &def : $2){
        auto ident = std::make_unique< Ident >(dynamic_cast< Def* >(def.get())->ident->name);
        ident->begin = def->begin;
        ident->end = def->end;
        auto decl = std::make_unique< Decl >(Type::INT, std::move(ident));
        $$.push_back(std::move(decl));
        if (dynamic_cast< Def* >(def.get())->expr->getType() != Type::NONE){
            $$.push_back(std::move(def));
        }
    }
}
        | TYPE_FLOAT VarDefList SEMICOLON {
    $$ = std::vector< std::unique_ptr< BlockItem > >();
    for(auto &def : $2){
        auto ident = std::make_unique< Ident >(dynamic_cast< Def* >(def.get())->ident->name);
        ident->begin = def->begin;
        ident->end = def->end;
        auto decl = std::make_unique< Decl >(Type::FLOAT, std::move(ident));
        $$.push_back(std::move(decl));
        if (dynamic_cast< Def* >(def.get())->expr->getType() != Type::NONE){
            $$.push_back(std::move(def));
        }
    }
}
        ;

VarDefList: VarDef { $$ = std::vector< std::unique_ptr< BlockItem > >(); $$.push_back(std::move($1)); }
            | VarDefList COMMA VarDef { $$ = std::move($1); $$.push_back(std::move($3)); }
            ;


VarDef: Ident { auto nullExpr = std::make_unique< NullExpr > (); $$ = std::make_unique< Def >(std::move($1), std::move(nullExpr)); }
        | Ident ASSIGN InitVal { $$ = std::make_unique< Def >(std::move($1), std::move($3)); std::cout<<"detect a vardef"<<std::endl;}
        ;

InitVal: Exp { $$ = std::move($1); }
         ;






%%

void yy::parser::error(const location_type& l, const std::string& m) {
     driver.error(l, m);
}

