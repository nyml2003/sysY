#ifndef SYSY_COMPILER_SCOPE_HPP
#define SYSY_COMPILER_SCOPE_HPP
#include <vector>
#include <map>
#include <string>
#include "AST.hpp"
#include "Value.hpp"
namespace Compiler::Scope
{
    struct Symbol
    {
        explicit Symbol(std::string name, Compiler::AST::InnerType type, int32_t intValue);
        explicit Symbol(std::string name, Compiler::AST::InnerType type, float floatValue);
        explicit Symbol(std::string name, Compiler::AST::InnerType type);
        std::string name;
        Compiler::AST::InnerType type;
        bool isDefined;
        std::shared_ptr<Value> value;
    };

    struct Message
    {
        bool success;
        std::string content;
    };

    struct Context
    {
        std::vector< std::map<std::string, std::unique_ptr<Symbol> > > symbolTable;
        Message insert(std::string name, Compiler::AST::InnerType type, int32_t intValue);
        Message insert(std::string name, Compiler::AST::InnerType type, float floatValue);
        Message insert(std::string name, Compiler::AST::InnerType type);
        Message confirm(std::string name);
        Message find(std::string name);
        void enterScope();
        void exitScope();
        
    };
}
inline auto context = Compiler::Scope::Context();
#endif // SYSY_COMPILER_SCOPE_HPP