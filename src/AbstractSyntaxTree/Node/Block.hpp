//
// Created by 风唤长河 on 2024/2/23.
//

#ifndef SYSY_COMPILER_ABSTRACT_SYNTAX_TREE_BLOCK_HPP
#define SYSY_COMPILER_ABSTRACT_SYNTAX_TREE_BLOCK_HPP
#include <vector>
#include "Type.hpp"
namespace Compiler::AbstractSyntaxTree::Node {
    class BlockItem : virtual public Node
    {
    public:
        void toMermaid() override = 0;
        void toIR() override = 0;
        void optimize() override = 0;
    };
    class Block : public Node {
    public:
        explicit Block();
        explicit Block(std::vector< std::unique_ptr<BlockItem> > items);
        std::vector< std::unique_ptr<BlockItem> >  items;
        void append(std::unique_ptr<BlockItem> item);
        void append(std::vector<std::unique_ptr<BlockItem>> items);
        void toMermaid() override;
        void toIR() override;
        void optimize() override;
    };
}
using Block = Compiler::AbstractSyntaxTree::Node::Block;
using BlockPtr = std::unique_ptr<Block>;
using BlockItem = Compiler::AbstractSyntaxTree::Node::BlockItem;
using BlockItemPtr = std::unique_ptr<BlockItem>;
using BlockItemList = std::vector<BlockItemPtr>;
#endif //SYSY_COMPILER_ABSTRACT_SYNTAX_TREE_BLOCK_HPP
