#ifndef SYSY_COMPILER_SCOPE_HPP
#define SYSY_COMPILER_SCOPE_HPP
#include <vector>
#include <map>
#include <string>
#include "AST.hpp"
#include <variant>
#include <optional>
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
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
        Message insert(std::string name, Compiler::AST::InnerType type);
        Message confirm(std::string name);
        Message find(std::string name);
        void enterScope();
        void exitScope();
        std::vector<std::map<std::string, std::variant<int32_t, float, std::monostate> > > constTable;
        Message setValue(std::string name, std::variant<int32_t, float, std::monostate> value);

        std::vector<llvm::BasicBlock*> blockStack;
        std::vector<llvm::Function*> functionStack;
        std::vector<llvm::GlobalVariable*> globalStack;
        std::vector<llvm::Value*> valueStack;
        std::vector<Compiler::AST::InnerType> typeStack;

        bool isGlobal(); //根据symbolTable判断是否为全局变量
        std::optional<bool> isConstant = std::nullopt; //当前是否为常量
    };
}
inline auto context = Compiler::Scope::Context();
inline auto llvmContext = llvm::LLVMContext();
inline auto IRBuilder = llvm::IRBuilder<>(llvmContext);
inline auto module = new llvm::Module("sysy", llvmContext);
#endif // SYSY_COMPILER_SCOPE_HPP