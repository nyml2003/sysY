%skeleton "lalr1.cc"

%code requires {
  #include <memory>
  #include <string>
  #include "Header.hpp"
  namespace Compiler {
    class Driver;
  }
}

%code {
    #include "Driver.hpp"
}

%language "c++"
%locations
%param { Compiler::Driver &driver }

%define api.value.type variant
%define api.token.raw
%define api.token.constructor
%define api.token.prefix {TOKEN_}
%define parse.assert

%define parse.error detailed
%define parse.lac full
%define parse.trace


%type < std::unique_ptr<Compiler::AbstractSyntaxTree::CompilationUnit> > CompUnit
%type < std::unique_ptr<Compiler::AbstractSyntaxTree::FunctionDefinition> > FuncDef
%type < std::unique_ptr<Compiler::AbstractSyntaxTree::Type> > FuncType
%type < std::unique_ptr<Compiler::AbstractSyntaxTree::Block> > Block
%type < std::unique_ptr<Compiler::AbstractSyntaxTree::Statement> > Stmt
%type < std::unique_ptr<Compiler::AbstractSyntaxTree::Expression::Int32> > Number
%type < std::unique_ptr<Compiler::AbstractSyntaxTree::Expression::Identifier> > Ident
%type < std::unique_ptr<Compiler::AbstractSyntaxTree::Expression::IExpression> > Exp PrimaryExp UnaryExp MulExp AddExp RelExp EqExp LAndExp LOrExp
%type < Compiler::AbstractSyntaxTree::Expression::Operator > UNARYOP
%token<std::string> IDENT
%token<std::string> KEYWORD_RETURN
%token<std::string> TYPE_INT
%token<std::int32_t> INT_CONST
%token LPAREN RPAREN LBRACE RBRACE SEMICOLON PLUS MINUS STAR SLASH PERCENT AND OR EQ NE LT GT LE GE NOT ASSIGN


%%
%start CompUnit;
CompUnit: FuncDef {
              driver.result = std::make_unique<Compiler::AbstractSyntaxTree::CompilationUnit>(std::move($1));
            }
        ;

FuncDef: FuncType Ident LPAREN RPAREN Block {
                $$ = std::make_unique<Compiler::AbstractSyntaxTree::FunctionDefinition>(std::move($1), std::move($2), std::move($5));
            }
       ;

FuncType: TYPE_INT {
                $$ = std::make_unique<Compiler::AbstractSyntaxTree::Type>("int");
            }
        ;

Block: LBRACE Stmt RBRACE { $$ = std::make_unique<Compiler::AbstractSyntaxTree::Block>(std::move($2)); }
     ;

Stmt: KEYWORD_RETURN Exp SEMICOLON { $$ = std::make_unique<Compiler::AbstractSyntaxTree::Statement>(std::move($2));}
    ;

Number: INT_CONST { $$ = std::make_unique<Compiler::AbstractSyntaxTree::Expression::Int32>($1); }
      ;

Ident: IDENT { $$ = std::make_unique<Compiler::AbstractSyntaxTree::Expression::Identifier>($1); }
      ;

Exp: LOrExp { $$ = std::move($1); }
    ;

UNARYOP: MINUS { $$ = Compiler::AbstractSyntaxTree::Expression::Operator::MINUS; }
       | NOT { $$ = Compiler::AbstractSyntaxTree::Expression::Operator::NOT; }
       | PLUS { $$ = Compiler::AbstractSyntaxTree::Expression::Operator::PLUS; }
       ;

UnaryExp: PrimaryExp { $$ = std::move($1); }
        | UNARYOP UnaryExp {
            switch($1){
                case Compiler::AbstractSyntaxTree::Expression::Operator::MINUS:
                    $$ = std::make_unique<Compiler::AbstractSyntaxTree::Expression::UnaryExpression>(std::move($2), Compiler::AbstractSyntaxTree::Expression::Operator::MINUS);
                    break;
                case Compiler::AbstractSyntaxTree::Expression::Operator::NOT:
                    $$ = std::make_unique<Compiler::AbstractSyntaxTree::Expression::UnaryExpression>(std::move($2), Compiler::AbstractSyntaxTree::Expression::Operator::NOT);
                    break;
                case Compiler::AbstractSyntaxTree::Expression::Operator::PLUS:
                    $$ = std::move($2);
                    break;
                default:
                    break;
            }
        }
        | Ident { $$ = std::move($1); }
        ;

PrimaryExp: Number { $$ = std::move($1); }
          | LPAREN Exp RPAREN { $$ = std::move($2); }
          ;

MulExp: UnaryExp { $$ = std::move($1); }
        | MulExp STAR UnaryExp { $$ = std::make_unique<Compiler::AbstractSyntaxTree::Expression::BinaryExpression>(std::move($1), std::move($3), Compiler::AbstractSyntaxTree::Expression::Operator::MUL); }
        | MulExp SLASH UnaryExp { $$ = std::make_unique<Compiler::AbstractSyntaxTree::Expression::BinaryExpression>(std::move($1), std::move($3), Compiler::AbstractSyntaxTree::Expression::Operator::DIV); }
        | MulExp PERCENT UnaryExp { $$ = std::make_unique<Compiler::AbstractSyntaxTree::Expression::BinaryExpression>(std::move($1), std::move($3), Compiler::AbstractSyntaxTree::Expression::Operator::MOD); }
        ;

AddExp: MulExp { $$ = std::move($1); }
        | AddExp PLUS MulExp { $$ = std::make_unique<Compiler::AbstractSyntaxTree::Expression::BinaryExpression>(
        std::move($1), std::move($3), Compiler::AbstractSyntaxTree::Expression::Operator::ADD); }
        | AddExp MINUS MulExp { $$ = std::make_unique<Compiler::AbstractSyntaxTree::Expression::BinaryExpression>(std::move($1), std::move($3), Compiler::AbstractSyntaxTree::Expression::Operator::SUB); }
        ;

RelExp: AddExp { $$ = std::move($1); }
        | RelExp LT AddExp { $$ = std::make_unique<Compiler::AbstractSyntaxTree::Expression::BinaryExpression>(std::move($1), std::move($3), Compiler::AbstractSyntaxTree::Expression::Operator::LT); }
        | RelExp GT AddExp { $$ = std::make_unique<Compiler::AbstractSyntaxTree::Expression::BinaryExpression>(std::move($1), std::move($3), Compiler::AbstractSyntaxTree::Expression::Operator::GT); }
        | RelExp LE AddExp { $$ = std::make_unique<Compiler::AbstractSyntaxTree::Expression::BinaryExpression>(std::move($1), std::move($3), Compiler::AbstractSyntaxTree::Expression::Operator::LE); }
        | RelExp GE AddExp { $$ = std::make_unique<Compiler::AbstractSyntaxTree::Expression::BinaryExpression>(std::move($1), std::move($3), Compiler::AbstractSyntaxTree::Expression::Operator::GE); }
        ;

EqExp: RelExp { $$ = std::move($1); }
        | EqExp EQ RelExp { $$ = std::make_unique<Compiler::AbstractSyntaxTree::Expression::BinaryExpression>(std::move($1), std::move($3), Compiler::AbstractSyntaxTree::Expression::Operator::EQ); }
        | EqExp NE RelExp { $$ = std::make_unique<Compiler::AbstractSyntaxTree::Expression::BinaryExpression>(std::move($1), std::move($3), Compiler::AbstractSyntaxTree::Expression::Operator::NE); }
        ;


LAndExp: EqExp { $$ = std::move($1); }
        | LAndExp AND EqExp { $$ = std::make_unique<Compiler::AbstractSyntaxTree::Expression::BinaryExpression>(std::move($1), std::move($3), Compiler::AbstractSyntaxTree::Expression::Operator::AND); }
        ;

LOrExp: LAndExp { $$ = std::move($1); }
        | LOrExp OR LAndExp { $$ = std::make_unique<Compiler::AbstractSyntaxTree::Expression::BinaryExpression>(std::move($1), std::move($3), Compiler::AbstractSyntaxTree::Expression::Operator::OR); }
        ;
        


%%

void yy::parser::error(const location_type& l, const std::string& m) {
  driver.error(l, m);
}

