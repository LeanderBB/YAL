/*
 *  Copyright 2017 by Leander Beernaert (leanderbb@gmail.com)
 *
 *  This file is part of YAL.
 *
 *  YAL is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, either version 3
 *  of the License, or (at your option) any later version.
 *
 *  YAL is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with YAL. If not, see <http://www.gnu.org/licenses/>.
 */
#include "yal/yal.h"
#include "yal/util/prettyprint.h"
#include "yal/io/bytestream.h"
#include "yal/util/log.h"
#include "yal/io/sourcemanager.h"
#include "yal/io/memorystream.h"
namespace yal {

    void
    PrettyPrint::SourceErrorPrint(SourceItem& sourceItem,
                                  Log& log,
                                  const size_t lineStart,
                                  const size_t lineEnd,
                                  const size_t columnStart,
                                  const size_t columnEnd) {
        ByteStream& stream = sourceItem.getByteStream();
        if (stream.isSeekable())
        {
            stream.seek(0);
            for(size_t i= 1; i < lineStart;++i) {
                stream.skipLine();
            }
            const std::string line = stream.readLine();
            log.error("In %:%:%\n", sourceItem.getPath(), lineStart, columnStart);
            log.error("    %\n", line.c_str());
            if (lineStart != lineEnd) {
                log.error("    %", FormatIdent<const char*>(columnStart-1,' ',"^~~~\n"));
            } else {
                log.error("    %%", FormatIdent<const char*>(columnStart-1,' ',"^"),
                          FormatIdent<const char*>(columnEnd-columnStart,'~',"\n"));
            }
        }
    }


    void
    PrettyPrint::SourceErrorPrint( Log& log,
                                   const SourceInfo& info,
                                   const SourceManager &manager) {
        SourceItemOpt item =  manager.getItem(info.handle);
        if (item.has_value()) {
            SourceErrorPrint(*item.value(),
                             log,
                             info.begin.line,
                             info.end.line,
                             info.begin.column,
                             info.end.column);
        }
    }
}
