#pragma once
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
#include <yal/yal.h>
#include <stdarg.h>
namespace yal{

    class ByteStream;
    class Log {
    public:

        class MultiLineScope {
        public:
            MultiLineScope(Log& log);
            ~MultiLineScope();
            YAL_NO_COPY_CLASS(MultiLineScope);
        private:
            Log& m_log;
        };

        Log(ByteStream& stream);

        Log(ByteStream& out,
            ByteStream& error);

        void message(const char* format, ...);

        void warning(const char* format, ...);

        void error(const char* format, ...);

        void debug(const char* format, ...);

    private:

        size_t formatWithTag(const char* tag,
                             const size_t tagLen,
                             const char* format,
                             ::va_list args);


    private:
        enum {kInternalBufferSizBytes = 4096};
        char m_buffer[kInternalBufferSizBytes];
        ByteStream& m_out;
        ByteStream& m_error;
        bool m_multiLine;
    };

}
