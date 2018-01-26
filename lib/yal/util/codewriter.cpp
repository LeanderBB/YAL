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

#include "yal/util/codewriter.h"
#include "yal/io/bytestream.h"
namespace yal{

    CodeWriter::CodeWriter(ByteStream& output,
                           const uint32_t identSpacing):
        m_stream(output),
        m_formaterIdent(),
        m_formaterText(),
        m_identLevel(0),
        m_identSpacing(identSpacing),
        m_isNewLine(true) {

    }

    void
    CodeWriter::write() {
        const char ch = '\n';
        m_stream.write(&ch, 1);
        m_isNewLine = true;
    }

    void
    CodeWriter::ident() {
        m_identLevel += m_identSpacing;
    }

    void
    CodeWriter::uindent() {
        YAL_ASSERT(m_identLevel >= m_identSpacing);
        m_identLevel -= m_identSpacing;
    }

    void
    CodeWriter::writeToStream() {
        // Write identation, if any
        if (m_isNewLine) {
            writeIdent();
            m_isNewLine = false;
        }

        const StringRef text = m_formaterText.toStringRef();
        size_t newLinePos = text.findFirstOf('\n');
        size_t currPos = 0;
        if (newLinePos == StringRef::npos) {
            // If no newline found, write entire line
            m_stream.write(text.data(), text.size());
        } else {
            // Incrementally write new line
            while(newLinePos != StringRef::npos) {

                // find consecutive \n characters and group them together
                size_t nextNewLine = newLinePos + 1;
                while(nextNewLine < text.size() && text[nextNewLine] == '\n') {
                    ++nextNewLine;
                }
                newLinePos = std::max(newLinePos, nextNewLine - 1);

                // write new line
                const StringRef subStr = text.subStr(currPos, (newLinePos + 1) - currPos);

                m_stream.write(subStr.data(), subStr.size());
                if (newLinePos == text.size() -1) {
                    // if \n is the last character, mark new line
                    m_isNewLine = true;
                } else {
                    // if next character is not \n, write ident
                    writeIdent();
                }

                currPos = newLinePos + 1;
                newLinePos =  text.findFirstOf(newLinePos + 1, '\n');
            }

            // Write remaining text
            if (currPos < text.size()) {
                const StringRef subStr = text.subStr(currPos);
                m_stream.write(subStr.data(), subStr.size());
            }
        }
    }

    void
    CodeWriter::writeIdent() {
        if (m_identLevel > 0) {
            Format(m_formaterIdent, "%", FormatIdent<const char*>(m_identLevel,' ',""));
            const StringRef identStr = m_formaterIdent.toStringRef();
            m_stream.write(identStr.data(), identStr.size());
        }
    }
}
