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

#include "yal/error/error.h"
#include "yal/util/stringref.h"

#include <vector>

namespace yal {
    struct ProcessArgs;
}

namespace yal::backend::c
{
    class ErrorMkdir final : public Error {
    public:
        static const ErrorCode kCode;

        ErrorMkdir(const StringRef path);

        StringRef getErrorName() const final override;

        void print(ErrorPrinter& printer) const final override;

    public:
        const std::string m_path;
    };

    class ErrorOpenFile final : public Error {
    public:
        static const ErrorCode kCode;

        ErrorOpenFile(const StringRef path,
                      const uint32_t flags);

        StringRef getErrorName() const final override;

        void print(ErrorPrinter& printer) const final override;

    public:
        const std::string m_path;
        const uint32_t m_flags;
    };


    class ErrorCCompile final : public Error {
    public:
        static const ErrorCode kCode;

        ErrorCCompile(std::string&& msg);

        StringRef getErrorName() const final override;

        void print(ErrorPrinter& printer) const final override;

    public:
        const std::string m_msg;
    };

    class ErrorExecProcess final : public Error {
    public:
        static const ErrorCode kCode;

        ErrorExecProcess(const ProcessArgs& args);

        StringRef getErrorName() const final override;

        void print(ErrorPrinter& printer) const final override;

    private:
        const std::string m_command;
        std::vector<std::string> m_params;
    };
}
