//
// Created by 风唤长河 on 2024/2/28.
//
#include "Scope.hpp"
#include <iostream>
namespace Compiler::Scope {
    void Context::defineVar(std::string name) {
        if (this->symbolTable.back().find(name) == this->symbolTable.back().end()) {
            std::cerr << "Error: " << name << " is not declared." << std::endl;
        }
        this->symbolTable.back()[name] = this->symbolTable.size();
    }

    void Context::declareVar(std::string name) {
        if (this->symbolTable.back().find(name) != this->symbolTable.back().end()) {
            std::cerr << "Error: " << name << " is already declared." << std::endl;
        }
        this->symbolTable.back()[name] = this->symbolTable.size();
    }
}