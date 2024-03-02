//
// Created by 风唤长河 on 2024/2/28.
//
#include "UnaryExpression.hpp"
#include "Int32.hpp"
#include <iostream>
namespace Compiler::AbstractSyntaxTree::Node {
    
    UnaryExpression::UnaryExpression(Operator op, ExprPtr expr) : op(op), expr(std::move(expr)) {
        this->typeName = "UnaryExpression";
    }

    void UnaryExpr::toMermaid() {
        std::cout << this->id << "[" << this->typeName << "]" << std::endl;
        std::cout << this->id << "-->" << this->expr->id << std::endl;
        this->expr->toMermaid();
    }

    void UnaryExpr::toIR() {
        // TODO
    }

    ExprPtr UnaryExpr::constantFold()
    {
        if (this->expr->isConstant() && this->expr->constantFold()) {
            return std::make_unique<Int32>(
                eval(
                    this->op,
                    dynamic_cast<Int32 *>(this->expr.get())->value));
            }
        return nullptr;
    }

    bool UnaryExpr::isConstant()
    {
        if (this->expr->isConstant() && this->expr->constantFold()) {
            this->expr = this->expr->constantFold();
        }
        return this->expr->isConstant();
    }
}