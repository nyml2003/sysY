//
// Created by 风唤长河 on 2024/2/24.
//

#include "Expression.hpp"
#include <iostream>
namespace Compiler::AbstractSyntaxTree::Node {
    std::int32_t eval(Operator op, std::int32_t left, std::int32_t right)
    {
        switch (op)
        {
        case Operator::ADD:
            return left + right;
        case Operator::SUB:
            return left - right;
        case Operator::MUL:
            return left * right;
        case Operator::DIV:
            return left / right;
        case Operator::MOD:
            return left % right;
        case Operator::AND:
            return left && right;
        case Operator::OR:
            return left || right;
        case Operator::EQ:
            return left == right;
        case Operator::NE:
            return left != right;
        case Operator::LT:
            return left < right;
        case Operator::GT:
            return left > right;
        case Operator::LE:
            return left <= right;
        case Operator::GE:
            return left >= right;
        default:
            std::cerr << "eval() is not implemented" << std::endl;
            return 0;
        }
    }

    std::int32_t eval(Operator op, std::int32_t value)
    {
        switch (op)
        {
        case Operator::PLUS:
            return value;
        case Operator::MINUS:
            return -value;
        case Operator::NOT:
            return !value;
        default:
            std::cerr << "eval() is not implemented" << std::endl;
            return 0;
        }
    }

    void Expression::optimize(){}

    NullExpression::NullExpression()
    {
        this->typeName = "NullExpression";
    }

    void NullExpression::toMermaid()
    {
        std::cout << this->id << "[" << this->typeName << "]" << std::endl;
    }

    void NullExpression::toIR()
    {
        // TODO
    }

    std::unique_ptr<Expression> NullExpression::constantFold()
    {
        return nullptr;
    }

    bool NullExpression::isConstant()
    {
        return true;
    }

    Type NullExpression::getType()
    {
        return Type::NONE;
    }
}