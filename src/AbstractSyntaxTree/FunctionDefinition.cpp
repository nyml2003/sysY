//
// Created by venty on 2024/2/23.
//

#include "Header.hpp"
#include "llvm/IR/Verifier.h"
#include <iostream>
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Function.h"
namespace Compiler::AbstractSyntaxTree {
    FunctionDefinition::FunctionDefinition(std::unique_ptr<Type> type, std::unique_ptr<Expression::Identifier> identifier, std::unique_ptr<Block> block) : type(std::move(type)), identifier(std::move(identifier)), block(std::move(block)) {}
    void FunctionDefinition::dump() {
        switch (dumpType) {
            case DumpType::AST_MERMAID:
                dumpMermaid();
                break;
            case DumpType::IR_LLVM:
                break;

                default:
                std::cerr << "FunctionDefinition::dump() is not implemented" << std::endl;
        }
    }

    void FunctionDefinition::dumpMermaid()  {
        std::cout<< id << "[" << "FunctionDefinition" << "]\n";
        std::cout<< id << "-->" << type->id << "\n";
        type->dump();
        std::cout<< id << "-->" << identifier->id << "\n";
        identifier->dump();
        std::cout<< id << "-->" << block->id << "\n";
        block->dump();
    }

    void FunctionDefinition::dumpLLVM(llvm::Module* module) {
        llvm::FunctionType *functionType = llvm::FunctionType::get(builder.getInt32Ty(), false);
        llvm::Function *function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, identifier->name, module);
        block->dumpLLVM(function);
        llvm::verifyFunction(*function);
    }

    void FunctionDefinition::optimize() {
        block->optimize();
    }
}