#include "yal/ast/astprinter.h"
#include "yal/ast/asthdrs.h"
#include "yal/types/typehdrs.h"
#include <cstdlib>
#include <iomanip>
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
        _sink <<"-" << node.astTypeStr()
             <<"<ln " << std::setw(4) << std::setfill('0') << src_info.firstLine
            << ":" << std::setw(4) << std::setfill('0') << src_info.lastLine
            << "  cl " << std::setw(4) << std::setfill('0') << src_info.firstColumn
            << ":" << std::setw(4) << std::setfill('0') << src_info.lastColumn << "> ";
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
    _sink << "type:" << node.constantType()->typeString() << "=";
    const ConstantValue& value = node.constantValue();
    switch(value.type())
    {
    case kConstantTypeBool:
        _sink << value.valueAsBoolean();
        break;
    case kConstantTypeFloat32:
        _sink << value.valueAsFloat32();
        break;
    case kConstantTypeFloat64:
        _sink << value.valueAsFloat64();
        break;
    case kConstantTypeInt32:
        _sink << value.valueAsInt32();
        break;
    case kConstantTypeInt64:
        _sink << value.valueAsInt64();
        break;
    case kConstantTypeUInt32:
        _sink << value.valueAsUInt32();
        break;
    case kConstantTypeUInt64:
        _sink << value.valueAsUInt64();
        break;
    case kConstantTypeText:
        _sink << "\"" << value.valueAsText() << "\"";
        break;
    default:
        _sink <<"(print not yet implemented)";
    }
    _sink << std::endl;
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
    _sink << "'" << node.variableName() << "'";
    if (node.hasExplicitType())
    {
        _sink << " explicitType='" << node.explicitType()->typeString() << "'";
    }
    _sink << std::endl;
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
    _sink << "'" << node.nativeFunctionName() << "' : '" << node.returnValueType()->typeString() << "'" << std::endl;

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
        ++idx;
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
    _sink << "'" << node.nativeFunctionName() << "' : '" << node.returnValueType()->typeString() << "'" << std::endl;
}

void
AstPrinter::visit(ParentExpNode& node)
{
    printNodeTitle(node, true);
    onDescent(true);
    node.expression()->accept(*this);
    onAscend();
}

void
AstPrinter::visit(ArrayCtrNode& node)
{
    printNodeTitle(node, node.expressionCount() != 0);
    const size_t count = node.expressionCount();
    if (count)
    {
        size_t idx = 1;
        for(auto& v : node.expressions())
        {
            onDescent(idx == count);
            v->accept(*this);
            onAscend();
            ++idx;
        }
    }
    else
    {
        _sink << " "<< node.explicitType()->typeString() << std::endl;
    }
}

void
AstPrinter::visit(UserTypeMemberNode& node)
{
    printNodeTitle(node);
    _sink << "name: '" << node.memberName()
          << "' type:'" <<  node.memberType()->typeString() << "'" << std::endl;
}

void
AstPrinter::visit(UserTypeMembersNode& node)
{
    printNodeTitle(node, true);

    size_t idx = 1;
    const size_t count = node.memberCount();
    for(auto& member : node.members())
    {
        onDescent(idx == count);
        member->accept(*this);
        onAscend();
        ++idx;
    }
}

void
AstPrinter::visit(UserTypeDeclNode& node)
{
    printNodeTitle(node);
    _sink << "'" << node.userTypeName() << "'" << std::endl;
    onDescent(true);
    node.members()->accept(*this);
    onAscend();
}


}
