//
// Created by 风唤长河 on 2024/2/24.
//

#ifndef SYSY_COMPILER_ABSTRACT_SYNTAX_TREE_EXPRESSION_HPP
#define SYSY_COMPILER_ABSTRACT_SYNTAX_TREE_EXPRESSION_HPP

#include "Node.hpp"
#include <map>
#include <cstdint>
#include "Type.hpp"
namespace Compiler::AbstractSyntaxTree::Node
{
    enum class Operator
    {
        ADD,
        SUB,
        MUL,
        DIV,
        MOD,
        AND,
        OR,
        EQ,
        NE,
        LT,
        GT,
        LE,
        GE,
        NOT,
        PLUS,
        MINUS,
    };
    std::int32_t eval(Operator op, std::int32_t left, std::int32_t right);
    std::int32_t eval(Operator op, std::int32_t value);
    inline static std::map<Operator, std::string> operatorName = {
            {Operator::ADD, "\"+\""},
            {Operator::SUB, "\"-\""},
            {Operator::MUL, "\"*\""},
            {Operator::DIV, "\"/\""},
            {Operator::MOD, "\"%\""},
            {Operator::AND, "\"&&\""},
            {Operator::OR, "\"||\""},
            {Operator::EQ, "\"==\""},
            {Operator::NE, "\"!=\""},
            {Operator::LT, "\"<\""},
            {Operator::GT, "\">\""},
            {Operator::LE, "\"<=\""},
            {Operator::GE, "\">=\""},
            {Operator::NOT, "\"!\""},
            {Operator::PLUS, "\"+\""},
            {Operator::MINUS, "\"-\""},
    };
    class Expression : virtual public Node
    {
    public:
        Expression() = default;
        void toMermaid() override =0;
        void toIR() override =0;
        virtual std::unique_ptr<Expression> constantFold()=0;
        void optimize() override;
        virtual bool isConstant()=0;
        ~Expression() override = default;
    };
    class NullExpression : public Expression
    {
    public:
        explicit NullExpression();
        void toMermaid() override;
        void toIR() override;
        std::unique_ptr<Expression> constantFold() override;
        bool isConstant() override;
    };
}
using Operator = Compiler::AbstractSyntaxTree::Node::Operator;
using Expr = Compiler::AbstractSyntaxTree::Node::Expression;
using ExprPtr = std::unique_ptr<Expr>;
using NullExpr = Compiler::AbstractSyntaxTree::Node::NullExpression;
using NullExprPtr = std::unique_ptr<NullExpr>;
inline auto& opName = Compiler::AbstractSyntaxTree::Node::operatorName;
#endif // SYSY_COMPILER_ABSTRACT_SYNTAX_TREE_EXPRESSION_HPP
