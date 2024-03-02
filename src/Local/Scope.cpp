//
// Created by 风唤长河 on 2024/2/28.
//
#include "Scope.hpp"
#include <iostream>
#include "Driver.hpp"
namespace Compiler::Scope {
    Context::Context() {
        this->isReady = false;
    }

    Message Context::declareVar(Type type, std::string name)
    {
        for (auto it = this->symbolTable.rbegin(); it != this->symbolTable.rend(); it++) {
            if (it->find(name) != it->end()) {
                return {it->at(name), false, "redefined variable " + name};
            }
        }
        this->symbolTable.back()[name] = type;
        return {Type::UNDEFINED, true, ""};
    }

    Message Context::defineVar(std::string name)
    {
        auto result = this->symbolTable.back().find(name);
        if (result == this->symbolTable.back().end())
        {
            return {Type::UNDEFINED, false, "undefined variable " + name};
        }
        return {result->second, true, ""};
    }

    Message Context::checkVar(std::string name)
    {
        for (auto it = this->symbolTable.rbegin(); it != this->symbolTable.rend(); it++) {
            if (it->find(name) != it->end()) {
                return {it->at(name), true, ""};
            }
        }
        return {Type::UNDEFINED, false, "undefined variable " + name};
    }

    Message Context::ChangeVarType(std::string name, Type type)
    {
        for (auto it = this->symbolTable.rbegin(); it != this->symbolTable.rend(); it++) {
            if (it->find(name) != it->end()) {
                it->at(name) = type;
                return {type, true, ""};
            }
        }
        return {Type::UNDEFINED, false, "undefined variable " + name};
    }

    void Context::enterScope() {
        this->symbolTable.push_back(std::map<std::string, Type>());
    }

    void Context::exitScope() {
        this->symbolTable.pop_back();
    }

    void Context::print() {
        std::cout << "Symbol Table:" << std::endl;
        for (auto &scope : this->symbolTable) {
            for (auto &item : scope) {
                std::cout << item.first << " ";
            }
            std::cout << std::endl;
        }
    }
}