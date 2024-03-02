//
// Created by 风唤长河 on 2024/3/1.
//

#include "Declaration.hpp"
namespace Compiler::AbstractSyntaxTree::Node {
    Declaration::Declaration(Type type, IdentPtr ident) : type(type), ident(std::move(ident)) {
        this->typeName = "Declaration";
    }

    void Declaration::toMermaid() {
        std::cout << this->id << "[" << this->typeName << "]" << std::endl;
        std::cout << this->id << "--type-->" << maxId+this->id << "[" << innerType[this->type] << "]" << std::endl;
        std::cout << this->id << "--name-->" << this->ident->id << std::endl;
        this->ident->toMermaid();
    }

    void Declaration::toIR() {
        // TODO
    }

    void Declaration::optimize() {
        // TODO
    }
}