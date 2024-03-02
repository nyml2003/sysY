//
// Created by venty on 2024/2/23.
//

#ifndef FLEX_BISON_LLVM_BLOCK_HPP
#define FLEX_BISON_LLVM_BLOCK_HPP
#include "Node/Node.hpp"
#include "Statement.hpp"
namespace Compiler::AbstractSyntaxTree {
    class Block : public Node {
    public:
        explicit Block(std::unique_ptr<Statement> statement);
        std::unique_ptr<Statement> statement;
        void dump() override;
        void dumpMermaid();
        void dumpLLVM(llvm::Function* function);
        void optimize() override;
    };
}
#endif //FLEX_BISON_LLVM_BLOCK_HPP
