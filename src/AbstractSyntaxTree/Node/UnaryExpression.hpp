//
// Created by 风唤长河 on 2024/2/28.
//

#ifndef SYSY_COMPILER_ABSTRACT_SYNTAX_TREE_UNARY_EXPRESSION_HPP
#define SYSY_COMPILER_ABSTRACT_SYNTAX_TREE_UNARY_EXPRESSION_HPP
#include "Expression.hpp"
namespace Compiler::AbstractSyntaxTree::Node {
    class UnaryExpression : public Expr
    {
    public:
        UnaryExpression(Operator op, ExprPtr expr);
        Operator op;
        ExprPtr expr;
        void toMermaid() override;
        void toIR() override;
        ExprPtr constantFold() override;
        bool isConstant() override;
        Type getType() override;
    };
}
using UnaryExpr = Compiler::AbstractSyntaxTree::Node::UnaryExpression;
using UnaryExprPtr = std::unique_ptr<UnaryExpr>;
#endif //SYSY_COMPILER_ABSTRACT_SYNTAX_TREE_UNARY_EXPRESSION_HPP
