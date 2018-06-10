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

#pragma once

#include "yal/yal.h"
#include "yal/util/stringref.h"
#include "yal/util/cast.h"

namespace yal {

    struct Formater;
    struct FormatTypeArgs;
    struct SourceInfo;
    class ErrorPrinter;

    class ErrorCode {
    public:
        ErrorCode();
        explicit ErrorCode(const uint16_t category,
                           const uint16_t errorType);

        bool operator == (const ErrorCode& code) const;
        bool operator != (const ErrorCode& code) const;

    public:
        uint16_t m_category;
        uint16_t m_errorType;
    };

    inline ErrorCode MakeErrorCode(const uint16_t category,
                                   const uint16_t errorType) {
        return ErrorCode(category, errorType);
    }

    class Error {

    protected:
        Error(const ErrorCode code);
    public:
        virtual ~Error() = default;

        virtual StringRef getErrorName() const = 0;

        virtual void print(ErrorPrinter& printer) const = 0;

        bool isError() const {
            return !isWarning() && !isSuggestion();
        }

        bool isWarning() const {
            return m_isWarning == 1;
        }

        bool isSuggestion() const {
            return m_isSuggestion == 1;
        }

        bool isFatal() const {
            return m_isFatal == 1;
        }

        ErrorCode getCode() const {
            return m_code;
        }

    protected:

        void flagAsWarning();

        void flagAsSuggestion();

        void flagAsFatal();

    private:
        const ErrorCode m_code;
        unsigned m_isWarning:1;
        unsigned m_isSuggestion:1;
        unsigned m_isFatal: 1;
    };

}
