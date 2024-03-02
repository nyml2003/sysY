//
// Created by 风唤长河 on 2024/2/28.
//

#ifndef SYSY_COMPILER_ABSTRACT_SYNTAX_TREE_INT32_HPP
#define SYSY_COMPILER_ABSTRACT_SYNTAX_TREE_INT32_HPP
#include "Expression.hpp"
#include <cstdint>
#include <map>
#include "Type.hpp"
namespace Compiler::AbstractSyntaxTree::Node {
    class Int32 : public Expr
    {
    public:
        explicit Int32(std::int32_t value);
        std::int32_t value{};
        void toMermaid() override;
        void toIR() override;
        ExprPtr constantFold() override;
        bool isConstant() override;
    };
}
using Int32 = Compiler::AbstractSyntaxTree::Node::Int32;
using Int32Ptr = std::unique_ptr<Int32>;
#endif //SYSY_COMPILER_ABSTRACT_SYNTAX_TREE_INT32_HPP
