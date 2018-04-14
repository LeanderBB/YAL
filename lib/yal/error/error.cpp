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

#include "yal/error/error.h"
#include "yal/util/formattypes.h"
#include "yal/util/format.h"

namespace yal {

    Error::Error(const uint32_t code):
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

    size_t
    FormatType(FormatTypeArgs &args,
               const Error *error){
        YAL_ASSERT(error != nullptr);
        return FormatType(args, *error);
    }

    size_t
    FormatType(FormatTypeArgs& args,
               const Error& error) {
        FormaterFromFormatArgs formater(args);
        error.printDetail(formater);
        return formater.bufferPos;
    }
}
