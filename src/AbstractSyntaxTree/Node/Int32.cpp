//
// Created by 风唤长河 on 2024/2/28.
//

#include "Int32.hpp"
#include <iostream>
namespace Compiler::AbstractSyntaxTree::Node {
    Int32::Int32(std::int32_t value) : value(value) {
        this->typeName = "Int32";
    }

    void Int32::toMermaid()
    {
        std::cout << this->id << "[" << this->value << "]" << std::endl;
    }

    void Int32::toIR()
    {
        // TODO
    }

    ExprPtr Int32::constantFold()
    {
        return nullptr;
    }

    bool Int32::isConstant()
    {
        return true;
    }

    Type Int32::getType()
    {
        return Type::INT;
    }
}