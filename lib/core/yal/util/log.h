/*
 *  Copyright 2017 by Leander Beernaert (lbb-dev@pm.me)
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

#pragma once
#include <yal/yal.h>
#include <stdarg.h>
#include <yal/util/format.h>
namespace yal{

    class ByteStream;
    class Log {
    private:
        static const StringRef kTagMessage;
        static const StringRef kTagError;
        static const StringRef kTagWarning;
        static const StringRef kTagDebug;

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

        template <typename ...Args>
        void message(const char* format,
                     const Args&... args) {
            Format(m_formater, format, args...);
            log(m_out, kTagMessage);
        }

        template <typename ...Args>
        void warning(const char* format,
                     const Args&... args) {
            Format(m_formater, format, args...);
            log(m_out, kTagWarning);
        }

        template <typename ...Args>
        void error(const char* format,
                   const Args&... args) {
            Format(m_formater, format, args...);
            log(m_error, kTagError);
        }

        template <typename ...Args>
        void debug(const char* format,
                   const Args&... args) {
            Format(m_formater, format, args...);
            log(m_out, kTagDebug);
        }

    private:

        void log(ByteStream& stream,
                 const StringRef& tag);


    private:
        enum {kInternalBufferSizBytes = 4096};
        FormaterStack<kInternalBufferSizBytes> m_formater;
        ByteStream& m_out;
        ByteStream& m_error;
        bool m_multiLine;
        bool m_firstLine;
    };

}
