//
// Created by 风唤长河 on 2024/2/28.
//

#ifndef SYSY_COMPILER_SCOPE_HPP
#define SYSY_COMPILER_SCOPE_HPP
#include <vector>
#include <map>
#include <string>
#include "Type.hpp"
namespace Compiler::Scope
{
    struct Message
    {
        Type type;
        bool success;
        std::string message;
    };

    struct Context
    {
        explicit Context();
        std::vector<std::map<std::string, Type>> symbolTable;
        Message declareVar(Type type, std::string name);
        Message defineVar(std::string name);
        Message checkVar(std::string name);
        Message ChangeVarType(std::string name, Type type);
        void enterScope();
        void exitScope();
        void print();
        bool isReady;
    };
}
inline auto context = Compiler::Scope::Context();
#endif // SYSY_COMPILER_SCOPE_HPP