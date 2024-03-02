//
// Created by venty on 2024/2/24.
//

#include "Header.hpp"
#include <string>
#include <iostream>
namespace Compiler::AbstractSyntaxTree::Expression{
    void IExpression::dump()
    {
       Compiler::driver.errorFile << "IExpression::dump() is not implemented" << std::endl;
    }

    llvm::Value* IExpression::dumpLLVM()
    {
        std::cout << "IExpression::dumpLLVM() is not implemented" << std::endl;
        return nullptr;
    }

    std::unique_ptr<IExpression> IExpression::constantFold()
    {
       Compiler::driver.errorFile << "IExpression::constantFold() is not implemented" << std::endl;
        return nullptr;
    }

    bool IExpression::isConstant()
    {
       Compiler::driver.errorFile << "IExpression::isConstant() is not implemented" << std::endl;
        return false;
    }

    void IExpression::optimize()
    {
       Compiler::driver.errorFile << "IExpression::optimize() is not implemented" << std::endl;
    }

    BinaryExpression::BinaryExpression(std::unique_ptr<IExpression> left, std::unique_ptr<IExpression> right, Operator op)
        : left(std::move(left)), right(std::move(right)), op(op) {}

    void BinaryExpression::dump()  {
        std::cout << id << "[" << opName[op] << "]\n";
        std::cout << id << "--" << "left" << "-->" << left->id << std::endl;
        left->dump();
        std::cout << id << "--" << "right" << "-->" << right->id << std::endl;
        right->dump();
    }

    llvm::Value *BinaryExpression::dumpLLVM()
    {
        llvm::Value *leftValue = left->dumpLLVM();
        llvm::Value *rightValue = right->dumpLLVM();
        switch (op)
        {
        case Operator::ADD:
            return builder.CreateAdd(leftValue, rightValue);
        case Operator::SUB:
            return builder.CreateSub(leftValue, rightValue);
        case Operator::MUL:
            return builder.CreateMul(leftValue, rightValue);
        case Operator::DIV:
            return builder.CreateSDiv(leftValue, rightValue);
        case Operator::MOD:
            return builder.CreateSRem(leftValue, rightValue);
        case Operator::AND:
            return builder.CreateAnd(leftValue, rightValue);
        case Operator::OR:
            return builder.CreateOr(leftValue, rightValue);
        case Operator::EQ:
            return builder.CreateICmpEQ(leftValue, rightValue);
        case Operator::NE:
            return builder.CreateICmpNE(leftValue, rightValue);
        case Operator::LT:
            return builder.CreateICmpSLT(leftValue, rightValue);
        case Operator::GT:
            return builder.CreateICmpSGT(leftValue, rightValue);
        case Operator::LE:
            return builder.CreateICmpSLE(leftValue, rightValue);
        case Operator::GE:
            return builder.CreateICmpSGE(leftValue, rightValue);
        default:
           Compiler::driver.errorFile << "dumpLLVM() is not implemented" << std::endl;
            return nullptr;
        }
    }

    std::unique_ptr<IExpression> BinaryExpression::constantFold()
    {
        if (this->isConstant())
        {
            return std::make_unique<Int32>(
                eval(
                    op,
                    dynamic_cast<Int32 *>(left.get())->value,
                    dynamic_cast<Int32 *>(right.get())->value));
        }
        else
        {
            return std::make_unique<BinaryExpression>(std::move(left), std::move(right), op);
        }
    }

    bool BinaryExpression::isConstant()
    {
        if (left->isConstant() && left->constantFold())
        {
            left = std::move(left->constantFold());
        }
        if (right->isConstant() && right->constantFold())
        {
            right = std::move(right->constantFold());
        }
        return left->isConstant() && right->isConstant();
    }

    UnaryExpression::UnaryExpression(std::unique_ptr<IExpression> expression, Operator op) : expression(std::move(expression)), op(op) {}

    void UnaryExpression::dump()
    {
        std::cout << id << "[" << opName[op] << "]\n";
        std::cout << id << "-->" << expression->id << std::endl;
        expression->dump();
    }

    llvm::Value *UnaryExpression::dumpLLVM()
    {
        llvm::Value *value = expression->dumpLLVM();
        switch (op)
        {
        case Operator::PLUS:
            return value;
        case Operator::MINUS:
            return builder.CreateNeg(value);
        case Operator::NOT:
            return builder.CreateNot(value);
        default:
           Compiler::driver.errorFile << "dumpLLVM() is not implemented" << std::endl;
            return nullptr;
        }
    }

    std::unique_ptr<IExpression> UnaryExpression::constantFold()
    {
        if (this->isConstant())
        {
            return std::make_unique<Int32>(
                eval(
                    op,
                    dynamic_cast<Int32 *>(expression.get())->value));
        }
        else
        {
            return std::make_unique<UnaryExpression>(std::move(expression), op);
        }
    }

    bool UnaryExpression::isConstant()
    {
        if (expression->isConstant() && expression->constantFold())
        {
            expression = std::move(expression->constantFold());
        }
        return expression->isConstant();
    }

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
           Compiler::driver.errorFile << "eval() is not implemented" << std::endl;
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
           Compiler::driver.errorFile << "eval() is not implemented" << std::endl;
            return 0;
        }
    }

    Int32::Int32(std::int32_t value) : value(value) {}

    void Int32::dump()
    {
        std::cout << id << "[" << value << "]\n";
    }

    llvm::Value *Int32::dumpLLVM()
    {
        return builder.getInt32(value);
    }

    std::unique_ptr<IExpression> Int32::constantFold()
    {
        return nullptr;
    }

    bool Int32::isConstant()
    {
        return true;
    }

    Identifier::Identifier(const std::string &name) : name(name) {}

    void Identifier::dump()
    {
        std::cout << id << "[" << name << "]\n";
    }

    llvm::Value *Identifier::dumpLLVM()
    {
        //TODO
        return nullptr;
    }

    std::unique_ptr<IExpression> Identifier::constantFold()
    {
        return nullptr;
    }

    bool Identifier::isConstant()
    {
        return false;
    }
}