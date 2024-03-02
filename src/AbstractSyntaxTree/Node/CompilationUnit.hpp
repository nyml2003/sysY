//
// Created by 风唤长河 on 2024/2/23.
//

#ifndef SYSY_COMPILER_ABSTRACT_SYNTAX_TREE_COMPILATION_UNIT_HPP
#define SYSY_COMPILER_ABSTRACT_SYNTAX_TREE_COMPILATION_UNIT_HPP
#include "FunctionDefinition.hpp"
namespace Compiler::AbstractSyntaxTree::Node {
    class CompilationUnit : public Node {
    public:
        explicit CompilationUnit(FuncDefPtr funcDef);
        FuncDefPtr funcDef;
        void toMermaid() override;
        void toIR() override;
        void optimize() override;
    };
}
using CompUnit = Compiler::AbstractSyntaxTree::Node::CompilationUnit;
using CompUnitPtr = std::unique_ptr<CompUnit>;
#endif //SYSY_COMPILER_ABSTRACT_SYNTAX_TREE_COMPILATION_UNIT_HPP
