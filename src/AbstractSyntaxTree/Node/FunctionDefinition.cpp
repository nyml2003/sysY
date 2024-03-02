//
// Created by 风唤长河 on 2024/2/23.
//
#include "FunctionDefinition.hpp"
#include <iostream>
namespace Compiler::AbstractSyntaxTree::Node {
    FunctionDefinition::FunctionDefinition(Type type, IdentPtr ident, BlockPtr block) : type(type), ident(std::move(ident)), block(std::move(block)) {
        this->typeName = "FuncDef";
        this->begin = this->ident->begin;
        this->end = this->block->end;
    }

    void FuncDef::toMermaid() {
        std::cout << this->id << "[" << this->typeName << "]" << std::endl;
        std::cout << this->id << "--" << "type" << "-->" << 
        maxId+this->id << "[" << innerType[this->type] << "]" << std::endl;
        std::cout << this->id << "--" << "name" << "-->" << this->ident->id << std::endl;
        this->ident->toMermaid();
        std::cout << this->id << "-->" << this->block->id << std::endl;
        this->block->toMermaid();
    }

    void FuncDef::toIR() {
        // TODO
    }

    void FuncDef::optimize() {
        this->block->optimize();
    }
}