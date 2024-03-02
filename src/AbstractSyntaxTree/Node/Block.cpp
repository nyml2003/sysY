//
// Created by 风唤长河 on 2024/2/23.
//
#include "Block.hpp"
#include "Scope.hpp"
#include <iostream>
#include "Driver.hpp"
namespace Compiler::AbstractSyntaxTree::Node {
    Block::Block() {
        this->typeName = "block";
    }

    Block::Block(std::vector<BlockItemPtr>  items) {
        this->items = std::move(items);
        this->typeName = "block";
        this->begin = this->items[0]->begin;
        this->end = this->items[this->items.size()-1]->end;
    }

    void Block::toMermaid() {
        std::cout<< this->id << "[" << this->typeName << "]" << std::endl;
        for (size_t i = 0; i < this->items.size(); i++){
            std::cout << this->id << "--item" << i << "-->" <<
            this->items[i]->id << std::endl;
            this->items[i]->toMermaid();
        }
    }

    void Block::append(BlockItemPtr item) {
        this->items.push_back(std::move(item));
        this->end = this->items[this->items.size()-1]->end;
    }

    void Block::append(std::vector<BlockItemPtr> items) {
        for (auto &item : items) {
            this->items.push_back(std::move(item));
        }
        this->end = this->items[this->items.size()-1]->end;
    }

    void Block::toIR() {
        // TODO
    }

    void Block::optimize()
    {
        context.enterScope();
        for (auto it = this->items.begin(); it != this->items.end(); it++){
            (*it)->optimize();
        }
        context.exitScope();
    }

}