#include "Scope.hpp"

namespace Compiler::Scope {

    Symbol::Symbol(std::string name, Compiler::AST::InnerType type, int32_t intValue)
    {
        this->name = name;
        this->type = type;
        this->isDefined = false;
        this->value = std::make_shared<IntValue>(intValue);
    }

    Symbol::Symbol(std::string name, Compiler::AST::InnerType type, float floatValue)
    {
        this->name = name;
        this->type = type;
        this->isDefined = false;
        this->value = std::make_shared<FloatValue>(floatValue);
    }

    Symbol::Symbol(std::string name, Compiler::AST::InnerType type)
    {
        this->name = name;
        this->type = type;
        this->isDefined = false;
    }

    Message Context::insert(std::string name, Compiler::AST::InnerType type, int32_t intValue)
    {
        auto &symbol = this->symbolTable.back();
        if (symbol.find(name) != symbol.end()) {
            return Message{false, "redefined variable"};
        }
        symbol[name] = std::make_unique<Symbol>(name, type, intValue);
        return Message{true, ""};
    }

    Message Context::insert(std::string name, Compiler::AST::InnerType type, float floatValue)
    {
        auto &symbol = this->symbolTable.back();
        if (symbol.find(name) != symbol.end()) {
            return Message{false, "redefined variable"};
        }
        symbol[name] = std::make_unique<Symbol>(name, type, floatValue);
        return Message{true, ""};
    }

    Message Context::insert(std::string name, Compiler::AST::InnerType type)
    {
        auto &symbol = this->symbolTable.back();
        if (symbol.find(name) != symbol.end()) {
            return Message{false, "redefined variable"};
        }
        symbol[name] = std::make_unique<Symbol>(name, type);
        return Message{true, ""};
    }

    Message Context::confirm(std::string name) {
        for (auto it = this->symbolTable.rbegin(); it != this->symbolTable.rend(); it++) {
            if (it->find(name) != it->end()) {
                return Message{true, ""};
            }
        }
        return Message{false, "undefined variable"};
    }

    Message Context::find(std::string name) {
        for (auto it = this->symbolTable.rbegin(); it != this->symbolTable.rend(); it++) {
            if (it->find(name) != it->end()) {
                return Message{true, ""};
            }
        }
        return Message{false, "undefined variable"};
    }

    void Context::enterScope() {
        this->symbolTable.push_back(std::map<std::string, std::unique_ptr<Symbol>>());
    }

    void Context::exitScope() {
        this->symbolTable.pop_back();
    }
}