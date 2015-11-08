#include "yal/ast/astprinter.h"
#include "yal/ast/asthdrs.h"
#include <cstdlib>

namespace yal
{

AstPrinter::AstPrinter(std::ostream &sink):
    _sink(sink)
{

}

AstPrinter::~AstPrinter()
{

}

void
AstPrinter::process(const ParserState_t &state)
{
    _sink << "Module" << std::endl;
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
        _sink << ch;
    }
}

void
AstPrinter::printNodeTitle(const AstBaseNode& node,
                           const bool empty)
{
    printIdent();
    const SourceLocationInfo& src_info = node.locationInfo();

    if (node.isSourceAstNode())
    {
        _sink <<"-" <<  node.astTypeStr()
            <<"<ln " << src_info.firstLine
            << ":" << src_info.lastLine
            << "  cl " << src_info.firstColumn
            << ":" << src_info.lastColumn << "> ";
    }
    else
    {
        _sink <<"-" <<  node.astTypeStr() << " ";
    }

    if (empty)
    {
        _sink << std::endl;
    }
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
    _sink <<"'" << OperatorTypeToStr(node.assignOperatorType())
         << (node.assignOperatorType() == kOperatorTypeCopy ? "" : "=")
         << " '" << std::endl;

    onDescent(false);
    node.expressionLeft()->accept(*this);
    onAscend();
    onDescent(true);
    node.expressionRight()->accept(*this);
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
    _sink <<"'" << OperatorTypeToStr(node.compareOperatorType()) << "'" << std::endl;
    onDescent(false);
    node.expressionLeft()->accept(*this);
    onAscend();
    onDescent(true);
    node.expressionRight()->accept(*this);
    onAscend();
}

void
AstPrinter::visit(ConstantNode& node)
{
    printNodeTitle(node);
    _sink << "type:" << ConstantTypeToStr(node.constantValue().type()) << std::endl;
}

void
AstPrinter::visit(ArgumentDeclNode& node)
{
    printNodeTitle(node);
    _sink << "name: '" << node.argumentName()
          << "' type:'" <<  node.argumentType()->typeString() << "'" << std::endl;
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
    _sink << "'" << node.variableName() << "'" << std::endl;
    onDescent(true);
    node.expression()->accept(*this);
    onAscend();
}

void
AstPrinter::visit(DualOperatorNode& node)
{
    printNodeTitle(node);
    _sink << "'" << OperatorTypeToStr(node.dualOperatorType()) << "'" << std::endl;
    onDescent(false);
    node.expressionLeft()->accept(*this);
    onAscend();
    onDescent(true);
    node.expressionRight()->accept(*this);
    onAscend();
}

void
AstPrinter::visit(SingleOperatorNode& node)
{
    printNodeTitle(node);
    _sink << "'" << OperatorTypeToStr(node.singleOperatorType()) << "'" << std::endl;
    onDescent(true);
    node.expression()->accept(*this);
    onAscend();
}

void
AstPrinter::visit(FunctionCallNode& node)
{
    printNodeTitle(node);
    _sink << "'" << node.functionName() << "'" << std::endl;
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
    _sink << "'" << node.functionName() << "' : '" << node.returnValueType()->typeString() << "'" << std::endl;

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

void
AstPrinter::visit(VariableAccessNode& node)
{
    printNodeTitle(node);
    _sink << "'" << node.variableName() << "'" << std::endl;
}

void
AstPrinter::visit(StringCreateNode& node)
{
    printNodeTitle(node, true);
    onDescent(true);
    node.constantNode()->accept(*this);
    onAscend();
}

void
AstPrinter::visit(ObjectCreateNode& node)
{
    printNodeTitle(node, true);
    onDescent(true);
    node.expression()->accept(*this);
    onAscend();
}

void
AstPrinter::visit(ObjectRetainNode& node)
{
    printNodeTitle(node, true);
    onDescent(true);
    node.expression()->accept(*this);
    onAscend();
}

void
AstPrinter::visit(ObjectReleaseNode& node)
{
    printNodeTitle(node, true);
    onDescent(true);
    node.expression()->accept(*this);
    onAscend();
}

void
AstPrinter::visit(FunctionDeclNativeNode& node)
{
    printNodeTitle(node);
    _sink << "'" << node.functionName() << "' : '" << node.returnValueType()->typeString() << "'" << std::endl;
}

}
