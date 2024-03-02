//
// Created by 风唤长河 on 2024/2/23.
//

#ifndef SYSY_COMPILER_ABSTRACT_SYNTAX_TREE_NODE_HPP
#define SYSY_COMPILER_ABSTRACT_SYNTAX_TREE_NODE_HPP

#include <memory>
#include <vector>
#include <string>
#include <iostream>
namespace Compiler::AbstractSyntaxTree::Node {
    class Node;
}

namespace Compiler::AbstractSyntaxTree::Node {
    inline size_t maxId = 0;
    class Node {
    public:
        explicit Node();
        virtual ~Node() = default;
        virtual void toMermaid()=0;
        virtual void toIR()=0;
        virtual void optimize()=0;
        size_t id;
        std::string typeName;
    };
}
using Node = Compiler::AbstractSyntaxTree::Node::Node;
using NodePtr = std::unique_ptr<Node>;
using NodeList = std::vector<NodePtr>;
inline auto& maxId = Compiler::AbstractSyntaxTree::Node::maxId;
#endif //SYSY_COMPILER_ABSTRACT_SYNTAX_TREE_NODE_HPP