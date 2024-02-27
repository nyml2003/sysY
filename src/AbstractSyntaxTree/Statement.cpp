//
// Created by venty on 2024/2/23.
//

#include "Header.hpp"
#include <iostream>
namespace Compiler::AbstractSyntaxTree {
    Statement::Statement(std::unique_ptr<Expression::IExpression> expression) : expression(std::move(expression)) {}
    void Statement::dump(){
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

    void Statement::dumpMermaid() {
        std::cout<< id << "[" << "Statement" << "]\n";
        std::cout << id << "-->" << expression->id << "\n";
        expression->dump();
    }

    void Statement::dumpLLVM() {
        llvm::Value* value = expression->dumpLLVM();
        builder.CreateRet(value);
    }

    void Statement::optimize() {
        expression = std::move(expression->constantFold());
    }
}