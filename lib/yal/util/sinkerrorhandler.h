#ifndef __YAL_SINKERRORHANDLER_H__
#define __YAL_SINKERRORHANDLER_H__

#include "yal/util/errorhandler.h"
#include "yal/util/outputsink.h"
#include "yal/util/outputformater.h"

namespace yal
{
class SinkErrorHandler : public yal::ErrorHandler
{
public:
    SinkErrorHandler(yal::OutputSink& sink):
        ErrorHandler(),
        _sink(sink)
    {

    }

    virtual ~SinkErrorHandler(){}

    virtual void onCompileError(const char* message,
                         const int msgLen,
                         const yal::SourceLocationInfo& location)
    {
        (void) msgLen;
        _formater.formatAndWrite(_sink,"[%d:%d] %s\n",
                                 location.firstLine,
                                 location.firstColumn,
                                 message);
    }

    virtual void onError(const char* message,
                         const int msgLen)
    {
        (void) msgLen;
        _formater.formatAndWrite(_sink, "%s\n", message);
    }

private:
    yal::OutputSink& _sink;
    OutputFormater _formater;
};

}

#endif

