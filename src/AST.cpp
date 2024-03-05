#include "AST.hpp"
#include <iostream>
namespace Compiler::AST
{
    
    Node::Node()
    {
        nodeId = maxASTNodeId++;
    }
    size_t Node::maxASTNodeId = 0;

    void CompUnit::toMermaid()
    {
        for (size_t i = 0; i < children.size(); i++)
        {
            std::cout << nodeId << "[CompUnit]";
            std::cout <<  "--" << i << "--->" << children[i]->nodeId << std::endl;
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

    void Decl::addDecorator(Decorator decorator)
    {
        decorators.push_back(decorator);
    }

    Decl::Decl(NodePtr type, std::vector<NodePtr> defList) : defList(std::move(defList))
    {
        this->type = std::make_unique<Type>(dynamic_cast<Type*>(type.get())->type);
    }



    void Type::toMermaid()
    {
        std::cout << nodeId << "[Type:" << innerType[static_cast<int>(type)] << "]" << std::endl;
    }

    Type::Type(InnerType type) : type(type)
    {
    }

    Def::Def(NodePtr lval, NodePtr initVal) : lval(std::move(lval)), initVal(std::move(initVal))
    {
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

    Lval::Lval(NodePtr ident, std::vector<NodePtr> dim) : ident(std::move(ident)), dim(std::move(dim))
    {
    }

    Lval::Lval(NodePtr ident) : ident(std::move(ident))
    {
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

    InitVal::InitVal(std::vector<NodePtr> children) : children(std::move(children))
    {
    }

    InitVal::InitVal(NodePtr child)
    {
        children.push_back(std::move(child));
    }


    FuncDef::FuncDef(NodePtr retType, NodePtr ident, NodePtr block) : retType(std::move(retType)), ident(std::move(ident)), block(std::move(block))
    {
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
    }

    void ExpStmt::toMermaid()
    {
        std::cout << nodeId << "[ExpStmt]" << std::endl;
        std::cout << nodeId << "--expr-->" << expr->nodeId << std::endl;
        expr->toMermaid();
    }

    IfStmt::IfStmt(NodePtr cond, NodePtr thenStmt, NodePtr elseStmt) : cond(std::move(cond)), thenStmt(std::move(thenStmt)), elseStmt(std::move(elseStmt))
    {
    }

    IfStmt::IfStmt(NodePtr cond, NodePtr thenStmt) : cond(std::move(cond)), thenStmt(std::move(thenStmt)), elseStmt(nullptr)
    {
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

    void BreakStmt::toMermaid()
    {
        std::cout << nodeId << "[BreakStmt]" << std::endl;
    }

    ContinueStmt::ContinueStmt()
    {
    }

    void ContinueStmt::toMermaid()
    {
        std::cout << nodeId << "[ContinueStmt]" << std::endl;
    }

    ReturnStmt::ReturnStmt(NodePtr expr) : expr(std::move(expr))
    {
    }

    void ReturnStmt::toMermaid()
    {
        std::cout << nodeId << "[ReturnStmt]" << std::endl;
        std::cout << nodeId << "--expr-->" << expr->nodeId << std::endl;
        expr->toMermaid();
    }

    BinaryExp::BinaryExp(NodePtr left, Operator op, NodePtr right) : left(std::move(left)), op(op), right(std::move(right))
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

    Int32::Int32(int32_t val) : val(val)
    {
    }

    void Int32::toMermaid()
    {
        std::cout << nodeId << "[" << val << "]" << std::endl;
    }

    float32::float32(float val) : val(val)
    {
    }

    void float32::toMermaid()
    {
        std::cout << nodeId << "[" << val << "]" << std::endl;
    }

}