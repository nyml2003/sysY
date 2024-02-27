//
// Created by venty on 2024/2/23.
//

#include "IAbstractSyntaxTree.hpp"
#include <iostream>

namespace Compiler::AbstractSyntaxTree {
    
    IAbstractSyntaxTree::IAbstractSyntaxTree(){
        id = idCounter++;
    }
    void IAbstractSyntaxTree::dump()
    {
        std::cerr << "IAbstractSyntaxTree::dump() is not implemented" << std::endl;
    }
    void IAbstractSyntaxTree::optimize()
    {
        std::cerr << "IAbstractSyntaxTree::optimize() is not implemented" << std::endl;
    }
}