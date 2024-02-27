//
// Created by venty on 2024/2/23.
//

#ifndef FLEX_BISON_LLVM_STATEMENT_HPP
#define FLEX_BISON_LLVM_STATEMENT_HPP
#include "IAbstractSyntaxTree.hpp"
#include "Expression.hpp"

namespace Compiler::AbstractSyntaxTree {
    class Statement : public IAbstractSyntaxTree {
    public:
        explicit Statement(std::unique_ptr<Expression::IExpression>);
        std::unique_ptr<Expression::IExpression> expression;
        void dump();
        void dumpMermaid();
        void dumpLLVM();
        void optimize() override;
    };
}
#endif //FLEX_BISON_LLVM_STATEMENT_HPP
