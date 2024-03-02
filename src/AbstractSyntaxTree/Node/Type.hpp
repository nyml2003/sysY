//
// Created by venty on 2024/2/23.
//

#ifndef FLEX_BISON_LLVM_TYPE_HPP
#define FLEX_BISON_LLVM_TYPE_HPP
#include "Header.hpp"
#include <string>
namespace Compiler::AbstractSyntaxTree {
    class Type : public Node {
    public:
        explicit Type(std::string value);
        std::string value;
        void dump();
        void dumpMermaid();
    };
}
#endif //FLEX_BISON_LLVM_TYPE_HPP
