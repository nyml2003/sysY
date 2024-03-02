//
// Created by 风唤长河 on 2024/2/23.
//

#ifndef SYSY_COMPILER_ABSTRACT_SYNTAX_TREE_TYPE_HPP
#define SYSY_COMPILER_ABSTRACT_SYNTAX_TREE_TYPE_HPP
#include "Node.hpp"
#include <map>
namespace Compiler::AbstractSyntaxTree::Node {
    enum class Type {
        UNDEFINED,
        INT,
        VOID,
        FLOAT,
    };
    inline std::map<Type, std::string> typeName = {
            {Type::INT, "int"},
            {Type::VOID, "void"},
            {Type::FLOAT, "float"},
    };
}
using Type = Compiler::AbstractSyntaxTree::Node::Type;
inline auto& innerType = Compiler::AbstractSyntaxTree::Node::typeName;
#endif //SYSY_COMPILER_ABSTRACT_SYNTAX_TREE_TYPE_HPP
