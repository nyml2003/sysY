//
// Created by 风唤长河 on 2024/2/23.
//

#ifndef SYSY_COMPILER_ABSTRACT_SYNTAX_TREE_FUNCTION_DEFINITION_HPP
#define SYSY_COMPILER_ABSTRACT_SYNTAX_TREE_FUNCTION_DEFINITION_HPP
#include "Type.hpp"
#include "Identifier.hpp"
#include "Block.hpp"
namespace Compiler::AbstractSyntaxTree::Node {
    class FunctionDefinition : public BlockItem {
    public:
        explicit FunctionDefinition(Type type, IdentPtr ident, BlockPtr block);
        Type type;
        IdentPtr ident;
        BlockPtr block;
        void toMermaid() override;
        void toIR() override;
        void optimize() override;
    };
    
}
using FuncDef = Compiler::AbstractSyntaxTree::Node::FunctionDefinition;
using FuncDefPtr = std::unique_ptr<FuncDef>;
#endif //SYSY_COMPILER_ABSTRACT_SYNTAX_TREE_FUNCTION_DEFINITION_HPP
