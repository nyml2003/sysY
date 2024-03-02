//
// Created by 风唤长河 on 2024/3/1.
//

#include "Definition.hpp"

namespace Compiler::AbstractSyntaxTree::Node {
    Definition::Definition(IdentPtr ident, ExprPtr expr) : ident(std::move(ident)), expr(std::move(expr)) {
        this->typeName = "Definition";
    }
    void Definition::toMermaid() {
        std::cout << this->id << "[" << this->typeName << "]" << std::endl;
            std::cout << this->id << "--name-->" << this->ident->id << std::endl;
            this->ident->toMermaid();
            std::cout << this->id << "-->" << this->expr->id << std::endl;
            this->expr->toMermaid();
    }

    void Definition::toIR() {
        // TODO
    }

    void Definition::optimize() {
        // TODO
    }
}