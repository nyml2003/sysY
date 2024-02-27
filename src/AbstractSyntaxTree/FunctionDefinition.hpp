//
// Created by venty on 2024/2/23.
//

#ifndef FLEX_BISON_LLVM_FUNCTION_DEFINITION_HPP
#define FLEX_BISON_LLVM_FUNCTION_DEFINITION_HPP
#include "Header.hpp"

namespace Compiler::AbstractSyntaxTree {
    class FunctionDefinition : public IAbstractSyntaxTree {
    public:
        explicit FunctionDefinition(std::unique_ptr<Type> type, std::unique_ptr<Expression::Identifier> identifier, std::unique_ptr<Block> block);
        std::unique_ptr<Type> type;
        std::unique_ptr<Expression::Identifier> identifier;
        std::unique_ptr<Block> block;
        void dump();
        void dumpMermaid();
        void dumpLLVM(llvm::Module* module);
        void optimize() override;
    };
}
#endif //FLEX_BISON_LLVM_FUNCTION_DEFINITION_HPP
