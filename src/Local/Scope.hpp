//
// Created by 风唤长河 on 2024/2/28.
//

#ifndef SYSY_COMPILER_SCOPE_HPP
#define SYSY_COMPILER_SCOPE_HPP
#include <vector>
#include <map>
#include <string>
namespace Compiler::Scope
{
    struct Context
    {
        explicit Context()=default;
        std::vector<std::map<std::string, size_t>> symbolTable;
        void declareVar(std::string name);
        void defineVar(std::string name);
        void declareAndDefineVar(std::string name);
    };
}

#endif // SYSY_COMPILER_SCOPE_HPP