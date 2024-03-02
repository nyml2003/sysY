//
// Created by venty on 2024/2/23.
//

#include "Header.hpp"
#include <iostream>
namespace Compiler::AbstractSyntaxTree {
    Block::Block(std::unique_ptr<Statement> statement) : statement(std::move(statement)) {}
    void Block::dump() {
        switch (dumpType) {
            case DumpType::AST_MERMAID:
                dumpMermaid();
                break;
            case DumpType::IR_LLVM:
                //dumpLLVM();
                break;
        }
    }

    void Block::dumpMermaid() {
        std::cout<< id << "[" << "Block" << "]\n";
        std::cout<< id << "-->" << statement->id << std::endl;
        statement->dump();
    }

    void Block::dumpLLVM(llvm::Function* function) {
        llvm::BasicBlock* block = llvm::BasicBlock::Create(context, "entry", function);
        builder.SetInsertPoint(block);
        statement->dumpLLVM();
    }

    void Block::optimize() {
        statement->optimize();
    }
}