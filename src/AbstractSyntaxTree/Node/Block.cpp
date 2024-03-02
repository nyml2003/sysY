//
// Created by 风唤长河 on 2024/2/23.
//
#include "Block.hpp"
#include <iostream>
namespace Compiler::AbstractSyntaxTree::Node {
    Block::Block() {
        this->typeName = "block";
    }

    Block::Block(std::vector<BlockItemPtr>  items) {
        this->items = std::move(items);
        this->typeName = "block";
    }

    void Block::toMermaid() {
        std::cout<< this->id << "[" << this->typeName << "]" << std::endl;
        for (auto &item : this->items){
            std::cout << this->id << "-->" << item->id << std::endl;
            item->toMermaid();
        }
    }

    void Block::append(BlockItemPtr item) {
        this->items.push_back(std::move(item));
    }

    void Block::append(std::vector<BlockItemPtr> items) {
        for (auto &item : items) {
            this->items.push_back(std::move(item));
        }
    }

    void Block::toIR() {
        // TODO
    }

    void Block::optimize()
    {
        for (auto &item : this->items) {
            item->optimize();
        }
    }

}