//
// Created by venty on 2024/2/24.
//

#ifndef FLEX_BISON_LLVM_EXPRESSION_HPP
#define FLEX_BISON_LLVM_EXPRESSION_HPP

#include "Header.hpp"
#include <map>
#include <string>

namespace Compiler::AbstractSyntaxTree::Expression
{
    std::int32_t eval(Operator op, std::int32_t left, std::int32_t right);
    std::int32_t eval(Operator op, std::int32_t value);
    enum class Operator
    {
        ADD,
        SUB,
        MUL,
        DIV,
        MOD,
        AND,
        OR,
        EQ,
        NE,
        LT,
        GT,
        LE,
        GE,
        NOT,
        PLUS,
        MINUS,
    };
    static std::map<Operator, std::string> opName = {
        {Operator::ADD, "\"+\""},
        {Operator::SUB, "\"-\""},
        {Operator::MUL, "\"*\""},
        {Operator::DIV, "\"/\""},
        {Operator::MOD, "\"%\""},
        {Operator::AND, "\"&&\""},
        {Operator::OR, "\"||\""},
        {Operator::EQ, "\"==\""},
        {Operator::NE, "\"!=\""},
        {Operator::LT, "\"<\""},
        {Operator::GT, "\">\""},
        {Operator::LE, "\"<=\""},
        {Operator::GE, "\">=\""},
        {Operator::NOT, "\"!\""},
        {Operator::PLUS, "\"+\""},
        {Operator::MINUS, "\"-\""},
    };

    class IExpression : public Node
    {
    public:
        virtual void dump() override;

        virtual llvm::Value *dumpLLVM();

        virtual std::unique_ptr<IExpression> constantFold();

        virtual bool isConstant();

        virtual ~IExpression() = default;

        void optimize() override;
    };

    class BinaryExpression : public IExpression
    {
    public:
        explicit BinaryExpression(std::unique_ptr<IExpression> left, std::unique_ptr<IExpression> right, Operator op);
        std::unique_ptr<IExpression> left;
        std::unique_ptr<IExpression> right;
        Operator op;
        void dump() override;
        llvm::Value* dumpLLVM() override;
        std::unique_ptr<IExpression> constantFold() override;
        bool isConstant() override;
    };

    class UnaryExpression : public IExpression
    {
    public:
        UnaryExpression(std::unique_ptr<IExpression> expression, Operator op);

        std::unique_ptr<IExpression> expression;
        Operator op;

        void dump() override;
        llvm::Value* dumpLLVM() override;
        std::unique_ptr<IExpression> constantFold() override;

        bool isConstant() override;
    };

    class Int32 : public IExpression
    {
    public:
        explicit Int32(std::int32_t value);

        std::int32_t value{};

        void dump() override;
        llvm::Value* dumpLLVM() override;
        std::unique_ptr<IExpression> constantFold() override;

        bool isConstant() override;
    };

    class Identifier : public IExpression
    {
    public:
        explicit Identifier(const std::string &name);

        std::string name;

        void dump() override;
        llvm::Value* dumpLLVM() override;
        std::unique_ptr<IExpression> constantFold() override;

        bool isConstant() override;
    };
}
#endif // FLEX_BISON_LLVM_EXPRESSION_HPP
