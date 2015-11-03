#include "yal/memory/arcpass.h"
#include "yal/ast/asthdrs.h"
#include "yal/symbols/objectscopeaction.h"

namespace yal
{

/*
 * TODO: Handle all reference counting issues
 *
 * 1) Object assignment
 * 2) Object copies
 * 3) Object return values, might be released by scope exit
 * 4) Function return object no collected, and therefor not released
 */

ArcPass::ArcPass()
{

}

void
ArcPass::process(ParserState_t &state)
{
    for (auto& v : state.program)
    {
        v->accept(*this);
    }
}

void
ArcPass::visit(AssignOperatorNode& node)
{
    node.expressionLeft()->accept(*this);
    node.expressionRight()->accept(*this);

    ExpressionNode* exp_right = node.expressionRight();
    if (node.expressionResult().type->isObjectType())
    {

        //node.replaceExpressionLeft(new ObjectReleaseNode(node.expressionLeft()));

        if (exp_right->expressionResult().type->isObjectType()
                && !ast_typeof<ObjectCreateNode>(exp_right))
        {
            node.replaceExpressionRight(new ObjectRetainNode(exp_right));
        }
    }
}

void
ArcPass::visit(CodeBodyNode& node)
{
    for(auto& v : node.statements)
    {
        v->accept(*this);
    }
}

void
ArcPass::visit(CompareOperatorNode& node)
{
    node.expressionLeft()->accept(*this);
    node.expressionRight()->accept(*this);
}

void
ArcPass::visit(ConstantNode&)
{
}

void
ArcPass::visit(ArgumentDeclNode&)
{
}

void
ArcPass::visit(ArgumentDeclsNode& node)
{
    for(auto& v : node.arguments())
    {
        v->accept(*this);
    }
}

void
ArcPass::visit(FunctionCallArgsNode& node)
{
    for(auto& v : node.expressions)
    {
        v->accept(*this);
    }
}

void
ArcPass::visit(VariableDeclNode& node)
{
    ExpressionNode* exp_node = node.expression();
    exp_node->accept(*this);

    // check for object creation nodes
    if (node.expressionResult().type->isObjectType())
    {
        if (exp_node->expressionResult().type->isObjectType()
                && !ast_typeof<ObjectCreateNode>(exp_node))
        {
            node.replaceExpression(new ObjectRetainNode(exp_node));
        }
    }
}

void
ArcPass::visit(DualOperatorNode& node)
{
    node.expressionLeft()->accept(*this);
    node.expressionRight()->accept(*this);
}

void
ArcPass::visit(SingleOperatorNode& node)
{
    node.expression()->accept(*this);
}

void
ArcPass::visit(FunctionCallNode& node)
{
    if (node.hasFunctionArguments())
    {
        node.functionArguments()->accept(*this);
    }
}

void
ArcPass::visit(FunctionDeclNode& node)
{
    if (node.hasFunctionArguments())
    {
        node.functionArguments()->accept(*this);
    }
    node.functionCode()->accept(*this);
}

void
ArcPass::visit(ConditionNode& node)
{
    if (node.condition())
    {
        node.condition()->accept(*this);
    }

    if (node.onTrue())
    {
        node.onTrue()->accept(*this);
    }

    if (node.onFalse())
    {
        node.onFalse()->accept(*this);
    }
}

void
ArcPass::visit(ReturnNode& node)
{
    if (node.expression())
    {
        node.expression()->accept(*this);
    }
}

void
ArcPass::visit(PrintNode& node)
{
    node.arguments()->accept(*this);
}

void
ArcPass::visit(PrintArgsNode& node)
{
    for(auto& v : node.expressions)
    {
        v->accept(*this);
    }
}

void
ArcPass::visit(WhileLoopNode& node)
{
    node.condition()->accept(*this);
    node.code()->accept(*this);
}

void
ArcPass::visit(VariableAccessNode&)
{
}

void
ArcPass::visit(StringCreateNode& node)
{
    node.constantNode()->accept(*this);
}

void
ArcPass::visit(ObjectCreateNode& node)
{
    node.expression()->accept(*this);
}

void
ArcPass::visit(ObjectRetainNode&)
{
    YAL_ASSERT(false && "Should not be reached");
}

void
ArcPass::visit(ObjectReleaseNode&)
{
    YAL_ASSERT(false && "Should not be reached");
}


void
ArcScopeActionVisitor::visitOnEnter(const ObjectScopeAction&)
{
    // do nothing
}

void
ArcScopeActionVisitor::visitOnExit(const ObjectScopeAction& action)
{
    const Symbol* sym = action.symbol();
    const Type* sym_type = sym->astNode()->nodeType();
    YAL_ASSERT(sym->isVariable() && sym_type->isObjectType());

    _codeBody.addStatement(new ObjectReleaseNode(new VariableAccessNode(SourceLocationInfo(), sym->symbolName())));
}


}
