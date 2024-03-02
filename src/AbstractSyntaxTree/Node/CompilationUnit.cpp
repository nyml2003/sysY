//
// Created by 风唤长河 on 2024/2/23.
//
#include "CompilationUnit.hpp"
#include <iostream>
namespace Compiler::AbstractSyntaxTree::Node {

    CompilationUnit::CompilationUnit(FuncDefPtr funcDef) : funcDef(std::move(funcDef)) {
        this->typeName = "CompilationUnit";
    }
    void CompUnit::toMermaid() {
        std::cout << this->id << "[" << this->typeName << "]" << std::endl;
        std::cout << this->id << "-->" << this->funcDef->id << std::endl;
        this->funcDef->toMermaid();
    }

    void CompUnit::toIR() {
        // TODO
    }

    void CompUnit::optimize() {
        this->funcDef->optimize();
    }

}