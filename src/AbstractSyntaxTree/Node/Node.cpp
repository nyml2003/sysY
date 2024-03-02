//
// Created by venty on 2024/2/23.
//

#include "Node.hpp"
#include "Driver.hpp"

namespace Compiler::AbstractSyntaxTree {
    using Driver = Compiler::Driver;
    extern Driver driver;
    Node::Node(){
        this->id = ++Node::maxId;
    }

    void Node::generate() {
        driver.generator.generate(*this);
    }
}