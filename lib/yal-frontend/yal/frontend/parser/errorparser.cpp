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

#include "yal/frontend/parser/errorparser.h"
#include "yal/error/errorprinter.h"
#include "yal/frontend/lexer/tokens.h"
#include "yal/util/format.h"

namespace yal::frontend {

    static const uint16_t kErrorCategoryCode = 2;
    static const uint16_t kErrorTypeCode = 1;

    const ErrorCode ErrorParser::kCode
    = MakeErrorCode(kErrorCategoryCode, kErrorTypeCode);

    ErrorParser::ErrorParser(const TokenInfo& tokenInfo,
                             const SourceManager::Handle srcHandle):
        ErrorFrontend (ErrorParser::kCode),
        m_srcInfo() {
        flagAsFatal();
        m_srcInfo.begin.line = tokenInfo.lineStart;
        m_srcInfo.begin.column = tokenInfo.columnStart;
        m_srcInfo.begin.streamOffset = tokenInfo.tokenOffsetInStream;
        m_srcInfo.end.line = tokenInfo.lineEnd;
        m_srcInfo.end.column = tokenInfo.columnEnd;
        m_srcInfo.end.streamOffset = tokenInfo.tokenOffsetInStream
                + tokenInfo.tokenStr.size;
        m_srcInfo.handle = srcHandle;
    }


    StringRef
    ErrorParser::getErrorName() const {
        return "Syntax Error";
    }

    void
    ErrorParser::printDetail(ErrorPrinter& printer) const {
        FormatAppend(printer.getFormater(), "Unexpected token\n");
    }

    const SourceInfo&
    ErrorParser::getSourceInfo() const {
        return m_srcInfo;
    }

}
