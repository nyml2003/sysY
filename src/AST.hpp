#ifndef SED_COMPILER_AST_HPP
#define SED_COMPILER_AST_HPP
#include <cstddef>
#include <memory>
#include <vector>
#include <string>
#include "location.hh"
namespace Compiler::AST
{
    enum class InnerType
    {
        INT,
        VOID,
        FLOAT,
        ARRAY
    };
    inline std::vector<std::string> innerType = {"int", "void", "float", "array"};
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
    enum class NODE_TYPE
    {
        COMP_UNIT,
        DECL,
        TYPE,
        DEF,
        LVAL,
        INIT_VAL,
        FUNC_DEF,
        BLOCK,
        ASSIGN_STMT,
        EXP_STMT,
        IF_STMT,
        WHILE_STMT,
        BREAK_STMT,
        CONTINUE_STMT,
        RETURN_STMT,
        INT32,
        FLOAT32,
        UNARY_EXP,
        BINARY_EXP,
        IDENT,
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
        /**
         * @brief 分析抽象语法树
        */
        virtual void analyze() = 0;
        size_t nodeId;
        static size_t maxASTNodeId;
        yy::position begin;
        yy::position end;
        NODE_TYPE nodeType;
        void printLocation(const std::string& message);
    };
    using NodePtr = std::unique_ptr<Node>;

    struct CompUnit : public Node
    {
        explicit CompUnit();
        void toMermaid() override;
        void analyze() override;
        void attach(std::vector<NodePtr> chilren);
        std::vector<NodePtr> children;
    };

    struct Decl : public Node
    {
        explicit Decl(NodePtr type, std::vector<NodePtr> defList);
        void analyze() override;
        void toMermaid() override;
        void addDecorator(Decorator decorator);
        NodePtr type;
        std::vector<NodePtr> defList;
        std::vector<Decorator> decorators;
    };

    struct Type : public Node
    {
        explicit Type();
        void analyze() override;
        explicit Type(InnerType type);
        void toMermaid() override;
        InnerType type;
    };

    struct Def : public Node
    {
        void analyze() override;
        explicit Def(NodePtr lval, NodePtr initVal);
        void toMermaid() override;
        NodePtr lval;
        NodePtr initVal;
    };

    struct Lval : public Node
    {
        void analyze() override;
        explicit Lval(NodePtr ident, std::vector<NodePtr> dim);
        explicit Lval(NodePtr ident);
        void toMermaid() override;
        NodePtr ident;
        std::vector<NodePtr> dim;
    };

    struct InitVal : public Node
    {
        void analyze() override;
        explicit InitVal(NodePtr child);
        explicit InitVal(std::vector<NodePtr> children);
        void toMermaid() override;
        std::vector<NodePtr> children;
    };

    struct FuncDef : public Node
    {
        void analyze() override;
        explicit FuncDef(NodePtr retType, NodePtr ident, NodePtr block);
        void toMermaid() override;
        NodePtr retType;
        NodePtr ident;
        NodePtr block;
    };

    struct Block : public Node
    {
        void analyze() override;
        explicit Block();
        explicit Block(std::vector<NodePtr> children);
        void toMermaid() override;
        std::vector<NodePtr> children;
    };

    struct AssignStmt : public Node
    {
        void analyze() override;
        explicit AssignStmt(NodePtr lval, NodePtr expr);
        void toMermaid() override;
        NodePtr lval;
        NodePtr expr;
    };

    struct ExpStmt : public Node
    {
        void analyze() override;
        explicit ExpStmt(NodePtr expr);
        void toMermaid() override;
        NodePtr expr;
    };

    struct IfStmt : public Node
    {
        void analyze() override;
        explicit IfStmt(NodePtr cond, NodePtr thenStmt, NodePtr elseStmt);
        explicit IfStmt(NodePtr cond, NodePtr thenStmt);
        void toMermaid() override;
        NodePtr cond;
        NodePtr thenStmt;
        NodePtr elseStmt;
    };

    struct WhileStmt : public Node
    {
        void analyze() override;
        explicit WhileStmt(NodePtr expr, NodePtr stmt);
        void toMermaid() override;
        NodePtr expr;
        NodePtr stmt;
    };

    struct BreakStmt : public Node
    {
        void analyze() override;
        explicit BreakStmt();
        void toMermaid() override;
    };

    struct ContinueStmt : public Node
    {
        void analyze() override;
        explicit ContinueStmt();
        void toMermaid() override;
    };

    struct ReturnStmt : public Node
    {
        void analyze() override;
        explicit ReturnStmt(NodePtr expr);
        void toMermaid() override;
        NodePtr expr;
    };

    struct Int32 : public Node
    {
        void analyze() override;
        explicit Int32(int32_t val);
        void toMermaid() override;
        int32_t val;
    };

    struct Float32 : public Node
    {
        void analyze() override;
        explicit Float32(float val);
        void toMermaid() override;
        float val;
    };

    struct UnaryExp : public Node
    {
        void analyze() override;
        explicit UnaryExp(Operator op, NodePtr expr);
        void toMermaid() override;
        Operator op;
        NodePtr expr;
    };

    struct BinaryExp : public Node
    {
        void analyze() override;
        explicit BinaryExp(NodePtr left, Operator op, NodePtr right);
        void toMermaid() override;
        NodePtr left;
        Operator op;
        NodePtr right;
    };

    struct Ident : public Node
    {
        void analyze() override;
        explicit Ident(std::string& name);
        void toMermaid() override;
        std::string name;
    };
}

#endif // SED_COMPILER_AST_HPP