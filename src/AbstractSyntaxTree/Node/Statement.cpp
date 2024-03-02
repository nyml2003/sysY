//
// Created by 风唤长河 on 2024/2/23.
//
#include "Statement.hpp"
#include "Expression.hpp"
#include <iostream>

namespace Compiler::AbstractSyntaxTree::Node {

    AssignStatement::AssignStatement(IdentPtr ident, ExprPtr def) : def(std::make_unique<Definition>(std::move(ident), std::move(def))) {
        this->typeName = "Assign";
        this->begin = this->def->begin;
        this->end = this->def->end;
    }

    void AssignStatement::toMermaid() {
        std::cout << this->id << "[" << this->typeName << "]" << std::endl;
        std::cout << this->id << "-->" << this->def->id << std::endl;
        this->def->toMermaid();
    }

    void AssignStatement::toIR() {
        // TODO
    }

    void AssignStatement::optimize() {
        this->def->optimize();
    }

    ReturnStatement::ReturnStatement(ExprPtr expr) : expr(std::move(expr)) {
        this->typeName = "Return";
        this->begin = this->expr->begin;
        this->end = this->expr->end;
    }

    void ReturnStatement::toMermaid() {
        std::cout << this->id << "[" << this->typeName << "]" << std::endl;
        std::cout << this->id << "-->" << this->expr->id << std::endl;
        this->expr->toMermaid();
    }

    void ReturnStatement::toIR() {
        // TODO
    }

    void ReturnStatement::optimize() {
        this->expr->optimize();
    }


}