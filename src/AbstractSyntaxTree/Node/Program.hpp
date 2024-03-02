//
// Created by 风唤长河 on 2024/2/29.
//

#ifndef SYSY_COMPILER_ABSTRACT_SYNTAX_TREE_PROGRAM_HPP
#define SYSY_COMPILER_ABSTRACT_SYNTAX_TREE_PROGRAM_HPP
#include "Block.hpp"
#include <vector>
namespace Compiler::AbstractSyntaxTree::Node {
    class Program : public Node {
    public:
        explicit Program();
        ~Program() = default;
        void toMermaid() override;
        void toIR() override;
        void optimize() override;
        BlockPtr block;
    };
}
using Program = Compiler::AbstractSyntaxTree::Node::Program;
using ProgramPtr = std::unique_ptr<Program>;
#endif //SYSY_COMPILER_ABSTRACT_SYNTAX_TREE_PROGRAM_HPP
