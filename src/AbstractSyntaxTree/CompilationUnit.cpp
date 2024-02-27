//
// Created by venty on 2024/2/23.
//
#include "CompilationUnit.hpp"
#include <iostream>
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
        llvm::Module* module = new llvm::Module("top", context);
        functionDefinition->dumpLLVM(module);
        module->print(llvm::outs(), nullptr);
        std::cout<<"end of module\n";
    }

    void CompilationUnit::optimize() {
        functionDefinition->optimize();
    }
}