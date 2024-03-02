//
// Created by 风唤长河 on 2024/2/28.
//

#include "BinaryExpression.hpp"
#include "Expression.hpp"
#include "Int32.hpp"
#include <iostream>
namespace Compiler::AbstractSyntaxTree::Node {
    BinaryExpression::BinaryExpression(ExprPtr left, Operator op, ExprPtr right) : left(std::move(left)), op(op), right(std::move(right)) {
        this->typeName = "BinaryExpression";
        this->begin = this->left->begin;
        this->end = this->right->end;
    }

    void BinExpr::toMermaid() {
        std::cout << this->id << "[" << this->typeName << "]" << std::endl;
        std::cout << this->id << "--lhs-->" << this->left->id << std::endl;
        std::cout << this->id << "--rhs-->" << this->right->id << std::endl;
        this->left->toMermaid();
        this->right->toMermaid();
    }

    ExprPtr BinaryExpression::constantFold() {
        if (this->isConstant()) {
            return std::make_unique<Int32>(
                    eval(
                            this->op,
                            dynamic_cast<Int32 *>(this->left.get())->value,
                            dynamic_cast<Int32 *>(this->right.get())->value));
        } else {
            return nullptr;
        }
    }

    void BinaryExpression::toIR() {
        // TODO
    }

    bool BinExpr::isConstant() {
        if (this->left->isConstant() && this->left->constantFold()) {
            this->left = this->left->constantFold();
        }
        if (this->right->isConstant() && this->right->constantFold()) {
            this->right = this->right->constantFold();
        }
        return this->left->isConstant() && this->right->isConstant();
    }

    Type BinExpr::getType() {
        if (this->left->getType() == Type::UNDEFINED || this->right->getType() == Type::UNDEFINED) {
            return Type::UNDEFINED;
        }
        auto leftType = this->left->getType();
        auto rightType = this->right->getType();
        if (leftType != rightType) {
            this->printLocation();
            std::cerr << "|Error: Type mismatch" << std::endl;
            std::cerr << "|wanted: " << innerType[leftType] << " got: " << innerType[rightType] << std::endl<<std::endl;
        }
        return this->left->getType();
    }
}
