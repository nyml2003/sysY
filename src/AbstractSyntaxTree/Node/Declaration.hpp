//
// Created by 风唤长河 on 2024/3/1.
//

#ifndef SYSY_COMPILER_ABSTRACT_SYNTAX_TREE_DECLARATION_HPP
#define SYSY_COMPILER_ABSTRACT_SYNTAX_TREE_DECLARATION_HPP
#include "Type.hpp"
#include "Identifier.hpp"
#include "Block.hpp"
#include "Definition.hpp"
namespace Compiler::AbstractSyntaxTree::Node {
    class Declaration : public BlockItem {
    public:
        explicit Declaration(Type type, IdentPtr ident);
        Type type;
        IdentPtr ident;
        void toMermaid() override;
        void toIR() override;
        void optimize() override;
    };
}
using Decl = Compiler::AbstractSyntaxTree::Node::Declaration;
using DeclPtr = std::unique_ptr<Decl>;
#endif //SYSY_COMPILER_ABSTRACT_SYNTAX_TREE_DECLARATION_HPP