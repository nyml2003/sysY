//
// Created by 风唤长河 on 2024/3/1.
//

#include "Definition.hpp"
#include "Scope.hpp"
namespace Compiler::AbstractSyntaxTree::Node {
    Definition::Definition(IdentPtr ident, ExprPtr expr) : ident(std::move(ident)), expr(std::move(expr)) {
        this->typeName = "Definition";
        this->begin = this->ident->begin;
        this->end = this->expr->end;
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
        auto message = context.defineVar(this->ident->name);
        if (!message.success){
            this->printLocation();
            std::cerr << "|Error: " << message.message << std::endl<<std::endl;
        }
        auto exprType = this->expr->getType();
        if (message.type == Type::NONE)
        {
            context.ChangeVarType(this->ident->name, exprType);
            message = context.checkVar(this->ident->name);
        }
        if (message.type != exprType){
            this->printLocation();
            std::cerr << "|Error: Type mismatch" << std::endl;
            std::cerr << "|wanted: " << innerType[message.type] << " got: " << innerType[exprType] << std::endl<<std::endl;
        }
        this->expr->optimize();
    }
}