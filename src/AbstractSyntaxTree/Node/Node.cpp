//
// Created by 风唤长河 on 2024/2/23.
//

#include "Node.hpp"
namespace Compiler::AbstractSyntaxTree::Node {
    Node::Node(){
        this->id = ++maxId;
    }
}