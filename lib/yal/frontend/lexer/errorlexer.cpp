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

#include "yal/frontend/lexer/errorlexer.h"
#include "yal/frontend/lexer/tokens.h"
#include "yal/util/format.h"

namespace yal {

    static const uint16_t kErrorCategoryCode = 1;
    static const uint16_t kErrorTypeCode = 1;

    const ErrorCode ErrorLexer::kCode
        = MakeErrorCode(kErrorCategoryCode, kErrorTypeCode);

    ErrorLexer::ErrorLexer(const TokenInfo& tokenInfo,
                           const SourceManager::Handle srcHandle):
        Error(ErrorLexer::kCode),
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
    ErrorLexer::getErrorName() const {
        return "Lexer Error";
    }

    void
    ErrorLexer::printDetail(Formater& formater) const {
        Format(formater, "Unknown token\n");
    }

    const SourceInfo&
    ErrorLexer::getSourceInfo() const {
        return m_srcInfo;
    }
}
