#ifndef __YAL_BYTECODEGENEXCEPTION_H__
#define __YAL_BYTECODEGENEXCEPTION_H__

#include <exception>
#include <string>
#include "yal/ast/astbasenode.h"

namespace yal
{

class ByteCodeGenException : public std::exception
{
public:
    ByteCodeGenException(const char* text,
                         const AstBaseNode& node):
        _message(text),
        _node(node)
    {
    }

    ByteCodeGenException(const std::string& str,
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

