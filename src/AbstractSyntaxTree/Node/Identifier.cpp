//
// Created by 风唤长河 on 2024/2/28.
//

#include "Identifier.hpp"
#include <iostream>
namespace Compiler::AbstractSyntaxTree::Node {
    Identifier::Identifier(const std::string &name) {
        this->name = name;
        this->type = Type::VOID;
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
}