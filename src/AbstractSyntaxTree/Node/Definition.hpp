//
// Created by 风唤长河 on 2024/3/1.
//

#ifndef SYSY_COMPILER_ABSTRACT_SYNTAX_TREE_DEFINITION_HPP
#define SYSY_COMPILER_ABSTRACT_SYNTAX_TREE_DEFINITION_HPP
#include "Identifier.hpp"
#include "Block.hpp"
namespace Compiler::AbstractSyntaxTree::Node {
    class Definition : public BlockItem {
    public:
        explicit Definition(IdentPtr ident, ExprPtr expr);
        IdentPtr ident;
        ExprPtr expr;
        void toMermaid() override;
        void toIR() override;
        void optimize() override;
    };
}
using Def = Compiler::AbstractSyntaxTree::Node::Definition;
using DefPtr = std::unique_ptr<Def>;
using DefList = std::vector<DefPtr>;

#endif //SYSY_COMPILER_ABSTRACT_SYNTAX_TREE_DEFINITION_HPP