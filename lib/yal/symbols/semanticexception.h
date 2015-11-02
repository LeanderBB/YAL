#ifndef __YAL_SEMANTICEXCEPTION_H__
#define __YAL_SEMANTICEXCEPTION_H__


#include <exception>
#include <sstream>
#include "yal/ast/astbasenode.h"

namespace yal
{

class SemanticException : public std::exception
{
public:
    SemanticException(const char* text,
                      const AstBaseNode& node):
        _message(text),
        _node(node)
    {
    }

    SemanticException(const std::string& str,
                      const AstBaseNode& node):
        _message(str),
        _node(node)
    {
    }


    virtual const char* what() const throw()
    {
        return _message.c_str();
    }

    const AstBaseNode& astNode() const
    {
        return _node;
    }

private:
    const std::string _message;
    const AstBaseNode& _node;
};


}

#endif

