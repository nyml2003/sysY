//
// Created by 风唤长河 on 2024/2/28.
//

#ifndef FLEX_BISON_LLVM_IDENTIFIER_HPP
#define FLEX_BISON_LLVM_IDENTIFIER_HPP
#include "Expression.hpp"
#include <map>
#include "Type.hpp"
namespace Compiler::AbstractSyntaxTree::Node {
    class Identifier : public Expr {
    public:
        explicit Identifier(const std::string& name);
        std::string name;
        Type type;
        void toMermaid() override;
        void toIR() override;
        ExprPtr constantFold() override;
        bool isConstant() override;
        Type getType() override;
    };
}
using Ident = Compiler::AbstractSyntaxTree::Node::Identifier;
using IdentPtr = std::unique_ptr<Ident>;
#endif //FLEX_BISON_LLVM_IDENTIFIER_HPP
