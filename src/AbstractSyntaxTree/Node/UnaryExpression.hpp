//
// Created by venty on 2024/2/28.
//

#ifndef SYSY_COMPILER_ABSTRACT_SYNTAX_TREE_BINARY_EXPRESSION_HPP
#define SYSY_COMPILER_ABSTRACT_SYNTAX_TREE_BINARY_EXPRESSION_HPP
#include "Expression.hpp"
namespace Compiler::AbstractSyntaxTree::Node {
    class BinaryExpression : public Expr
    {
    public:
        explicit BinaryExpression(ExprPtr left, Operator op, ExprPtr right);
        ExprPtr left;
        ExprPtr right;
        Operator op;
        void toMermaid() override;
        void toIR() override;
        ExprPtr constantFold() override;
        bool isConstant() override;
    };
    using BinExpr = Compiler::AbstractSyntaxTree::Node::BinaryExpression;
    using BinExprPtr = std::unique_ptr<BinExpr>;
}
#endif //SYSY_COMPILER_ABSTRACT_SYNTAX_TREE_BINARY_EXPRESSION_HPP
