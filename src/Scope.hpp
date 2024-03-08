#ifndef SYSY_COMPILER_SCOPE_HPP
#define SYSY_COMPILER_SCOPE_HPP
#include <vector>
#include <map>
#include <string>
#include "AST.hpp"
#include <variant>
namespace Compiler::Scope
{

    struct Message
    {
        bool success;
        std::string content;
        std::variant<int32_t, float,std::monostate> value;
    };

    struct Context
    {
        std::vector< std::map<std::string, Compiler::AST::InnerType> > symbolTable;
        std::map<std::string, size_t> memoryTable;
        Message insert(std::string name, Compiler::AST::InnerType type);
        Message confirm(std::string name);
        Message find(std::string name);
        void enterScope();
        void exitScope();
        std::vector<std::map<std::string, std::variant<int32_t, float, std::monostate> > > constTable;
        Message setValue(std::string name, std::variant<int32_t, float, std::monostate> value);
    };
}
inline auto context = Compiler::Scope::Context();
#endif // SYSY_COMPILER_SCOPE_HPP