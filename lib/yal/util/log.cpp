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
#include "yal/util/log.h"
#include "yal/io/bytestream.h"
#include <cstdio>
#include <cstring>
#include <algorithm>

namespace yal{

    const StringRef Log::kTagMessage ("[ INFO] ");
    const StringRef Log::kTagError   ("[ERROR] ");
    const StringRef Log::kTagWarning ("[ WARN] ");
    const StringRef Log::kTagDebug   ("[DEBUG] ");
    static const StringRef kMultiLine("        ");

    Log::Log(ByteStream& stream):
        Log(stream, stream){

    }

    Log::Log(ByteStream& out,
             ByteStream& error):
        m_formater(),
        m_out(out),
        m_error(error),
        m_multiLine(false),
        m_firstLine(true){

    }

    Log::MultiLineScope::MultiLineScope(Log& log):
        m_log(log) {
        m_log.m_multiLine = true;
        m_log.m_firstLine = false;
    }

    Log::MultiLineScope::~MultiLineScope() {
        m_log.m_multiLine = false;
        m_log.m_firstLine = true;
    }

    void
    Log::log(ByteStream& stream,
             const StringRef& tag){


        if (m_multiLine && !m_firstLine) {
            stream.write(kMultiLine.data(),kMultiLine.size());
        } else {
            stream.write(tag.data(), tag.size());
        }

        if (m_formater.bufferPos > 0) {
            stream.write(m_formater.buffer,m_formater.bufferPos);
            if(m_multiLine && m_firstLine) {
                if(::memchr(m_formater.buffer, '\n', m_formater.bufferPos) != nullptr) {
                    m_firstLine = false;
                }
            }
        }
    }

}
