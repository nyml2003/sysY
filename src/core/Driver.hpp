//
// Created by venty on 2024/2/23.
//

#ifndef FLEX_BISON_LLVM_DRIVER_HPP
#define FLEX_BISON_LLVM_DRIVER_HPP
#include "parser.hpp"
namespace Compiler {
    class Driver {
    public:
        explicit Driver(std::string input, std::string output, std::string error,bool trace_parsing, bool trace_scanning);
        bool trace_parsing;
        bool trace_scanning;
        yy::location location;
        std::unique_ptr<Compiler::AbstractSyntaxTree::CompilationUnit> result;
        ~Driver();
        void parse();
        std::string inputFileName;
        std::string outputFileName;
        std::string errorFileName;
        static void error(const yy::location &l, const std::string &m);
        void scan_begin();
        void scan_end();
    };
}
# define YY_DECL \
  yy::parser::symbol_type yylex (Compiler::Driver &driver)

YY_DECL;
#endif //FLEX_BISON_LLVM_DRIVER_HPP
