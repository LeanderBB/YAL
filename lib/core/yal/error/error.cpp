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

#include "yal/error/error.h"
#include "yal/util/formattypes.h"
#include "yal/util/format.h"

namespace yal {

    ErrorCode::ErrorCode():
        m_category(0),
        m_errorType(0) {

    }

    ErrorCode::ErrorCode(const uint16_t category,
                         const uint16_t errorType):
        m_category(category),
        m_errorType(errorType) {

    }

    bool
    ErrorCode::operator == (const ErrorCode& code) const {
        return m_category == code.m_category
                && m_errorType == code.m_errorType;
    }
    bool
    ErrorCode::operator != (const ErrorCode& code) const {
        return m_category != code.m_category
                || m_errorType != code.m_errorType;
    }


    Error::Error(const ErrorCode code):
        m_code(code),
        m_isWarning(0),
        m_isSuggestion(0),
        m_isFatal(0){

    }

    void
    Error::flagAsWarning() {
        m_isWarning = 1;
    }

    void
    Error::flagAsSuggestion() {
        m_isSuggestion = 1;
    }

    void
    Error::flagAsFatal() {
        m_isFatal = 1;
    }
}
