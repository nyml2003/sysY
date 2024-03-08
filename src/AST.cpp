#include "AST.hpp"
#include <iostream>
#include <fstream>
#include "driver.hpp"
#include "Scope.hpp"
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

    bool isConst(Node *node);
    NodePtr constantFolding(Node *node);

    bool isConst(Node* node){
        auto isIdent = dynamic_cast<Ident *>(node);
        if (isIdent)
        {
            auto message = context.confirm(isIdent->name);
            if (!message.success)
            {
                node->printLocation(message.content);
                exit(1);
            }
            message = context.find(isIdent->name);
            return message.success;
        }
        auto isInt32 = dynamic_cast<Int32 *>(node);
        if (isInt32)
        {
            return true;
        }
        auto isFloat32 = dynamic_cast<Float32 *>(node);
        if (isFloat32)
        {
            return true;
        }
        auto isBinaryExp = dynamic_cast<BinaryExp *>(node);
        if (isBinaryExp)
        {
            bool isLeftConst = isConst(isBinaryExp->left.get());
            if (isLeftConst){
                auto left = constantFolding(isBinaryExp->left.get());
                if (left){
                    isBinaryExp->left = std::move(left);
                }
            }
            bool isRightConst = isConst(isBinaryExp->right.get());
            if (isRightConst){
                auto right = constantFolding(isBinaryExp->right.get());
                if (right){
                    isBinaryExp->right = std::move(right);
                }
            }
            return isLeftConst && isRightConst;
        }
        auto isUnaryExp = dynamic_cast<UnaryExp *>(node);
        if (isUnaryExp)
        {
            bool isChildConst = isConst(isUnaryExp->expr.get());
            if (isChildConst){
                auto child = constantFolding(isUnaryExp->expr.get());
                if (child){
                    isUnaryExp->expr = std::move(child);
                }
            }
            return isChildConst;
        }
        auto isLval = dynamic_cast<Lval *>(node);
        if (isLval)
        {
            return isConst(isLval->ident.get());
        }
        std::cerr<<"isConst() is not implemented"<<std::endl;
        exit(1);
    }

    NodePtr constantFolding(Node* node){
        auto isIdent = dynamic_cast<Ident *>(node);
        if (isIdent)
        {
            auto message = context.confirm(isIdent->name);
            if (!message.success)
            {
                node->printLocation(message.content);
                exit(1);
            }
            message = context.find(isIdent->name);
            if (!message.success)
            {
                return nullptr;
            }else {
                auto value = message.value;
                if (std::holds_alternative<int32_t>(value)){
                    return std::make_unique<Int32>(std::get<int32_t>(value));
                }else if (std::holds_alternative<float>(value)){
                    return std::make_unique<Float32>(std::get<float>(value));
                }else{
                    std::cerr<<"constantFolding() is not implemented"<<std::endl;
                    exit(1);
                }
            }
        }
        auto isInt32 = dynamic_cast<Int32 *>(node);
        if (isInt32)
        {
            return nullptr;
        }
        auto isFloat32 = dynamic_cast<Float32 *>(node);
        if (isFloat32)
        {
            return nullptr;
        }
        auto isBinaryExp = dynamic_cast<BinaryExp *>(node);
        if (isBinaryExp)
        {
            bool isExprConst = isConst(node);
            if (isExprConst){
                if (constantFolding(isBinaryExp->left.get())){
                    isBinaryExp->left = std::move(constantFolding(isBinaryExp->left.get()));
                }
                if (constantFolding(isBinaryExp->right.get())){
                    isBinaryExp->right = std::move(constantFolding(isBinaryExp->right.get()));
                }
                auto isLeftInt32 = dynamic_cast<Int32 *>(isBinaryExp->left.get());
                auto isRightInt32 = dynamic_cast<Int32 *>(isBinaryExp->right.get());
                if (isLeftInt32 && isRightInt32){
                    return std::make_unique<Int32>(eval(isBinaryExp->op, isLeftInt32->val, isRightInt32->val));
                }
                auto isLeftFloat32 = dynamic_cast<Float32 *>(isBinaryExp->left.get());
                auto isRightFloat32 = dynamic_cast<Float32 *>(isBinaryExp->right.get());
                if (isLeftFloat32 && isRightFloat32){
                    return std::make_unique<Float32>(eval(isBinaryExp->op, isLeftFloat32->val, isRightFloat32->val));
                }
                return std::make_unique<Float32>(eval(isBinaryExp->op, (float)isLeftInt32->val, (float)isRightFloat32->val));
            }else{
                return nullptr;
            }
        }
        auto isUnaryExp = dynamic_cast<UnaryExp *>(node);
        if (isUnaryExp)
        {
            bool isExprConst = isConst(node);
            if (isExprConst){
                auto child = constantFolding(isUnaryExp->expr.get());
                auto isChildInt32 = dynamic_cast<Int32 *>(child.get());
                if (isChildInt32){
                    return std::make_unique<Int32>(eval(isUnaryExp->op, isChildInt32->val));
                }
                auto isChildFloat32 = dynamic_cast<Float32 *>(child.get());
                if (isChildFloat32){
                    return std::make_unique<Float32>(eval(isUnaryExp->op, isChildFloat32->val));
                }
                return nullptr;
            }else{
                return nullptr;
            }
        }
        auto isLval = dynamic_cast<Lval *>(node);
        if (isLval)
        {
            return constantFolding(isLval->ident.get());
        }
        std::cerr<<"constantFolding() is not implemented"<<std::endl;
        exit(1);
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
        auto last = dynamic_cast<FuncDef *>(children[children.size() - 1].get());
        if (last == nullptr)
        {
            this->printLocation("The last declaration must be a function definition");
            exit(1);
        }
        else
        {
            auto name = dynamic_cast<Ident *>(last->ident.get())->name;
            if (name != "main")
            {
                this->printLocation("The last function must be main");
                exit(1);
            }
            auto type = dynamic_cast<Type *>(last->retType.get());
            if (type->type != InnerType::INT)
            {
                this->printLocation("The return type of main must be int32");
                exit(1);
            }
        }
        context.enterScope();
        for (auto &child : children)
        {
            child->analyze();
        }
        context.exitScope();
    }

    CompUnit::CompUnit()
    {
        nodeType = NODE_TYPE::COMP_UNIT;
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
        bool hasConst = false;
        auto type = dynamic_cast<Type *>(this->type.get());
        for (auto &decorator : decorators)
        {
            if (decorator == Decorator::CONSTANT)
            {
                hasConst = true;
                break;
            }
        }
        if (hasConst)
        {
            for (auto &def : defList)
            {
                if (!dynamic_cast<Def *>(def.get()))
                {
                    this->printLocation("Constant must be initialized");
                    exit(1);
                }
                auto lval = dynamic_cast<Lval *>(dynamic_cast<Def *>(def.get())->lval.get());
                auto name = dynamic_cast<Ident *>(lval->ident.get())->name;
                auto message = context.insert(name, type->type);
                if (!message.success)
                {
                    this->printLocation(message.content);
                    exit(1);
                }
                auto initVal = dynamic_cast<InitVal *>(dynamic_cast<Def *>(def.get())->initVal.get());
                if (initVal->children.size() == 1){
                    switch (type->type)
                    {
                    case InnerType::INT:{
                        if (isConst(initVal->children[0].get()) && constantFolding(initVal->children[0].get()))
                        {
                            initVal->children[0] = std::move(constantFolding(initVal->children[0].get()));
                        }
                        if (dynamic_cast<Int32 *>(initVal->children[0].get()) == nullptr)
                        {
                            this->printLocation("The type of the initializer is not int32");
                            exit(1);
                        }
                        auto message = context.setValue(name, dynamic_cast<Int32 *>(initVal->children[0].get())->val);
                        if (!message.success)
                        {
                            this->printLocation(message.content);
                            exit(1);
                        }
                        break;
                    }
                        
                    case InnerType::FLOAT:{
                        if (isConst(initVal->children[0].get()) && constantFolding(initVal->children[0].get()))
                        {
                            initVal->children[0] = std::move(constantFolding(initVal->children[0].get()));
                        }
                        if (dynamic_cast<Float32 *>(initVal->children[0].get()) == nullptr)
                        {
                            this->printLocation("The type of the initializer is not float32");
                            exit(1);
                        }
                        auto message = context.setValue(name, dynamic_cast<Float32 *>(initVal->children[0].get())->val);
                        if (!message.success)
                        {
                            this->printLocation(message.content);
                            exit(1);
                        }
                        break;
                    }
                        
                    case InnerType::ARRAY:
                    case InnerType::VOID:
                    default:
                        this->printLocation("The type of the initializer is not implemented");
                        exit(1);
                    }
                }
            }
        }
        else {
            for (auto &def : defList)
            {
                if (dynamic_cast<Def *>(def.get()))
                {
                    auto lval = dynamic_cast<Lval *>(dynamic_cast<Def *>(def.get())->lval.get());
                    auto name = dynamic_cast<Ident *>(lval->ident.get())->name;
                    auto message = context.insert(name, type->type);
                    if (!message.success)
                    {
                        this->printLocation(message.content);
                        exit(1);
                    }
                    auto initVal = dynamic_cast<InitVal *>(dynamic_cast<Def *>(def.get())->initVal.get());
                    initVal->analyze();
                }
                else if (dynamic_cast<Lval *>(def.get()))
                {
                    auto lval = dynamic_cast<Lval *>(def.get());
                    auto name = dynamic_cast<Ident *>(lval->ident.get())->name;
                    auto message = context.insert(name, type->type);
                    if (!message.success)
                    {
                        this->printLocation(message.content);
                        exit(1);
                    }
                }
            }
        }
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
        this->nodeType = NODE_TYPE::DECL;
    }

    void Type::toMermaid()
    {
        std::cout << nodeId << "[Type:" << innerType[static_cast<int>(type)] << "]" << std::endl;
    }

    Type::Type(InnerType type) : type(type)
    {
        this->nodeType = NODE_TYPE::TYPE;
    }

    void Type::analyze()
    {
        // TODO
    }

    Def::Def(NodePtr lval, NodePtr initVal) : lval(std::move(lval)), initVal(std::move(initVal))
    {
        begin = this->lval->begin;
        end = this->initVal->end;
        this->nodeType = NODE_TYPE::DEF;
    }

    void Def::analyze()
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

    void Lval::analyze()
    {
        
    }

    Lval::Lval(NodePtr ident, std::vector<NodePtr> dim) : ident(std::move(ident)), dim(std::move(dim))
    {
        begin = this->ident->begin;
        end = this->dim[this->dim.size() - 1]->end;
        this->nodeType = NODE_TYPE::LVAL;
    }

    Lval::Lval(NodePtr ident) : ident(std::move(ident))
    {
        begin = this->ident->begin;
        end = this->ident->end;
        this->nodeType = NODE_TYPE::LVAL;
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
        for (auto &child : children)
        {
            if (dynamic_cast<InitVal *>(child.get()))
            {
                child->analyze();
            }
            else
            {
                if (isConst(child.get()) && constantFolding(child.get()))
                {
                    child = std::move(constantFolding(child.get()));
                }
            }
        }
    }

    InitVal::InitVal(std::vector<NodePtr> children) : children(std::move(children))
    {
        begin = this->children[0]->begin;
        end = this->children[this->children.size() - 1]->end;
        this->nodeType = NODE_TYPE::INIT_VAL;
    }

    InitVal::InitVal(NodePtr child)
    {
        children.push_back(std::move(child));
        begin = this->children[0]->begin;
        end = this->children[0]->end;
        this->nodeType = NODE_TYPE::INIT_VAL;
    }

    FuncDef::FuncDef(NodePtr retType, NodePtr ident, NodePtr block) : retType(std::move(retType)), ident(std::move(ident)), block(std::move(block))
    {
        begin = this->retType->begin;
        end = this->block->end;
        end.line += 1;
        end.column = 1;
        this->nodeType = NODE_TYPE::FUNC_DEF;
    }

    void FuncDef::analyze()
    {
        block->analyze();
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

    Block::Block(){
        this->nodeType = NODE_TYPE::BLOCK;}

    Block::Block(std::vector<NodePtr> children) : children(std::move(children))
    {
        begin = this->children[0]->begin;
        end = this->children[this->children.size() - 1]->end;
        this->nodeType = NODE_TYPE::BLOCK;
    }

    void Block::analyze()
    {
        context.enterScope();
        for (auto &child : children)
        {
            child->analyze();
        }
        context.exitScope();
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
        this->nodeType = NODE_TYPE::ASSIGN_STMT;
    }

    void AssignStmt::analyze()
    {
        if (constantFolding(this->expr.get()))
        {
            this->expr = std::move(constantFolding(this->expr.get()));
        }
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
        this->nodeType = NODE_TYPE::EXP_STMT;
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
        this->nodeType = NODE_TYPE::IF_STMT;
    }


    IfStmt::IfStmt(NodePtr cond, NodePtr thenStmt) : cond(std::move(cond)), thenStmt(std::move(thenStmt)), elseStmt(nullptr)
    {
        begin = this->cond->begin;
        end = this->thenStmt->end;
        this->nodeType = NODE_TYPE::IF_STMT;
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
        this->nodeType = NODE_TYPE::WHILE_STMT;
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
        this->nodeType = NODE_TYPE::BREAK_STMT;
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
        this->nodeType = NODE_TYPE::CONTINUE_STMT;
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
        this->nodeType = NODE_TYPE::RETURN_STMT;
    }

    void ReturnStmt::analyze()
    {
        if (constantFolding(this->expr.get()))
        {
            this->expr = std::move(constantFolding(this->expr.get()));
        }
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
        this->nodeType = NODE_TYPE::BINARY_EXP;
    }

    void BinaryExp::analyze()
    {
        // TODO
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
        this->nodeType = NODE_TYPE::UNARY_EXP;
    }

    void UnaryExp::analyze()
    {
        //TODO
    }
    void UnaryExp::toMermaid()
    {
        std::cout << nodeId << "[" << operatorName[static_cast<int>(op)] << "]" << std::endl;
        std::cout << nodeId << "--expr-->" << expr->nodeId << std::endl;
        expr->toMermaid();
    }

    Ident::Ident(std::string &name) : name(name)
    {
        this->nodeType = NODE_TYPE::IDENT;
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
        this->nodeType = NODE_TYPE::INT32;
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
        this->nodeType = NODE_TYPE::FLOAT32;
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