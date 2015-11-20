#ifndef __YAL_ASTNODE_H__
#define __YAL_ASTNODE_H__

#include "yal/yal.h"
#include "yal/enums.h"
#include "yal/util/stl_utils.h"
#include "yal/ast/astnodevisitor.h"
#include "yal/util/errorhandler.h"
#include "yal/symbols/symbol.h"

namespace yal
{
class Scope;

class Type;
class ArrayType;
class AstBaseNode
{
public:
    AstBaseNode(const SourceLocationInfo& loc);

    AstBaseNode(const AstBaseNode&) = delete;

    AstBaseNode& operator=(const AstBaseNode&) = delete;

    virtual ~AstBaseNode(){}

    virtual void accept(AstNodeVisitor& visitor) = 0;

    virtual AstType astType() const = 0;

    virtual const char* astTypeStr() const = 0;

    virtual bool isExpressionNode() const
    {
        return false;
    }

    const SourceLocationInfo& locationInfo() const;

    Scope* scope() const
    {
        return _pScope;
    }

    void setScope(Scope* pScope)
    {
        _pScope = pScope;
    }

    void setNodeType(Type* type)
    {
        _nodeType = type;
    }

    Type* nodeType() const
    {
        return _nodeType;
    }

    void setParentNode(AstBaseNode* parent)
    {
        YAL_ASSERT(parent != this);
        _parentNode = parent;
    }

    AstBaseNode* parentNode() const
    {
        return _parentNode;
    }

    bool isSourceAstNode() const;

protected:
    AstBaseNode* _parentNode = nullptr;
    const SourceLocationInfo _loc;
    Scope* _pScope = nullptr;
    Type* _nodeType = nullptr;
};


typedef YALVector<yal::AstBaseNode*> AstBaseNodeVec_t;

#define YAL_AST_NODE_ACCEPT_HDR(type) \
    static const yal::AstType NodeType;\
    inline virtual void accept(yal::AstNodeVisitor& visitor) override \
{ \
    visitor.visit(*this); \
} \
    \
    inline virtual yal::AstType astType() const override \
{ \
    return YAL_AST_NODE_TYPE(type); \
} \
    inline virtual const char* astTypeStr() const override\
{\
    return #type; \
}

#define YAL_AST_NODE_ACCEPT_HDR_BASE(type, base) \
    YAL_AST_NODE_ACCEPT_HDR(type) \
    inline virtual void acceptBase(yal::AstNodeVisitor& visitor) \
{ \
    base::accept(visitor); \
}

#define YAL_AST_NODE_ACCEPT_IMP(type) \
    const yal::AstType type::NodeType = YAL_AST_NODE_TYPE(type);

template<class Sym>
inline bool ast_typeof(const AstBaseNode* pNode)
{
    return pNode->astType() == Sym::NodeType;
}

template<class Sym>
inline Sym* ast_cast(AstBaseNode* pNode)
{
    return (ast_typeof<Sym>(pNode)) ? static_cast<Sym*>(pNode) : nullptr;
}



class StatementNode : public AstBaseNode
{
public:
    typedef std::vector<Symbol*> SymScope_t;

    StatementNode(const SourceLocationInfo& loc):
        AstBaseNode(loc)
    {
    }

    virtual ~StatementNode() {}


    const SymScope_t& symbolScope() const
    {
        return _stSymbolScope;
    }

    void addSymbolToScope(Symbol* sym)
    {
        _stSymbolScope.push_back(sym);
    }

    bool removeSymbolFromScope(Symbol* sym);

protected:
    SymScope_t _stSymbolScope;
};

typedef YALVector<yal::StatementNode*> StatementNodeVec_t;


class ExpressionResult
{
public:

    ExpressionResult() : type(nullptr), symbol(nullptr) {}

    //ExpressionResult(Type* t): type(t), symbol(nullptr){ YAL_ASSERT(t);}

    ExpressionResult(Type *t, Symbol* s) : type(t), symbol(s) {YAL_ASSERT(t); YAL_ASSERT(s);}

    Type* type;
    Symbol* symbol;
};

class ExpressionNode : public StatementNode
{
public:

    ExpressionNode(const SourceLocationInfo& loc):
        StatementNode(loc)
    {
    }

    void setExpressionResult(const ExpressionResult& result)
    {
        _expResult = result;
    }

    const ExpressionResult& expressionResult() const
    {
        return _expResult;
    }

    virtual bool isExpressionNode() const override
    {
        return true;
    }

    virtual ~ExpressionNode() {}

private:
    ExpressionResult _expResult;
};

class UnaryExpressionNode : public ExpressionNode
{
public:
    UnaryExpressionNode(const SourceLocationInfo& loc,
                        ExpressionNode* node):
        ExpressionNode(loc),
        _expression(node)
    {
        node->setParentNode(this);
    }

    virtual ~UnaryExpressionNode() {}

    ExpressionNode* expression() const
    {
        return _expression;
    }

    void replaceExpression(ExpressionNode* node)
    {
        node->setParentNode(this);
        _expression = node;
    }

protected:
    ExpressionNode* _expression;
};

class BinaryExpressionNode : public ExpressionNode
{
public:
    BinaryExpressionNode(const SourceLocationInfo& loc,
                         ExpressionNode* left,
                         ExpressionNode* right):
        ExpressionNode(loc),
        _expressionLeft(left),
        _expressionRight(right)
    {
        _expressionLeft->setParentNode(this);
        _expressionRight->setParentNode(this);
    }

    virtual ~BinaryExpressionNode() {}

    ExpressionNode* expressionRight() const
    {
        return _expressionRight;
    }

    ExpressionNode* expressionLeft() const
    {
        return _expressionLeft;
    }

    void replaceExpressionRight(ExpressionNode* node)
    {
        node->setParentNode(this);
        _expressionRight = node;
    }

    void replaceExpressionLeft(ExpressionNode* node)
    {
        node->setParentNode(this);
        _expressionLeft = node;
    }

protected:
    ExpressionNode* _expressionLeft;
    ExpressionNode* _expressionRight;

};

typedef YALVector<yal::ExpressionNode*> ExpressionNodeVec_t;


class MultiExpressionNode : public ExpressionNode
{
public:
    MultiExpressionNode(const SourceLocationInfo& loc):
        ExpressionNode(loc)
    {

    }

    virtual ~MultiExpressionNode() {}

    void addExpression(ExpressionNode* node)
    {
        node->setParentNode(this);
        _expressions.push_back(node);
    }

    size_t expressionCount() const
    {
        return _expressions.size();
    }

    bool replaceExpression(const ExpressionNode* old,
                           ExpressionNode* newExp);

    ExpressionNodeVec_t& expressions() {return _expressions;}

protected:
    ExpressionNodeVec_t _expressions;
};




class ExpressionList: public AstBaseNode
{
public:
    ExpressionList(const SourceLocationInfo& loc);

    ExpressionList(const SourceLocationInfo& loc,
                   ExpressionNodeVec_t&& list);

    virtual ~ExpressionList();

public:
    ExpressionNodeVec_t expressions;
};

}

#endif
