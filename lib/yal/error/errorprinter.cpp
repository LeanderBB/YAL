/*
 *  Copyright 2018 by Leander Beernaert (leanderbb@gmail.com)
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

#include "yal/error/errorprinter.h"

#include "yal/error/error.h"
#include "yal/error/errorreporter.h"
#include "yal/io/bytestream.h"
#include "yal/io/ansicolorcodes.h"
#include "yal/io/memorystream.h"
#include "yal/io/sourcemanager.h"
#include "yal/util/formathex.h"


namespace yal {

    ErrorPrinter::ErrorPrinter(ByteStream& stream,
                               SourceManager& srcManager):
        ErrorPrinter(stream, stream, srcManager){

    }

    ErrorPrinter::ErrorPrinter(ByteStream& msgStream,
                               ByteStream& errStream,
                               SourceManager& srcManager):
        m_formater(),
        m_errStream(errStream),
        m_msgStream(msgStream),
        m_srcManager(srcManager),
        m_useColorCodes(0) {

        (void) m_srcManager;
        (void) m_msgStream;
    }

    void
    ErrorPrinter::print(const ErrorReporter &reporter) {
        auto callable = [this](const Error& error) {

            {
                const char* errorTypeText = getErrorTypeString(error);
                Format(m_formater,
                       m_useColorCodes
                       ? "% " YAL_CLRCD_BEGIN YAL_CLRCD_BOLD \
                         YAL_CLRCD_END "[%]" YAL_CLRCD_RESET " %\n"
                       :"% [%] %\n",
                       errorTypeText,
                       FormatHex<ErrorCode>(error.getCode()),
                       error.getErrorName());
                FormatWrite(m_errStream, m_formater);
            }

            FormatReset(m_formater);
            const SourceInfo& srcInfo = error.getSourceInfo();
            SourceItem* item =  m_srcManager.getItem(srcInfo.handle);
            if (item != nullptr) {
                printSourceInfo(m_errStream, *item, srcInfo);
            }

            Format(m_formater, "> ");
            error.printDetail(*this);
            FormatWriteWithLinePrefix(m_errStream, m_formater, "  | ");
        };

        reporter.forEarchError(callable);
    }

    void
    ErrorPrinter::enableColorCodes(const bool value) {
        m_useColorCodes = value ? 1 : 0;
    }

    void
    ErrorPrinter::printSourceInfo(ByteStream& output,
                                  SourceItem& item,
                                  const SourceInfo& srcInfo) {
        const size_t bytesWritten = printSourceInfo(m_formater,
                                                    item,
                                                    srcInfo);
        if (bytesWritten != 0) {
            FormatWriteWithLinePrefix(output, m_formater, "  | ");
        }
    }

    const char*
    ErrorPrinter::getErrorTypeString(const Error &error) const {
        if (m_useColorCodes == 1) {
            if (error.isError()) {
                return YAL_CLRCD_BEGIN YAL_CLRCD_BOLD YAL_CLRCD_CONCAT \
                        YAL_CLRCD_COLOR_RED YAL_CLRCD_END "error:" YAL_CLRCD_RESET;
            } else if (error.isWarning()) {
                return YAL_CLRCD_BEGIN YAL_CLRCD_BOLD YAL_CLRCD_CONCAT \
                        YAL_CLRCD_COLOR_YELLOW YAL_CLRCD_END "warning:" YAL_CLRCD_RESET;
            } else if (error.isSuggestion()) {
                return YAL_CLRCD_BEGIN YAL_CLRCD_BOLD YAL_CLRCD_END "sugestion:" YAL_CLRCD_RESET;
            }
        } else {
            if (error.isError()) {
                return "error: ";
            } else if (error.isWarning()) {
                return "warning: ";
            } else if (error.isSuggestion()) {
                return "suggestion: ";
            }
        }
        YAL_ASSERT_MESSAGE(false, "Unknown error type");
        return "unknown: ";
    }


    size_t
    ErrorPrinter::printSourceInfo(Formater& formater,
                                  SourceItem& item,
                                  const SourceInfo& srcInfo)
    {
        size_t bytesWritten =0;
        MemoryStream& stream = item.getByteStream();
        if (stream.isSeekable())
        {
            // print item location
            bytesWritten += FormatAppend(formater,"%:%:%\n\n",
                                         item.getPath(),
                                         srcInfo.begin.line,
                                         srcInfo.begin.column);

            // print line info
            stream.seek(0);
            for(size_t i= 1; i < srcInfo.begin.line;++i) {
                stream.skipLine();
            }
            const std::string line = stream.readLine();
            bytesWritten += FormatAppend(formater,"%\n", line.c_str());

            if (srcInfo.begin.line != srcInfo.end.line) {
                bytesWritten +=  FormatAppend(formater,
                                              m_useColorCodes
                                              ? YAL_CLRCD_BEGIN YAL_CLRCD_BOLD YAL_CLRCD_END "%%" YAL_CLRCD_RESET
                                              : "%%",
                                              FormatIdent<const char*>(srcInfo.begin.column-1,' ',"^"),
                                              FormatIdent<const char*>(line.size() - srcInfo.begin.column,'^',"\n"));
            } else {
                bytesWritten += FormatAppend(formater,
                                             m_useColorCodes
                                             ? YAL_CLRCD_BEGIN YAL_CLRCD_BOLD YAL_CLRCD_END "%%" YAL_CLRCD_RESET
                                             : "%%",
                                             FormatIdent<const char*>(srcInfo.begin.column-1,' ',"^"),
                                             FormatIdent<const char*>(srcInfo.end.column-srcInfo.begin.column,'^',"\n"));
            }
            FormatAppend(formater, "\n");
        }

        return bytesWritten;
    }
}
