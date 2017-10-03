#include "yal/yal.h"
#include "yal/util/prettyprint.h"
#include "yal/io/bytestream.h"
#include "yal/util/log.h"

namespace yal {

    void
    PrettyPrint::SourceErrorPrint(ByteStream& stream,
                                  Log& log,
                                  const size_t lineStart,
                                  const size_t columnStart) {
        if (stream.isSeekable())
        {
            stream.seek(0);
            for(size_t i= 1; i < lineStart;++i) {
                stream.skipLine();
            }
            const std::string line = stream.readLine();
            log.error("%s\n", line.c_str());
            log.error("%*s%s", columnStart-1,"","^~~~\n");
        }
    }

}
