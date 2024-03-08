#include "Scope.hpp"

namespace Compiler::Scope {

    Message Context::insert(std::string name, Compiler::AST::InnerType _Type)
    {
        if (this->symbolTable.back().find(name) != this->symbolTable.back().end())
        {
            return Message{false, "redefined variable", nullptr};
        }
        this->symbolTable.back()[name] = _Type;
        return Message{true, "", nullptr};
    }

    Message Context::confirm(std::string name) {
        for (auto it = this->symbolTable.rbegin(); it != this->symbolTable.rend(); it++) {
            if (it->find(name) != it->end()) {
                return Message{true, "", nullptr};
            }
        }
        return Message{false, "undecleared variable", nullptr};
    }

    Message Context::find(std::string name) {
        for (auto it = this->constTable.rbegin(); it != this->constTable.rend(); it++) {
            if (it->find(name) != it->end()) {
                return Message{true, "", it->at(name)};
            }
        }
        return Message{false, "undefined variable", nullptr};
    }

    void Context::enterScope() {
        this->symbolTable.push_back(std::map<std::string, std::unique_ptr<Symbol>>());
        this->constTable.push_back(std::map<std::string, std::shared_ptr<Compiler::Value>>());
    }

    void Context::exitScope() {
        this->symbolTable.pop_back();
        this->constTable.pop_back();
    }

    Message Context::setValue(std::string name, std::shared_ptr<Compiler::Value> value) {
        for (auto it = this->symbolTable.rbegin(); it != this->symbolTable.rend(); it++) {
            if (it->find(name) != it->end()) {
                this->constTable.back()[name] = value;
                return Message{true, "", nullptr};
            }
        }
        return Message{false, "undefined variable", nullptr};
    }

}