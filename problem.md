```cpp
namespace Compiler::AbstractSyntaxTree::Node {
    class Node;
}
using Node = Compiler::AbstractSyntaxTree::Node::Node;
using NodePtr = std::unique_ptr<Node>;
using NodePtrOrVoid = std::optional<NodePtr>;

class Expression : public Node
    {
    public:
        Expression() = default;
        void toMermaid() override =0;
        void toIR() override =0;
        NodePtrOrVoid constantFold() override = 0;
        virtual bool isConstant()=0;
        ~Expression() override = default;
    };
using Expr = Compiler::AbstractSyntaxTree::Node::Expression;
using ExprPtr = std::unique_ptr<Expr>;

NodePtrOrVoid BinaryExpression::constantFold() {
    if (this->isConstant()) {
        return std::make_unique<Int32>(
                eval(
                        this->op,
                        dynamic_cast<Int32 *>(this->left.get())->value,
                        dynamic_cast<Int32 *>(this->right.get())->value));
    } else {
        return std::nullopt;
    }
}

NodePtrOrVoid Stmt::constantFold() {
    // this->expr = std::move(this->expr->constantFold());
}
```