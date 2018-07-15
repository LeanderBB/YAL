/*
 *  Copyright 2018 by Leander Beernaert (lbb-dev@pm.me)
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


#include "yal/util/format.h"
#include "yal/io/bytestream.h"

namespace yal {

    void
    FormatWrite(ByteStream& stream,
                const Formater& formater){
        if (formater.bufferPos != 0) {
            stream.write(formater.buffer, formater.bufferPos);
        }
    }

    void
    FormatWriteWithLinePrefix(ByteStream& stream,
                              const Formater& formater,
                              const StringRef linePrefix) {
        const StringRef formatStr = formater.toStringRef();
        FormatWriteWithLinePrefix(stream, formatStr, linePrefix);
    }

    void FormatWriteWithLinePrefix(ByteStream& stream,
                                   const StringRef formatStr,
                                   const StringRef linePrefix) {
        if (linePrefix.size() == 0) {
            stream.write(formatStr.data(), formatStr.size());
            return;
        }

        size_t prevLineLoc = 0;
        size_t newLineLoc = formatStr.findFirstOf('\n');
        while(newLineLoc != StringRef::npos) {
            newLineLoc++;
            const StringRef substr = formatStr.subStr(prevLineLoc,
                                                      newLineLoc - prevLineLoc);
            stream.write(linePrefix.data(), linePrefix.size());
            stream.write(substr.data(), substr.size());

            prevLineLoc = newLineLoc;
            newLineLoc = formatStr.findFirstOf(prevLineLoc,'\n');
        }

        if (prevLineLoc == 0) {
            stream.write(linePrefix.data(), linePrefix.size());
        }
        if (prevLineLoc < formatStr.size())
        {
            const StringRef substr = formatStr.subStr(prevLineLoc, formatStr.length() - prevLineLoc);
            stream.write(substr.data(), substr.size());
        }
    }

}
