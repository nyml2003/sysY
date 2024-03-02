//
// Created by 风唤长河 on 2024/2/28.
//

#include "Identifier.hpp"
#include <iostream>
#include <Scope.hpp>
namespace Compiler::AbstractSyntaxTree::Node {
    Identifier::Identifier(const std::string &name) {
        this->name = name;
        this->type = Type::NONE;
        this->typeName = "Ident";
    }

    void Ident::toMermaid()
    {
        std::cout << this->id << "[" << this->typeName << ": " << this->name << "]" << std::endl;
    }

    void Ident::toIR()
    {
        // TODO
    }

    ExprPtr Ident::constantFold()
    {
        return nullptr;
    }

    bool Ident::isConstant()
    {
        return false;
    }

    Type Ident::getType()
    {
        if (context.isReady){
            auto message = context.checkVar(this->name);
            if (!message.success){
                this->printLocation();
                std::cerr << "|Error: " << message.message << std::endl<<std::endl;
            }
            this->type = message.type;
        }
        return this->type;
    }
}