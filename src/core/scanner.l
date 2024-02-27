%{
#include <cerrno>
#include <climits>
#include <cstdlib>
#include "Driver.hpp"
#include "parser.hpp"
%}
%option noyywrap nounput noinput batch debug
%{
    yy::parser::symbol_type make_NUMBER(const std::string&s,const yy::parser::location_type& loc);
%}



%{
    # define YY_USER_ACTION loc.columns(yyleng);
%}
%%
%{
    yy::location& loc=driver.location;
    loc.step();
%}

";" {return yy::parser::make_SEMICOLON(loc);}
"(" {return yy::parser::make_LPAREN(loc);}
")" {return yy::parser::make_RPAREN(loc);}
"{" {return yy::parser::make_LBRACE(loc);}
"}" {return yy::parser::make_RBRACE(loc);}
"+" {return yy::parser::make_PLUS(loc);}
"-" {return yy::parser::make_MINUS(loc);}
"*" {return yy::parser::make_STAR(loc);}
"/" {return yy::parser::make_SLASH(loc);}
"%" {return yy::parser::make_PERCENT(loc);}
"==" {return yy::parser::make_EQ(loc);}
"!=" {return yy::parser::make_NE(loc);}
"<" {return yy::parser::make_LT(loc);}
"<=" {return yy::parser::make_LE(loc);}
">" {return yy::parser::make_GT(loc);}
">=" {return yy::parser::make_GE(loc);}
"=" {return yy::parser::make_ASSIGN(loc);}
"&&" {return yy::parser::make_AND(loc);}
"||" {return yy::parser::make_OR(loc);}
"!" {return yy::parser::make_NOT(loc);}



[0-9]+ {return make_NUMBER(yytext, loc);}
"int" {return yy::parser::make_TYPE_INT("int", loc);}
"return" {return yy::parser::make_KEYWORD_RETURN("return",loc);}
[ \t\r]+ {loc.step();}

\n+ {loc.lines(yyleng); loc.step();}


[a-zA-Z_][a-zA-Z0-9_]* return yy::parser::make_IDENT(yytext, loc);



. throw yy::parser::syntax_error(loc, "invalid character: " + std::string(yytext));

<<EOF>> return yy::parser::make_YYEOF(loc);

%%

yy::parser::symbol_type make_NUMBER(const std::string&s,const yy::parser::location_type&loc) {
    errno=0;
    long value=strtol(s.c_str(), nullptr, 10);
    if (! (INT_MIN<=value && value<=INT_MAX) || errno==ERANGE) {
        throw yy::parser::syntax_error(loc, "integer overflow: " + s);
    }
    std::int32_t i32=static_cast<std::int32_t>(value);
    return yy::parser::make_INT_CONST(i32, loc);
}

void Compiler::Driver::scan_begin() {
    yy_flex_debug=trace_scanning;
    if (this->inputFileName.empty() || this->inputFileName=="-") {
        yyin=stdin;
    } else if (! (yyin=fopen(this->inputFileName.c_str(), "r"))) {
        throw std::runtime_error("cannot open file: " + this->inputFileName);
    }
    if (this->outputFileName.empty() || this->outputFileName=="-") {
        yyout=stdout;
    } else if (! (yyout=fopen(this->outputFileName.c_str(), "w"))) {
        throw std::runtime_error("cannot open file: " + this->outputFileName);
    }
    if (this->errorFileName.empty() || this->errorFileName=="-") {
        ;
    } else {
        freopen(this->errorFileName.c_str(), "w", stderr);
    }
}

void Compiler::Driver::scan_end() {
    if (this->inputFileName.empty() || this->inputFileName=="-") {
        ;
    } else {
        fclose(yyin);
    }
    if (this->outputFileName.empty() || this->outputFileName=="-") {
        ;
    } else {
        fclose(yyout);
    }
    if (this->errorFileName.empty() || this->errorFileName=="-") {
        ;
    } else {
        fclose(stderr);
    }
}