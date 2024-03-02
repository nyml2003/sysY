//
// Created by 风唤长河 on 2024/2/23.
//

#ifndef SYSY_COMPILER_ABSTRACT_SYNTAX_TREE_STATEMENT_HPP
#define SYSY_COMPILER_ABSTRACT_SYNTAX_TREE_STATEMENT_HPP
#include "Expression.hpp"
#include "Block.hpp"
#include "Identifier.hpp"
#include "Definition.hpp"
namespace Compiler::AbstractSyntaxTree::Node {
    class Statement : virtual public BlockItem {
    public:
        virtual void toMermaid()  = 0;
        virtual void toIR() = 0;
        virtual void optimize() = 0;
    };
    class AssignStatement : public Statement {
    public:
        explicit AssignStatement(IdentPtr ident, ExprPtr expr);
        DefPtr def;
        void toMermaid() override;
        void toIR() override;
        void optimize() override;
    };

    class ReturnStatement : public Statement {
    public:
        explicit ReturnStatement(ExprPtr expr);
        ExprPtr expr;
        void toMermaid() override;
        void toIR() override;
        void optimize() override;
    };
}
using Stmt = Compiler::AbstractSyntaxTree::Node::Statement;
using StmtPtr = std::unique_ptr<Stmt>;
using StmtList = std::vector<StmtPtr>;
using AssignStmt = Compiler::AbstractSyntaxTree::Node::AssignStatement;
using AssignPtr = std::unique_ptr<AssignStmt>;
using ReturnStmt = Compiler::AbstractSyntaxTree::Node::ReturnStatement;
using ReturnPtr = std::unique_ptr<ReturnStmt>;
#endif //SYSY_COMPILER_ABSTRACT_SYNTAX_TREE_STATEMENT_HPP
