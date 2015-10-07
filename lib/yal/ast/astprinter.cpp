#include "yal/ast/astprinter.h"
#include "yal/ast/astnodetypes.h"
#include "yal/ast/astbasenode.h"
#include "yal/ast/assignoperatornode.h"
#include "yal/ast/codebodynode.h"
#include "yal/ast/compareoperatornode.h"
#include "yal/ast/constantnode.h"
#include "yal/ast/argumentdeclnode.h"
#include "yal/ast/variabledeclnode.h"
#include "yal/ast/dualoperatornode.h"
#include "yal/ast/singleoperatornode.h"
#include "yal/ast/functionnode.h"
#include "yal/ast/conditionnode.h"
#include "yal/ast/returnnode.h"
#include "yal/parser/parser_state.h"
#include "yal/ast/printnode.h"
#include "yal/ast/whileloopnode.h"
#include <cstdlib>

namespace yal
{

AstPrinter::AstPrinter(OutputSink &sink):
    _formater(),
    _sink(sink)
{

}

AstPrinter::~AstPrinter()
{

}

void
AstPrinter::process(ParserState_t &state)
{
    _formater.format("Module\n");
    _formater.write(_sink);
    _idents.clear();

    size_t idx = 1;
    for (auto& v : state.program)
    {
        onDescent(idx == state.program.size());
        v->accept(*this);
        onAscend();
        ++idx;
    }
}

void
AstPrinter::printIdent()
{
    for (auto& ch : _idents)
    {
        _formater.format("%c", ch);
        _formater.write(_sink);
    }
}

void
AstPrinter::printNodeTitle(const AstBaseNode& node,
                           const bool empty)
{
    printIdent();
    const SourceLocationInfo& src_info = node.locationInfo();
    if (!empty)
    {
        _formater.format("-%s <ln %d:%d cl %d:%d> ",
                         node.astTypeStr(),
                         src_info.firstLine,
                         src_info.lastLine,
                         src_info.firstColumn,
                         src_info.lastColumn);
    }
    else
    {
        _formater.format("-%s <ln %d:%d  cl %d:%d>\n",
                         node.astTypeStr(),
                         src_info.firstLine,
                         src_info.lastLine,
                         src_info.firstColumn,
                         src_info.lastColumn);
    }
    _formater.write(_sink);
}

void
AstPrinter::onDescent(const bool lastNode)
{
    if (_idents.size() > 1)
    {
        auto it = _idents.rbegin();
        if (*it == '`')
        {
            *it = ' ';
        }
    }
    _idents.push_back(' ');
    _idents.push_back(lastNode ? '`' : '|');
}

void
AstPrinter::onAscend()
{
    const size_t size = _idents.size() - 2;
    _idents.erase(_idents.begin() + size, _idents.end());
}

void
AstPrinter::visit(AssignOperatorNode& node)
{
    printNodeTitle(node);
    _formater.format(" %s var:%s\n",
                     OperatorTypeToStr(node.assignOperatorType()),
                     node.variableName());
    _formater.write(_sink);

    onDescent(true);
    node.expression()->accept(*this);
    onAscend();
}

void
AstPrinter::visit(CodeBodyNode& node)
{
    printNodeTitle(node, true);

    size_t idx = 1;
    for(auto& v : node.statements)
    {
        onDescent(idx == node.statements.size());
        v->accept(*this);
        onAscend();
        ++idx;
    }
}

void
AstPrinter::visit(CompareOperatorNode& node)
{
    printNodeTitle(node);
    _formater.format("%s \n",
                     OperatorTypeToStr(node.compareOperatorType()));
    _formater.write(_sink);
    onDescent(false);
    node.leftExpression()->accept(*this);
    onAscend();
    onDescent(true);
    node.rightExpression()->accept(*this);
    onAscend();
}

void
AstPrinter::visit(ConstantNode& node)
{
    printNodeTitle(node);
    _formater.format("type:%s \n",
                     ConstantTypeToStr(node.constantValue().type()));
    _formater.write(_sink);
}

void
AstPrinter::visit(ArgumentDeclNode& node)
{
    printNodeTitle(node);
    _formater.format(" name:%s type:%s \n",
                     node.argumentName(),
                     node.argumentType()->typeString());
    _formater.write(_sink);
}

void
AstPrinter::visit(ArgumentDeclsNode& node)
{
    printNodeTitle(node, true);

    yal_u32 idx = 1;
    for(auto& v : node.arguments())
    {
        onDescent(idx == node.argumentCount());
        v->accept(*this);
        onAscend();
        ++idx;
    }
}

void
AstPrinter::visit(FunctionCallArgsNode& node)
{
    printNodeTitle(node, true);
    size_t idx = 1;
    for(auto& v : node.expressions)
    {
        onDescent(idx == node.expressions.size());
        v->accept(*this);
        onAscend();
    }
}

void
AstPrinter::visit(VariableDeclNode& node)
{
    printNodeTitle(node);
    _formater.format(" %s \n",  node.variableName());
    _formater.write(_sink);
    onDescent(true);
    node.expression()->accept(*this);
    onAscend();
}

void
AstPrinter::visit(DualOperatorNode& node)
{
    printNodeTitle(node);
    _formater.format("%s\n",
                     OperatorTypeToStr(node.dualOperatorType()));
    _formater.write(_sink);
    onDescent(false);
    node.leftExpression()->accept(*this);
    onAscend();
    onDescent(true);
    node.rightExpression()->accept(*this);
    onAscend();
}

void
AstPrinter::visit(SingleOperatorNode& node)
{
    printNodeTitle(node);
    _formater.format("%s \n",
                     OperatorTypeToStr(node.singleOperatorType()));
    _formater.write(_sink);
    onDescent(true);
    node.expression()->accept(*this);
    onAscend();
}

void
AstPrinter::visit(FunctionCallNode& node)
{
    printNodeTitle(node);
    _formater.format("%s \n", node.functionName());
    _formater.write(_sink);
    if (node.hasFunctionArguments())
    {
        onDescent(true);
        node.functionArguments()->accept(*this);
        onAscend();
    }
}

void
AstPrinter::visit(FunctionDeclNode& node)
{
    printNodeTitle(node);
    _formater.format("%s \n", node.functionName());
    _formater.write(_sink);

    if (node.hasFunctionArguments())
    {
        onDescent(false);
        node.functionArguments()->accept(*this);
        onAscend();
    }

    onDescent(true);
    node.functionCode()->accept(*this);
    onAscend();
}

void
AstPrinter::visit(ConditionNode& node)
{
    printNodeTitle(node, true);

    if (node.condition())
    {
        onDescent(!node.onTrue() && !node.onFalse());
        node.condition()->accept(*this);
        onAscend();
    }

    if (node.onTrue())
    {
        onDescent(!node.onFalse());
        node.onTrue()->accept(*this);
        onAscend();
    }

    if (node.onFalse())
    {
        onDescent(true);
        node.onFalse()->accept(*this);
        onAscend();
    }
}

void
AstPrinter::visit(ReturnNode& node)
{
    printNodeTitle(node, true);
    onDescent(true);
    node.expression()->accept(*this);
    onAscend();
}

void
AstPrinter::visit(PrintNode& node)
{
    printNodeTitle(node, true);
    onDescent(true);
    node.arguments()->accept(*this);
    onAscend();
}

void
AstPrinter::visit(PrintArgsNode& node)
{
    printNodeTitle(node, true);
    size_t idx = 1;
    for(auto& v : node.expressions)
    {
        onDescent(idx == node.expressions.size());
        v->accept(*this);
        onAscend();
    }
}

void
AstPrinter::visit(WhileLoopNode& node)
{
    printNodeTitle(node, true);

    onDescent(false);
    node.condition()->accept(*this);
    onAscend();

    onDescent(true);
    node.code()->accept(*this);
    onAscend();
}

}
