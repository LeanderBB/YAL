#ifndef __YAL_PARSEEXCEPTION_H__
#define __YAL_PARSEEXCEPTION_H__

#include "yal/parser/bison_utils.h"
#include <exception>

namespace yal
{

class ParseException : public std::exception
{
public:
    ParseException(const char* text,
                   const SourceLocationInfo& loc):
        _message(text),
        _location(loc)
    {
    }

    virtual const char* what() const throw()
    {
        return _message.c_str();
    }

    const SourceLocationInfo& location() const
    {
        return _location;
    }

private:
    const std::string _message;
    const SourceLocationInfo _location;
};


}

#endif

