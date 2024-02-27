//
// Created by venty on 2024/2/23.
//

#include "Type.hpp"
#include <iostream>
namespace Compiler::AbstractSyntaxTree {
    Type::Type(std::string value) : value(std::move(value)) {}

    void Type::dump() {
        switch (dumpType) {
            case DumpType::AST_MERMAID:
                dumpMermaid();
                break;
            case DumpType::IR_LLVM:
                break;
            default:
                std::cerr << "FunctionDefinition::dump() is not implemented" << std::endl;
        }
    }

    void Type::dumpMermaid() {
        std::cout<<id<<"["<<value<<"]\n";
    }
}