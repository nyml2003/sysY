#include "AST.hpp"
#include <iostream>
#include <fstream>
#include "driver.hpp"
namespace Compiler::AST
{
    int32_t eval(Operator op, int32_t left, int32_t right){
        switch (op)
        {
        case Operator::ADD:
            return left + right;
        case Operator::SUB:
            return left - right;
        case Operator::MUL:
            return left * right;
        case Operator::DIV:
            return left / right;
        case Operator::MOD:
            return left % right;
        case Operator::AND:
            return left && right;
        case Operator::OR:
            return left || right;
        case Operator::EQ:
            return left == right;
        case Operator::NE:
            return left != right;
        case Operator::LT:
            return left < right;
        case Operator::GT:
            return left > right;
        case Operator::LE:
            return left <= right;
        case Operator::GE:
            return left >= right;
        default:
            std::cerr << "eval() is not implemented" << std::endl;
            return 0;
        }
    }

    int32_t eval(Operator op, int32_t value)
    {
        switch (op)
        {
        case Operator::PLUS:
            return value;
        case Operator::MINUS:
            return -value;
        case Operator::NOT:
            return !value;
        default:
            std::cerr << "eval() is not implemented" << std::endl;
            return 0;
        }
    }

    Node::Node()
    {
        nodeId = maxASTNodeId++;
        begin = driver.location.begin;
        end = driver.location.end;
    }
    size_t Node::maxASTNodeId = 0;

    void Node::printLocation(const std::string &message)
    {
        std::ifstream targetFile(driver.sourceFileName);
        std::vector<std::string> lines;
        std::string line;
        for (int i = 0; i < this->begin.line - 1; i++)
        {
            std::getline(targetFile, line);
        }
        for (int i = this->begin.line - 1; i < this->end.line; i++)
        {
            std::getline(targetFile, line);
            lines.push_back(line);
        }
        for (size_t i = 0; i < lines.size(); i++)
        {
            std::cerr << "|" << lines[i] << std::endl;
            std::cerr << "|";
            if (i == 0)
            {
                for (int j = 0; j < this->begin.column - 1; j++)
                {
                    std::cerr << " ";
                }
                if (this->begin.line == this->end.line)
                {
                    for (int j = this->begin.column - 1; j < this->end.column - 1; j++)
                    {
                        std::cerr << "^";
                    }
                }
                else
                {
                    for (size_t j = this->begin.column - 1; j < lines[i].size(); j++)
                    {
                        std::cerr << "^";
                    }
                }
            }
            else if (i == lines.size() - 1)
            {
                for (int j = 0; j < this->end.column - 1; j++)
                {
                    std::cerr << "^";
                }
            }
            else
            {
                for (size_t j = 0; j < lines[i].size(); j++)
                {
                    std::cerr << " ";
                }
            }
            std::cerr << std::endl;
        }
        std::cerr << "| Error: " << message << std::endl;
    }


    void CompUnit::toMermaid()
    {
        for (size_t i = 0; i < children.size(); i++)
        {
            std::cout << nodeId << "[CompUnit]";
            std::cout << "--" << i << "--->" << children[i]->nodeId << std::endl;
            children[i]->toMermaid();
        }
    }

    void CompUnit::attach(std::vector<NodePtr> _children)
    {
        this->children.clear();
        for (auto &child : _children)
        {
            this->children.push_back(std::move(child));
        }
        begin = children[0]->begin;
        end = children[children.size() - 1]->end;
    }

    void CompUnit::analyze()
    {
        if (auto last = dynamic_cast<FuncDef *>(children[children.size() - 1].get()); last == nullptr)
        {
            printLocation("The last declaration must be a function definition");
            exit(1);
        }else{
            if (auto name = dynamic_cast<Ident *>(last->ident.get())->name; name != "main")
            {
                printLocation("The last function must be main");
                exit(1);
            }
        }
        for (auto &child : children)
        {
            child->analyze();
        }
    }

    void Decl::toMermaid()
    {
        std::cout << nodeId << "[Decl:";
        for (auto &decorator : decorators)
        {
            std::cout << decoratorName[static_cast<int>(decorator)] << " ";
        }
        std::cout << "]" << std::endl;
        std::cout << nodeId << "--type-->" << type->nodeId << std::endl;
        type->toMermaid();
        for (size_t i = 0; i < defList.size(); i++)
        {
            std::cout << nodeId << "--defList-->" << defList[i]->nodeId << std::endl;
            defList[i]->toMermaid();
        }
    }

    void Decl::analyze()
    {
        // TODO
    }

    void Decl::addDecorator(Decorator decorator)
    {
        this->begin.column = 1;
        decorators.push_back(decorator);
    }

    Decl::Decl(NodePtr type, std::vector<NodePtr> defList) : defList(std::move(defList))
    {
        this->type = std::make_unique<Type>(dynamic_cast<Type *>(type.get())->type);
        this->begin = type->begin;
        this->end = this->defList[this->defList.size() - 1]->end;
    }

    void Type::toMermaid()
    {
        std::cout << nodeId << "[Type:" << innerType[static_cast<int>(type)] << "]" << std::endl;
    }

    Type::Type(InnerType type) : type(type)
    {
    }

    void Type::analyze()
    {
        // TODO
    }

    Def::Def(NodePtr lval, NodePtr initVal) : lval(std::move(lval)), initVal(std::move(initVal))
    {
        begin = this->lval->begin;
        end = this->initVal->end;
    }

    void Def::analyze()
    {
        // TODO
    }

    void Def::toMermaid()
    {
        std::cout << nodeId << "[Def]" << std::endl;
        std::cout << nodeId << "--lval-->" << lval->nodeId << std::endl;
        lval->toMermaid();
        std::cout << nodeId << "--initVal-->" << initVal->nodeId << std::endl;
        initVal->toMermaid();
    }

    void Lval::toMermaid()
    {
        std::cout << nodeId << "[Lval]" << std::endl;
        std::cout << nodeId << "--ident-->" << ident->nodeId << std::endl;
        ident->toMermaid();
        for (size_t i = 0; i < dim.size(); i++)
        {
            std::cout << nodeId << "--dim" << i << "-->" << dim[i]->nodeId << std::endl;
            dim[i]->toMermaid();
        }
    }

    void Lval::analyze()
    {
        // TODO
    }

    Lval::Lval(NodePtr ident, std::vector<NodePtr> dim) : ident(std::move(ident)), dim(std::move(dim))
    {
        begin = this->ident->begin;
        end = this->dim[this->dim.size() - 1]->end;
    }

    Lval::Lval(NodePtr ident) : ident(std::move(ident))
    {
        begin = this->ident->begin;
        end = this->ident->end;
    }

    void InitVal::toMermaid()
    {
        std::cout << nodeId << "[InitVal]" << std::endl;
        for (size_t i = 0; i < children.size(); i++)
        {
            std::cout << nodeId << "--" << i << "-->" << children[i]->nodeId << std::endl;
            children[i]->toMermaid();
        }
    }

    void InitVal::analyze()
    {
        // TODO
    }

    InitVal::InitVal(std::vector<NodePtr> children) : children(std::move(children))
    {
        begin = this->children[0]->begin;
        end = this->children[this->children.size() - 1]->end;
    }

    InitVal::InitVal(NodePtr child)
    {
        children.push_back(std::move(child));
        begin = this->children[0]->begin;
        end = this->children[0]->end;
    }

    FuncDef::FuncDef(NodePtr retType, NodePtr ident, NodePtr block) : retType(std::move(retType)), ident(std::move(ident)), block(std::move(block))
    {
        begin = this->retType->begin;
        end = this->block->end;
        end.line += 1;
        end.column = 1;
    }

    void FuncDef::analyze()
    {
        // TODO
    }

    void FuncDef::toMermaid()
    {
        std::cout << nodeId << "[FuncDef]" << std::endl;
        std::cout << nodeId << "--retType-->" << retType->nodeId << std::endl;
        retType->toMermaid();
        std::cout << nodeId << "--ident-->" << ident->nodeId << std::endl;
        ident->toMermaid();
        std::cout << nodeId << "--block-->" << block->nodeId << std::endl;
        block->toMermaid();
    }

    Block::Block()
    {
    }

    Block::Block(std::vector<NodePtr> children) : children(std::move(children))
    {
        begin = this->children[0]->begin;
        end = this->children[this->children.size() - 1]->end;
    }

    void Block::analyze()
    {
        // TODO
    }

    void Block::toMermaid()
    {
        std::cout << nodeId << "[Block]" << std::endl;
        for (size_t i = 0; i < children.size(); i++)
        {
            std::cout << nodeId << "--" << i << "-->" << children[i]->nodeId << std::endl;
            children[i]->toMermaid();
        }
    }

    AssignStmt::AssignStmt(NodePtr lval, NodePtr expr) : lval(std::move(lval)), expr(std::move(expr))
    {
        begin = this->lval->begin;
        end = this->expr->end;
    }

    void AssignStmt::analyze()
    {
        // TODO
    }

    void AssignStmt::toMermaid()
    {
        std::cout << nodeId << "[AssignStmt]" << std::endl;
        std::cout << nodeId << "--lval-->" << lval->nodeId << std::endl;
        lval->toMermaid();
        std::cout << nodeId << "--expr-->" << expr->nodeId << std::endl;
        expr->toMermaid();
    }

    ExpStmt::ExpStmt(NodePtr expr) : expr(std::move(expr))
    {
        begin = this->expr->begin;
        end = this->expr->end;
    }

    void ExpStmt::analyze()
    {
        // TODO
    }

    void ExpStmt::toMermaid()
    {
        std::cout << nodeId << "[ExpStmt]" << std::endl;
        std::cout << nodeId << "--expr-->" << expr->nodeId << std::endl;
        expr->toMermaid();
    }



    IfStmt::IfStmt(NodePtr cond, NodePtr thenStmt, NodePtr elseStmt) : cond(std::move(cond)), thenStmt(std::move(thenStmt)), elseStmt(std::move(elseStmt))
    {
        begin = this->cond->begin;
        end = this->elseStmt->end;
    }


    IfStmt::IfStmt(NodePtr cond, NodePtr thenStmt) : cond(std::move(cond)), thenStmt(std::move(thenStmt)), elseStmt(nullptr)
    {
        begin = this->cond->begin;
        end = this->thenStmt->end;
    }

    void IfStmt::analyze()
    {
        // TODO
    }

    void IfStmt::toMermaid()
    {
        std::cout << nodeId << "[IfStmt]" << std::endl;
        std::cout << nodeId << "--cond-->" << cond->nodeId << std::endl;
        cond->toMermaid();
        std::cout << nodeId << "--thenBlock-->" << thenStmt->nodeId << std::endl;
        thenStmt->toMermaid();
        if (elseStmt)
        {
            std::cout << nodeId << "--elseBlock-->" << elseStmt->nodeId << std::endl;
            elseStmt->toMermaid();
        }
    }

    WhileStmt::WhileStmt(NodePtr expr, NodePtr stmt) : expr(std::move(expr)), stmt(std::move(stmt))
    {
        begin = this->expr->begin;
        end = this->stmt->end;
    }

    void WhileStmt::analyze()
    {
        // TODO
    }

    void WhileStmt::toMermaid()
    {
        std::cout << nodeId << "[WhileStmt]" << std::endl;
        std::cout << nodeId << "--expr-->" << expr->nodeId << std::endl;
        expr->toMermaid();
        std::cout << nodeId << "--stmt-->" << stmt->nodeId << std::endl;
        stmt->toMermaid();
    }

    BreakStmt::BreakStmt()
    {
    }

    void BreakStmt::analyze()
    {
        // TODO
    }

    void BreakStmt::toMermaid()
    {
        std::cout << nodeId << "[BreakStmt]" << std::endl;
    }

    ContinueStmt::ContinueStmt()
    {
    }

    void ContinueStmt::analyze()
    {
        // TODO
    }

    void ContinueStmt::toMermaid()
    {
        std::cout << nodeId << "[ContinueStmt]" << std::endl;
    }


    ReturnStmt::ReturnStmt(NodePtr expr) : expr(std::move(expr))
    {
        begin = this->expr->begin;
        end = this->expr->end;
    }

    void ReturnStmt::analyze()
    {
        // TODO
    }

    void ReturnStmt::toMermaid()
    {
        std::cout << nodeId << "[ReturnStmt]" << std::endl;
        std::cout << nodeId << "--expr-->" << expr->nodeId << std::endl;
        expr->toMermaid();
    }

    BinaryExp::BinaryExp(NodePtr left, Operator op, NodePtr right) : left(std::move(left)), op(op), right(std::move(right))
    {
        begin = this->left->begin;
        end = this->right->end;
    }

    void BinaryExp::analyze()
    {
        // TODO
    }

    void BinaryExp::constantFolding()
    {
    }

    void BinaryExp::toMermaid()
    {
        std::cout << nodeId << "[" << operatorName[static_cast<int>(op)] << "]" << std::endl;
        std::cout << nodeId << "--left-->" << left->nodeId << std::endl;
        left->toMermaid();
        std::cout << nodeId << "--right-->" << right->nodeId << std::endl;
        right->toMermaid();
    }

    UnaryExp::UnaryExp(Operator op, NodePtr expr) : op(op), expr(std::move(expr))
    {
        begin = this->expr->begin;
        end = this->expr->end;
    }

    void UnaryExp::analyze()
    {
        // TODO
    }

    void UnaryExp::toMermaid()
    {
        std::cout << nodeId << "[" << operatorName[static_cast<int>(op)] << "]" << std::endl;
        std::cout << nodeId << "--expr-->" << expr->nodeId << std::endl;
        expr->toMermaid();
    }

    Ident::Ident(std::string &name) : name(name)
    {
    }

    void Ident::toMermaid()
    {
        std::cout << nodeId << "[" << name << "]" << std::endl;
    }

    void Ident::analyze()
    {
        // TODO
    }

    Int32::Int32(int32_t val) : val(val)
    {
    }

    void Int32::toMermaid()
    {
        std::cout << nodeId << "[" << val << "]" << std::endl;
    }


    void Int32::analyze()
    {
        // TODO
    }

    Float32::Float32(float val) : val(val)
    {
    }

    void Float32::analyze()
    {
        // TODO
    }



    void Float32::toMermaid()
    {
        std::cout << nodeId << "[" << val << "]" << std::endl;
    }
}