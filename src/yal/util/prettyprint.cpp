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
            log.error("%\n", line.c_str());
            log.error("%", FormatIdent<const char*>(columnStart-1,' ',"^~~~\n"));
        }
    }


    void
    PrettyPrint::SourceErrorPrint( Log& log,
                                   const SourceInfo& info,
                                   const SourceManager &manager) {
        SourceItem* item =  manager.getItem(info.handle);
        if (item != nullptr) {
            SourceErrorPrint(item->getByteStream(),
                             log,
                             info.begin.line, info.begin.column);
        }
    }
}
