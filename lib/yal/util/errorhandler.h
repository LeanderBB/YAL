#ifndef __YAL_ERRORHANDLER_H__
#define __YAL_ERRORHANDLER_H__

namespace yal
{

class SourceLocationInfo
{
public:
    SourceLocationInfo():
        firstLine(-1),
        firstColumn(-1),
        lastLine(-1),
        lastColumn(-1)
    {
    }

    SourceLocationInfo(const int firstln,
                       const int firstcl,
                       const int lastln,
                       const int lastcln):
        firstLine(firstln),
        firstColumn(firstcl),
        lastLine(lastln),
        lastColumn(lastcln)
    {

    }

    bool valid() const
    {
        return firstLine != -1
                && firstColumn != -1
                && lastLine != -1
                && lastColumn != -1;
    }

    int firstLine;
    int firstColumn;
    int lastLine;
    int lastColumn;
};


class ErrorHandler
{
public:
    ErrorHandler(){}
    virtual ~ErrorHandler(){}

    virtual void onCompileError(const char* message,
                                const int msgLen,
                                const SourceLocationInfo& location) =0;

    virtual void onError(const char* message,
                         const int msgLen) = 0;

private:
};

}

#endif
