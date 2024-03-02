//
// Created by 风唤长河 on 2024/3/1.
//

#include "Declaration.hpp"
#include "Scope.hpp"
namespace Compiler::AbstractSyntaxTree::Node {
    Declaration::Declaration(Type type, IdentPtr ident) {
        this->typeName = "Declaration";
        this->ident = std::move(ident);
        this->type = type;
        if (this->begin.column > this->ident->begin.column){
            this->begin = this->ident->begin;
        }
        if (this->end.column < this->ident->end.column){
            this->end = this->ident->end;
        }
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
        auto message = context.declareVar(this->ident->type, this->ident->name);
        if (!message.success){
            this->printLocation();
            std::cerr << "|Error: " << message.message << std::endl<<std::endl;
        }
    }
}