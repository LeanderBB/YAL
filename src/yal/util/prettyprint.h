#pragma once


namespace yal {
    class ByteStream;
    class Log;

    class PrettyPrint {
    public:
        static void SourceErrorPrint(ByteStream& stream,
                                     Log& log,
                                     const size_t lineStart,
                                     const size_t columnStart);
    };
}
