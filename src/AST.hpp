#ifndef SED_AST_HPP
#define SED_AST_HPP
#include <cstddef>
#include <memory>
#include <vector>
#include <string>

namespace Compiler::AST
{
    enum class InnerType
    {
        INT,
        VOID,
        FLOAT
    };
    inline std::vector<std::string> innerType = {"int", "void", "bool"};
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
    inline std::vector<std::string> operatorName = {
        "\"+\"",
        "\"-\"",
        "\"*\"",
        "\"/\"",
        "\"%\"",
        "\"&&\"",
        "\"||\"",
        "\"==\"",
        "\"!=\"",
        "\"<\"",
        "\">\"",
        "\"<=\"",
        "\">=\"",
        "\"!\"",
        "\"+\"",
        "\"-\"",
    };
    enum class Decorator
    {
        CONSTANT,
    };
    inline std::vector<std::string> decoratorName = {"const"};
    struct Node
    {
        explicit Node();
        virtual ~Node() = default;
        /**
         * @brief 输出mermaid格式的抽象语法树
        */
        virtual void toMermaid() = 0;
        size_t nodeId;
        static size_t maxASTNodeId;
    };
    using NodePtr = std::unique_ptr<Node>;

    struct CompUnit : public Node
    {
        void toMermaid() override;
        void attach(std::vector<NodePtr> chilren);
        std::vector<NodePtr> children;
    };

    struct Decl : public Node
    {
        explicit Decl(NodePtr type, std::vector<NodePtr> defList);
        void toMermaid() override;
        void addDecorator(Decorator decorator);
        NodePtr type;
        std::vector<NodePtr> defList;
        std::vector<Decorator> decorators;
    };

    struct Type : public Node
    {
        explicit Type(InnerType type);
        void toMermaid() override;
        InnerType type;
    };

    struct Def : public Node
    {
        explicit Def(NodePtr lval, NodePtr initVal);
        void toMermaid() override;
        NodePtr lval;
        NodePtr initVal;
    };

    struct Lval : public Node
    {
        explicit Lval(NodePtr ident, std::vector<NodePtr> dim);
        explicit Lval(NodePtr ident);
        void toMermaid() override;
        NodePtr ident;
        std::vector<NodePtr> dim;
    };

    struct InitVal : public Node
    {
        explicit InitVal(NodePtr child);
        explicit InitVal(std::vector<NodePtr> children);
        void toMermaid() override;
        std::vector<NodePtr> children;
    };

    struct FuncDef : public Node
    {
        explicit FuncDef(NodePtr retType, NodePtr ident, NodePtr block);
        void toMermaid() override;
        NodePtr retType;
        NodePtr ident;
        NodePtr block;
    };

    struct Block : public Node
    {
        explicit Block();
        explicit Block(std::vector<NodePtr> children);
        void toMermaid() override;
        std::vector<NodePtr> children;
    };

    struct AssignStmt : public Node
    {
        explicit AssignStmt(NodePtr lval, NodePtr expr);
        void toMermaid() override;
        NodePtr lval;
        NodePtr expr;
    };

    struct ExpStmt : public Node
    {
        explicit ExpStmt(NodePtr expr);
        void toMermaid() override;
        NodePtr expr;
    };

    struct IfStmt : public Node
    {
        explicit IfStmt(NodePtr cond, NodePtr thenStmt, NodePtr elseStmt);
        explicit IfStmt(NodePtr cond, NodePtr thenStmt);
        void toMermaid() override;
        NodePtr cond;
        NodePtr thenStmt;
        NodePtr elseStmt;
    };

    struct WhileStmt : public Node
    {
        explicit WhileStmt(NodePtr expr, NodePtr stmt);
        void toMermaid() override;
        NodePtr expr;
        NodePtr stmt;
    };

    struct BreakStmt : public Node
    {
        explicit BreakStmt();
        void toMermaid() override;
    };

    struct ContinueStmt : public Node
    {
        explicit ContinueStmt();
        void toMermaid() override;
    };

    struct ReturnStmt : public Node
    {
        explicit ReturnStmt(NodePtr expr);
        void toMermaid() override;
        NodePtr expr;
    };

    struct Int32 : public Node
    {
        explicit Int32(int32_t val);
        void toMermaid() override;
        int32_t val;
    };

    struct float32 : public Node
    {
        explicit float32(float val);
        void toMermaid() override;
        float val;
    };

    struct UnaryExp : public Node
    {
        explicit UnaryExp(Operator op, NodePtr expr);
        void toMermaid() override;
        Operator op;
        NodePtr expr;
    };

    struct BinaryExp : public Node
    {
        explicit BinaryExp(NodePtr left, Operator op, NodePtr right);
        void toMermaid() override;
        NodePtr left;
        Operator op;
        NodePtr right;
    };

    struct Ident : public Node
    {
        explicit Ident(std::string& name);
        void toMermaid() override;
        std::string name;
    };
}

#endif // SED_AST_HPP