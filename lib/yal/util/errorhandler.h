#ifndef __YAL_ERRORHANDLER_H__
#define __YAL_ERRORHANDLER_H__

namespace yal
{

struct SourceLocationInfo
{
    int firstLine = 0;
    int firstColumn = 0;
    int lastLine = 0;
    int lastColumn = 0;
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
