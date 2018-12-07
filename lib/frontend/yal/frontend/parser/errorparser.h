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

#include "yal/frontend/errorfrontend.h"
#include "yal/io/sourcemanager.h"

namespace yal {
    class SourceItem;
    class ErrorPinter;
}

namespace yal::frontend {

    struct TokenInfo;

    class ErrorParser final : public ErrorFrontend {
    public:

        static const ErrorCode kCode;

        ErrorParser(const TokenInfo& tokenInfo,
                   const SourceManager::Handle srcHandle);

        StringRef getErrorName() const final override;

    protected:
        void printDetail(ErrorPrinter& formater) const final override;

        const SourceInfo& getSourceInfo() const final override;

    private:
        SourceInfo m_srcInfo;
    };

    class ErrorParserStreamOpen final : public Error {
    public:
        static const ErrorCode kCode;

        ErrorParserStreamOpen(const SourceItem& item);

        StringRef getErrorName() const final override;

        void print(ErrorPrinter& printer) const override final;
    private:
        const SourceItem& m_item;
    };

}
