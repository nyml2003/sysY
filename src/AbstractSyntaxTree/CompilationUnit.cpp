//
// Created by venty on 2024/2/23.
//
#include "CompilationUnit.hpp"
#include <iostream>
#include <llvm/IR/Module.h>
namespace Compiler::AbstractSyntaxTree {
    CompilationUnit::CompilationUnit(std::unique_ptr<FunctionDefinition> functionDefinition) : functionDefinition(std::move(functionDefinition)) {}
    void CompilationUnit::dump(){
        this->optimize();
        switch(dumpType){
            case DumpType::AST_MERMAID:
                dumpMermaid();
                break;
            case DumpType::IR_LLVM:
                dumpLLVM();
                break;
            default:
                std::cerr << "CompilationUnit::dump() is not implemented" << std::endl;
                
        }
    }
    void CompilationUnit::dumpMermaid(){
        std::cout<<"```mermaid\n";
        std::cout<<"graph TD\n";
        std::cout<<id << "[" << "CompUnit" << "]\n";
        std::cout<<id << "-->" << functionDefinition->id << std::endl;
        functionDefinition->dumpMermaid();
        std::cout<<"```\n";
    }

    void CompilationUnit::dumpLLVM(){
        std::unique_ptr<llvm::Module> module = std::make_unique<llvm::Module>("main", context);
        functionDefinition->dumpLLVM(std::move(module));
        module->print(llvm::outs(), nullptr);
    }

    void CompilationUnit::optimize() {
        functionDefinition->optimize();
    }
}