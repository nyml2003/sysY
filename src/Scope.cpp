#include "Scope.hpp"

namespace Compiler::Scope
{

    Message Context::insert(std::string name, Compiler::AST::InnerType _Type)
    {
        if (this->symbolTable.back().find(name) != this->symbolTable.back().end())
        {
            return Message{false, "redefined variable", std::monostate{}};
        }
        this->symbolTable.back()[name] = _Type;
        return Message{true, "", std::monostate{}};
    }

    Message Context::confirm(std::string name)
    {
        for (auto it = this->symbolTable.rbegin(); it != this->symbolTable.rend(); it++)
        {
            if (it->find(name) != it->end())
            {
                return Message{true, "", std::monostate{}};
            }
        }
        return Message{false, "undecleared variable", std::monostate{}};
    }

    Message Context::find(std::string name)
    {
        for (auto it = this->constTable.rbegin(); it != this->constTable.rend(); it++)
        {
            if (it->find(name) != it->end())
            {
                if (std::holds_alternative<int32_t>(it->at(name)))
                {
                    return Message{true, "", it->at(name)};
                }
                else if (std::holds_alternative<float>(it->at(name)))
                {
                    return Message{true, "", it->at(name)};
                }
                else
                {
                    return Message{false, "unconstant variable", std::monostate{}};
                }
            }
        }
        return Message{false, "undefined variable", std::monostate{}};
    }

    void Context::enterScope()
    {
        this->symbolTable.push_back(std::map<std::string, Compiler::AST::InnerType>());
        this->constTable.push_back(std::map<std::string, std::variant<int32_t, float, std::monostate>>());
    }

    void Context::exitScope()
    {
        this->symbolTable.pop_back();
        this->constTable.pop_back();
    }
    Message Context::setValue(std::string name, std::variant<int32_t, float, std::monostate> value)
    {
        if (this->constTable.back().find(name) != this->constTable.back().end())
        {
            return Message{false, "redefined constant", std::monostate{}};
        }
        this->constTable.back()[name] = value;
        return Message{true, "", std::monostate{}};
    }

    bool Context::isGlobal()
    {
        return this->symbolTable.size() == 1;
    }
}