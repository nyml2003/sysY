//
// Created by venty on 2024/2/23.
//

#ifndef FLEX_BISON_LLVM_COMPILATION_UNIT_HPP
#define FLEX_BISON_LLVM_COMPILATION_UNIT_HPP
#include "Header.hpp"
namespace Compiler::AbstractSyntaxTree
{
    class CompilationUnit : public IAbstractSyntaxTree {
    public:
        explicit CompilationUnit(std::unique_ptr<FunctionDefinition> functionDefinition) ;
        std::unique_ptr<FunctionDefinition> functionDefinition;
        void dump() override;
        void dumpMermaid();
        void dumpLLVM();
        void optimize() override;
    };
}
#endif //FLEX_BISON_LLVM_COMPILATION_UNIT_HPP
