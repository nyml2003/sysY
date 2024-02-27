//
// Created by venty on 2024/2/23.
//

#ifndef FLEX_BISON_LLVM_INTERFACE_ABSTRACT_SYNTAX_TREE_HPP
#define FLEX_BISON_LLVM_INTERFACE_ABSTRACT_SYNTAX_TREE_HPP
#include <memory>
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
namespace Compiler::AbstractSyntaxTree {
    enum class DumpType {
        None,
        AST_MERMAID,
        IR_LLVM,
    };
    
    static int idCounter = 0;
    static llvm::LLVMContext context;
    static llvm::IRBuilder<> builder(context);
    static DumpType dumpType;
    class IAbstractSyntaxTree {
    public:
        IAbstractSyntaxTree();
        virtual ~IAbstractSyntaxTree() = default;
        int id;
        virtual void dump();
        virtual void optimize();
    };
    class CompilationUnit;
    class FunctionDefinition;
    class Type;
    class Block;
    class Statement;
    namespace Expression {
        class IExpression;
        class BinaryExpression;
        class UnaryExpression;
        enum class Operator;
        class Int32;
        class Identifier;
    }
}
#endif //FLEX_BISON_LLVM_INTERFACE_ABSTRACT_SYNTAX_TREE_HPP