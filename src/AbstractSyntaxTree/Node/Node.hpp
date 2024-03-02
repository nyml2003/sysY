//
// Created by venty on 2024/2/23.
//

#ifndef SYSY_COMPILER_ABSTRACT_SYNTAX_TREE_NODE_HPP
#define SYSY_COMPILER_ABSTRACT_SYNTAX_TREE_NODE_HPP

#include <memory>
#include <vector>
#include <string>

namespace Compiler::AbstractSyntaxTree::Node {
    class Node {
    public:
        explicit Node();
        virtual ~Node() = default;
        void generate();
        size_t id;
        std::string typeName;
        std::vector<std::unique_ptr<Node> > children;
    private:
        static size_t maxId;
    };
}
#endif //FLEX_BISON_LLVM_INTERFACE_ABSTRACT_SYNTAX_TREE_HPP