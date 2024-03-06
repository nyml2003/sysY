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
    | UNARYOP UnaryExp {
        switch ($1) {
            case Compiler::AST::Operator::MINUS:{
                auto isInt = dynamic_cast< Compiler::AST::Int32* >($2.get());
                auto isFloat = dynamic_cast< Compiler::AST::Float32* >($2.get());
                if ( isInt ) {
                    $$ = std::make_unique< Compiler::AST::Int32 >(-isInt->val);
                } else if ( isFloat ) {
                    $$ = std::make_unique< Compiler::AST::Float32 >(-isFloat->val);
                } else {
                    $$ = std::make_unique< Compiler::AST::UnaryExp >(Compiler::AST::Operator::MINUS, std::move($2));
                }
                break;
            }
                
            case Compiler::AST::Operator::NOT:{
                $$ = std::make_unique< Compiler::AST::UnaryExp >(Compiler::AST::Operator::NOT, std::move($2));
                break;
            }
                
            case Compiler::AST::Operator::PLUS:{
                $$ = std::move($2);
                break;
            }
                
            default:
                std::cerr << "Unknown unary operator" << std::endl;
                break;
        }
    }
    ;

MulExp:
    UnaryExp { $$ = std::move($1); }
    | MulExp STAR UnaryExp { 
        auto isLeftInt = dynamic_cast< Compiler::AST::Int32* >($1.get());
        auto isRightInt = dynamic_cast< Compiler::AST::Int32* >($3.get());
        auto isLeftFloat = dynamic_cast< Compiler::AST::Float32* >($1.get());
        auto isRightFloat = dynamic_cast< Compiler::AST::Float32* >($3.get());
        if ( isLeftInt && isRightInt ) {
            $$ = std::make_unique< Compiler::AST::Int32 >(isLeftInt->val * isRightInt->val);
        } else if ( isLeftFloat && isRightFloat ) {
            $$ = std::make_unique< Compiler::AST::Float32 >(isLeftFloat->val * isRightFloat->val);
        } else if ( isLeftInt && isRightFloat ) {
            $$ = std::make_unique< Compiler::AST::Float32 >(isLeftInt->val * isRightFloat->val);
        } else if ( isLeftFloat && isRightInt ) {
            $$ = std::make_unique< Compiler::AST::Float32 >(isLeftFloat->val * isRightInt->val);
        } else {
            $$ = std::make_unique< Compiler::AST::BinaryExp >(std::move($1), Compiler::AST::Operator::MUL, std::move($3));
        }    
    }
    | MulExp SLASH UnaryExp { 
        auto isLeftInt = dynamic_cast< Compiler::AST::Int32* >($1.get());
        auto isRightInt = dynamic_cast< Compiler::AST::Int32* >($3.get());
        auto isLeftFloat = dynamic_cast< Compiler::AST::Float32* >($1.get());
        auto isRightFloat = dynamic_cast< Compiler::AST::Float32* >($3.get());

        if ( isRightInt && isRightInt->val == 0 ) {
            driver.error(driver.location, "Division by zero");
        } else if ( isRightFloat && (isRightFloat->val == 0.0f || isRightFloat->val == -0.0f) ) {
            driver.error(driver.location, "Division by zero");
        }


        if ( isLeftInt && isRightInt ) {
            if ( isLeftInt->val % isRightInt->val == 0 ) {
                $$ = std::make_unique< Compiler::AST::Int32 >(isLeftInt->val / isRightInt->val);
            } else {
                $$ = std::make_unique< Compiler::AST::Float32 >(static_cast<float>(isLeftInt->val) / static_cast<float>(isRightInt->val));
            }
        } else if ( isLeftFloat && isRightFloat ) {
            $$ = std::make_unique< Compiler::AST::Float32 >(isLeftFloat->val / isRightFloat->val);
        } else if ( isLeftInt && isRightFloat ) {
            $$ = std::make_unique< Compiler::AST::Float32 >(isLeftInt->val / isRightFloat->val);
        } else if ( isLeftFloat && isRightInt ) {
            $$ = std::make_unique< Compiler::AST::Float32 >(isLeftFloat->val / isRightInt->val);
        } else {
            $$ = std::make_unique< Compiler::AST::BinaryExp >(std::move($1), Compiler::AST::Operator::DIV, std::move($3));
        }
    }
    | MulExp PERCENT UnaryExp { 
        auto isLeftInt = dynamic_cast< Compiler::AST::Int32* >($1.get());
        auto isRightInt = dynamic_cast< Compiler::AST::Int32* >($3.get());
        if ( isRightInt && isRightInt->val == 0 ) {
            driver.error(driver.location, "Division by zero");
        }
        if ( isLeftInt && isRightInt ) {
            $$ = std::make_unique< Compiler::AST::Int32 >(isLeftInt->val % isRightInt->val);
        } else {
            $$ = std::make_unique< Compiler::AST::BinaryExp >(std::move($1), Compiler::AST::Operator::MOD, std::move($3));
        }
    }
    ;

AddExp:
    MulExp { $$ = std::move($1); }
    | AddExp PLUS MulExp { 
        auto isLeftInt = dynamic_cast< Compiler::AST::Int32* >($1.get());
        auto isRightInt = dynamic_cast< Compiler::AST::Int32* >($3.get());
        auto isLeftFloat = dynamic_cast< Compiler::AST::Float32* >($1.get());
        auto isRightFloat = dynamic_cast< Compiler::AST::Float32* >($3.get());
        if ( isLeftInt && isRightInt ) {
            $$ = std::make_unique< Compiler::AST::Int32 >(isLeftInt->val + isRightInt->val);
        } else if ( isLeftFloat && isRightFloat ) {
            $$ = std::make_unique< Compiler::AST::Float32 >(isLeftFloat->val + isRightFloat->val);
        } else if ( isLeftInt && isRightFloat ) {
            $$ = std::make_unique< Compiler::AST::Float32 >(isLeftInt->val + isRightFloat->val);
        } else if ( isLeftFloat && isRightInt ) {
            $$ = std::make_unique< Compiler::AST::Float32 >(isLeftFloat->val + isRightInt->val);
        } else {
            $$ = std::make_unique< Compiler::AST::BinaryExp >(std::move($1), Compiler::AST::Operator::ADD, std::move($3));
        }
    }
    | AddExp MINUS MulExp { 
        auto isLeftInt = dynamic_cast< Compiler::AST::Int32* >($1.get());
        auto isRightInt = dynamic_cast< Compiler::AST::Int32* >($3.get());
        auto isLeftFloat = dynamic_cast< Compiler::AST::Float32* >($1.get());
        auto isRightFloat = dynamic_cast< Compiler::AST::Float32* >($3.get());
        if ( isLeftInt && isRightInt ) {
            $$ = std::make_unique< Compiler::AST::Int32 >(isLeftInt->val - isRightInt->val);
        } else if ( isLeftFloat && isRightFloat ) {
            $$ = std::make_unique< Compiler::AST::Float32 >(isLeftFloat->val - isRightFloat->val);
        } else if ( isLeftInt && isRightFloat ) {
            $$ = std::make_unique< Compiler::AST::Float32 >(isLeftInt->val - isRightFloat->val);
        } else if ( isLeftFloat && isRightInt ) {
            $$ = std::make_unique< Compiler::AST::Float32 >(isLeftFloat->val - isRightInt->val);
        } else {
            $$ = std::make_unique< Compiler::AST::BinaryExp >(std::move($1), Compiler::AST::Operator::SUB, std::move($3));
        }
    }
    ;

RelExp:
    AddExp { $$ = std::move($1); }
    | RelExp LT AddExp { 
        auto isLeftInt = dynamic_cast< Compiler::AST::Int32* >($1.get());
        auto isRightInt = dynamic_cast< Compiler::AST::Int32* >($3.get());
        auto isLeftFloat = dynamic_cast< Compiler::AST::Float32* >($1.get());
        auto isRightFloat = dynamic_cast< Compiler::AST::Float32* >($3.get());
        if ( isLeftInt && isRightInt ) {
            $$ = std::make_unique< Compiler::AST::Int32 >(isLeftInt->val < isRightInt->val);
        } else if ( isLeftFloat && isRightFloat ) {
            $$ = std::make_unique< Compiler::AST::Int32 >(isLeftFloat->val < isRightFloat->val);
        } else if ( isLeftInt && isRightFloat ) {
            $$ = std::make_unique< Compiler::AST::Int32 >(isLeftInt->val < isRightFloat->val);
        } else if ( isLeftFloat && isRightInt ) {
            $$ = std::make_unique< Compiler::AST::Int32 >(isLeftFloat->val < isRightInt->val);
        } else {
            $$ = std::make_unique< Compiler::AST::BinaryExp >(std::move($1), Compiler::AST::Operator::LT, std::move($3));
        }
    }
    | RelExp GT AddExp { 
        auto isLeftInt = dynamic_cast< Compiler::AST::Int32* >($1.get());
        auto isRightInt = dynamic_cast< Compiler::AST::Int32* >($3.get());
        auto isLeftFloat = dynamic_cast< Compiler::AST::Float32* >($1.get());
        auto isRightFloat = dynamic_cast< Compiler::AST::Float32* >($3.get());
        if ( isLeftInt && isRightInt ) {
            $$ = std::make_unique< Compiler::AST::Int32 >(isLeftInt->val > isRightInt->val);
        } else if ( isLeftFloat && isRightFloat ) {
            $$ = std::make_unique< Compiler::AST::Int32 >(isLeftFloat->val > isRightFloat->val);
        } else if ( isLeftInt && isRightFloat ) {
            $$ = std::make_unique< Compiler::AST::Int32 >(isLeftInt->val > isRightFloat->val);
        } else if ( isLeftFloat && isRightInt ) {
            $$ = std::make_unique< Compiler::AST::Int32 >(isLeftFloat->val > isRightInt->val);
        } else {
            $$ = std::make_unique< Compiler::AST::BinaryExp >(std::move($1), Compiler::AST::Operator::GT, std::move($3));
        }
    }
    | RelExp LE AddExp { 
        auto isLeftInt = dynamic_cast< Compiler::AST::Int32* >($1.get());
        auto isRightInt = dynamic_cast< Compiler::AST::Int32* >($3.get());
        auto isLeftFloat = dynamic_cast< Compiler::AST::Float32* >($1.get());
        auto isRightFloat = dynamic_cast< Compiler::AST::Float32* >($3.get());
        if ( isLeftInt && isRightInt ) {
            $$ = std::make_unique< Compiler::AST::Int32 >(isLeftInt->val <= isRightInt->val);
        } else if ( isLeftFloat && isRightFloat ) {
            $$ = std::make_unique< Compiler::AST::Int32 >(isLeftFloat->val <= isRightFloat->val);
        } else if ( isLeftInt && isRightFloat ) {
            $$ = std::make_unique< Compiler::AST::Int32 >(isLeftInt->val <= isRightFloat->val);
        } else if ( isLeftFloat && isRightInt ) {
            $$ = std::make_unique< Compiler::AST::Int32 >(isLeftFloat->val <= isRightInt->val);
        } else {
            $$ = std::make_unique< Compiler::AST::BinaryExp >(std::move($1), Compiler::AST::Operator::LE, std::move($3));
        }
    }
    | RelExp GE AddExp { 
        auto isLeftInt = dynamic_cast< Compiler::AST::Int32* >($1.get());
        auto isRightInt = dynamic_cast< Compiler::AST::Int32* >($3.get());
        auto isLeftFloat = dynamic_cast< Compiler::AST::Float32* >($1.get());
        auto isRightFloat = dynamic_cast< Compiler::AST::Float32* >($3.get());
        if ( isLeftInt && isRightInt ) {
            $$ = std::make_unique< Compiler::AST::Int32 >(isLeftInt->val >= isRightInt->val);
        } else if ( isLeftFloat && isRightFloat ) {
            $$ = std::make_unique< Compiler::AST::Int32 >(isLeftFloat->val >= isRightFloat->val);
        } else if ( isLeftInt && isRightFloat ) {
            $$ = std::make_unique< Compiler::AST::Int32 >(isLeftInt->val >= isRightFloat->val);
        } else if ( isLeftFloat && isRightInt ) {
            $$ = std::make_unique< Compiler::AST::Int32 >(isLeftFloat->val >= isRightInt->val);
        } else {
            $$ = std::make_unique< Compiler::AST::BinaryExp >(std::move($1), Compiler::AST::Operator::GE, std::move($3));
        }
    }
    ;

EqExp:
    RelExp { $$ = std::move($1); }
    | EqExp EQ RelExp { 
        auto isLeftInt = dynamic_cast< Compiler::AST::Int32* >($1.get());
        auto isRightInt = dynamic_cast< Compiler::AST::Int32* >($3.get());
        auto isLeftFloat = dynamic_cast< Compiler::AST::Float32* >($1.get());
        auto isRightFloat = dynamic_cast< Compiler::AST::Float32* >($3.get());
        if ( isLeftInt && isRightInt ) {
            $$ = std::make_unique< Compiler::AST::Int32 >(isLeftInt->val == isRightInt->val);
        } else if ( isLeftFloat && isRightFloat ) {
            $$ = std::make_unique< Compiler::AST::Int32 >(isLeftFloat->val == isRightFloat->val);
        } else if ( isLeftInt && isRightFloat ) {
            $$ = std::make_unique< Compiler::AST::Int32 >(isLeftInt->val == isRightFloat->val);
        } else if ( isLeftFloat && isRightInt ) {
            $$ = std::make_unique< Compiler::AST::Int32 >(isLeftFloat->val == isRightInt->val);
        } else {
            $$ = std::make_unique< Compiler::AST::BinaryExp >(std::move($1), Compiler::AST::Operator::EQ, std::move($3));
        }
    }
    | EqExp NE RelExp { 
        auto isLeftInt = dynamic_cast< Compiler::AST::Int32* >($1.get());
        auto isRightInt = dynamic_cast< Compiler::AST::Int32* >($3.get());
        auto isLeftFloat = dynamic_cast< Compiler::AST::Float32* >($1.get());
        auto isRightFloat = dynamic_cast< Compiler::AST::Float32* >($3.get());
        if ( isLeftInt && isRightInt ) {
            $$ = std::make_unique< Compiler::AST::Int32 >(isLeftInt->val != isRightInt->val);
        } else if ( isLeftFloat && isRightFloat ) {
            $$ = std::make_unique< Compiler::AST::Int32 >(isLeftFloat->val != isRightFloat->val);
        } else if ( isLeftInt && isRightFloat ) {
            $$ = std::make_unique< Compiler::AST::Int32 >(isLeftInt->val != isRightFloat->val);
        } else if ( isLeftFloat && isRightInt ) {
            $$ = std::make_unique< Compiler::AST::Int32 >(isLeftFloat->val != isRightInt->val);
        } else {
            $$ = std::make_unique< Compiler::AST::BinaryExp >(std::move($1), Compiler::AST::Operator::NE, std::move($3));
        }
    }
    ;

LAndExp:
    EqExp { $$ = std::move($1); }
    | LAndExp AND EqExp {
        auto isLeftInt = dynamic_cast< Compiler::AST::Int32* >($1.get());
        auto isRightInt = dynamic_cast< Compiler::AST::Int32* >($3.get());
        if ( isLeftInt && isRightInt ) {
            $$ = std::make_unique< Compiler::AST::Int32 >(isLeftInt->val && isRightInt->val);
        } else {
            $$ = std::make_unique< Compiler::AST::BinaryExp >(std::move($1), Compiler::AST::Operator::AND, std::move($3));
        }
    }
    ;

LOrExp:
    LAndExp { $$ = std::move($1); }
    | LOrExp OR LAndExp { 
        auto isLeftInt = dynamic_cast< Compiler::AST::Int32* >($1.get());
        auto isRightInt = dynamic_cast< Compiler::AST::Int32* >($3.get());
        if ( isLeftInt && isRightInt ) {
            $$ = std::make_unique< Compiler::AST::Int32 >(isLeftInt->val || isRightInt->val);
        } else {
            $$ = std::make_unique< Compiler::AST::BinaryExp >(std::move($1), Compiler::AST::Operator::OR, std::move($3));
        }
    }

%%

void yy::parser::error(const location_type& l, const std::string& m) {
     driver.error(l, m);
}

