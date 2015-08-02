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

#include <cstdlib>

namespace yal
{

AstPrinter::AstPrinter(OutputSink &sink):
    _formater(),
    _sink(sink),
    _ident(0)
{

}

AstPrinter::~AstPrinter()
{

}

void
AstPrinter::process(ParserState_t &state)
{
    for (auto& v : state.program)
    {
        v->accept(*this);
    }
}

void
AstPrinter::printIdent()
{
    for (size_t i = 0; i < _ident; ++i)
    {
        _formater.format(" ");
        _formater.write(_sink);
    }
}

void
AstPrinter::visit(AssignOperatorNode& node)
{
    printIdent();
    _formater.format("%s> %s var:%s\n", node.nodeTypeStr(),
                     AssignOperatorTypeToSt(node.assignOperatorType()),
                     node.variableName());
    _formater.write(_sink);

    _ident += kNumIdent;
    node.expression()->accept(*this);
    _ident -= kNumIdent;
}

void
AstPrinter::visit(CodeBodyNode& node)
{
    printIdent();
    _formater.format("%s>\n", node.nodeTypeStr());
    _formater.write(_sink);
    _ident += kNumIdent;
    for(auto& v : node.statements)
    {
        v->accept(*this);
    }
    _ident -= kNumIdent;
}

void
AstPrinter::visit(CompareOperatorNode& node)
{
    printIdent();
    _formater.format("%s> %s \n", node.nodeTypeStr(),
                     CompareOperatorToStr(node.compareOperatorType()));
    _formater.write(_sink);
    _ident += kNumIdent;
    printIdent();
    _formater.format("Left>\n");
    _formater.write(_sink);
    _ident += kNumIdent;
    node.leftExpression()->accept(*this);
    _ident -= kNumIdent;
    printIdent();
    _formater.format("Right>\n");
    _formater.write(_sink);
    _ident += kNumIdent;
    node.rightExpression()->accept(*this);
    _ident -= kNumIdent;
    _ident -= kNumIdent;
}

void
AstPrinter::visit(ConstantNode& node)
{
    printIdent();
    _formater.format("%s> type:%s \n", node.nodeTypeStr(),
                     ConstantTypeToStr(node.constantType()));
    _formater.write(_sink);
}

void
AstPrinter::visit(ArgumentDeclNode& node)
{
    printIdent();
    if (node.isCostumType())
    {
        _formater.format("%s> name:%s type:%s \n",node.nodeTypeStr(),
                         node.argumentName(),
                         node.argumentId());
    }
    else
    {
        _formater.format("%s> name:%s type:%s \n",node.nodeTypeStr(),
                         node.argumentName(),
                         ConstantTypeToStr(node.argumentType()));
    }
    _formater.write(_sink);
}

void
AstPrinter::visit(ArgumentDeclsNode& node)
{
    printIdent();
    _formater.format("%s>\n", node.nodeTypeStr());
    _formater.write(_sink);
    _ident += kNumIdent;
    for(auto& v : node.arguments())
    {
        v->accept(*this);
    }
    _ident -= kNumIdent;
}

void
AstPrinter::visit(ExpressionList& node)
{
    printIdent();
    _formater.format("%s>\n", node.nodeTypeStr());
    _formater.write(_sink);
    _ident += kNumIdent;
    for(auto& v : node.expressions)
    {
        v->accept(*this);
    }
    _ident -= kNumIdent;
}

void
AstPrinter::visit(VariableDeclNode& node)
{
    printIdent();
    _formater.format("%s> %s \n", node.nodeTypeStr(), node.variableName());
    _formater.write(_sink);
    _ident += kNumIdent;
    node.expression()->accept(*this);
    _ident -= kNumIdent;
}

void
AstPrinter::visit(DualOperatorNode& node)
{
    printIdent();
    _formater.format("%s> %s \n", node.nodeTypeStr(),
                    DualOperatorTypeToStr(node.dualOperatorType()));
    _formater.write(_sink);
    _ident += kNumIdent;
    printIdent();
    _formater.format("Left>\n");
    _formater.write(_sink);
    _ident += kNumIdent;
    node.leftExpression()->accept(*this);
    _ident -= kNumIdent;
    printIdent();
    _formater.format("Right>\n");
    _formater.write(_sink);
    _ident += kNumIdent;
    node.rightExpression()->accept(*this);
    _ident -= kNumIdent;
    _ident -= kNumIdent;
}



void
AstPrinter::visit(SingleOperatorNode& node)
{
    printIdent();
    _formater.format("%s> %s \n", node.nodeTypeStr(),
                    SingleOperatorTypeToStr(node.singleOperatorType()));
    _formater.write(_sink);
    _ident += kNumIdent;
    node.expression()->accept(*this);
    _ident -= kNumIdent;
}

void
AstPrinter::visit(FunctionCallNode& node)
{
    printIdent();
    _formater.format("%s> %s \n", node.nodeTypeStr(), node.functionName());
    _formater.write(_sink);
    _ident += kNumIdent;
    printIdent();
    _formater.format("Args>\n");
    _formater.write(_sink);
    _ident += kNumIdent;
    if (node.hasFunctionArguments())
    {
        node.functionArguments()->accept(*this);
    }
    else
    {
        printIdent();
        _formater.format("void\n");
        _formater.write(_sink);
    }
    _ident -= kNumIdent;
    _ident -= kNumIdent;
}

void
AstPrinter::visit(FunctionDeclNode& node)
{
    printIdent();
    _formater.format("%s> %s \n",node.nodeTypeStr(), node.functionName());
    _formater.write(_sink);
    _ident += kNumIdent;
    printIdent();
    _formater.format("Args>\n");
    _formater.write(_sink);
    _ident += kNumIdent;
    if (node.hasFunctionArguments())
    {
        node.functionArguments()->accept(*this);
    }
    else
    {
        printIdent();
        _formater.format("void\n");
        _formater.write(_sink);
    }
    _ident -= kNumIdent;
    printIdent();
    _formater.format("Code>\n");
    _formater.write(_sink);
    _ident += kNumIdent;
    node.functionCode()->accept(*this);
    _ident -= kNumIdent;
    _ident -= kNumIdent;
}

void
AstPrinter::visit(ConditionNode& node)
{
    printIdent();
    _formater.format("%s>\n", node.nodeTypeStr());
    _formater.write(_sink);
    _ident += kNumIdent;
    printIdent();
    _formater.format("Condition>\n");
    _formater.write(_sink);
    _ident += kNumIdent;

    if (node.condition())
    {
        node.condition()->accept(*this);
    }
    else
    {
        printIdent();
        _formater.format("null\n");
        _formater.write(_sink);
    }
    _ident -= kNumIdent;
    printIdent();
    _formater.format("True>\n");
    _formater.write(_sink);
    _ident += kNumIdent;
    if (node.onTrue())
    {
        node.onTrue()->accept(*this);
    }
    else
    {
        printIdent();
        _formater.format("null\n");
        _formater.write(_sink);
    }
    _ident -= kNumIdent;
    printIdent();
    _formater.format("False>\n");
    _formater.write(_sink);
    _ident += kNumIdent;
    if (node.onFalse())
    {
        node.onFalse()->accept(*this);
    }
    else
    {
        printIdent();
        _formater.format("null\n");
        _formater.write(_sink);
    }
    _ident -= kNumIdent;
    _ident -= kNumIdent;
}

void
AstPrinter::visit(ReturnNode& node)
{
    printIdent();
    _formater.format("%s>\n", node.nodeTypeStr());
    _formater.write(_sink);
    _ident += kNumIdent;
    node.expression()->accept(*this);
    _ident -= kNumIdent;
}





}
