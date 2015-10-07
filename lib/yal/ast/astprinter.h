#ifndef __YAL_ASTPRINTER_H__
#define __YAL_ASTPRINTER_H__

#include "yal/yal.h"
#include "yal/parser/parser_state.h"
#include "yal/ast/astnodevisitor.h"
#include "yal/util/outputformater.h"
#include <vector>

namespace yal
{

class AstPrinter : public AstNodeVisitor
{
public:
    enum
    {
        kNumIdent = 1
    };

    AstPrinter(OutputSink& sink);

    virtual ~AstPrinter();

#define YAL_NODE_LIST_FUNC(CLASS) virtual void visit(CLASS& node) override;
#include "yal/ast/astnodelist.h"
#undef YAL_NODE_LIST_FUNC

    void process(ParserState_t& state);

private:

    void printIdent();

    void printNodeTitle(const AstBaseNode& node,
                        const bool empty = false);

    void onDescent(const bool lastNode);

    void onAscend();

    void onSubDescent();

    void onSubAscend();
private:
    std::vector<char> _idents;
    OutputFormater _formater;
    OutputSink& _sink;
};

}
#endif
