//
// Created by 风唤长河 on 2024/2/29.
//
#include "Program.hpp"
#include <iostream>
#include "Scope.hpp"
namespace Compiler::AbstractSyntaxTree::Node {
    Program::Program() {
        this->block = std::make_unique<Block>();
        this->typeName = "program";
        this->begin = this->block->begin;
        this->end = this->block->end;
    }

    void Program::toMermaid() {
        std::cout << "```mermaid" << std::endl;
        std::cout << "graph TD" << std::endl;
        std::cout << this->id << "[" << this->typeName << "]" << std::endl;
        std::cout << this->id << "-->" << this->block->id << std::endl;
        this->block->toMermaid();
        std::cout << "```" << std::endl;
    }
    void Program::toIR() {
        // TODO
    }

    void Program::optimize() {
        context.isReady = true;
        this->block->optimize();
    }
}