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
#include "yal/backendc/errorsbackendc.h"
#include "yal/error/errorprinter.h"
#include "yal/io/bytestream.h"
#include "yal/io/sourcemanager.h"

namespace yal::backend::c {
    static const SourceInfo kNoInfo;

    const ErrorCode  ErrorMkdir::kCode =
            MakeErrorCode(0, 1);

    ErrorMkdir::ErrorMkdir(const StringRef path):
        Error(kCode),
        m_path(path.toString()) {
        flagAsFatal();
    }

    StringRef
    ErrorMkdir::getErrorName() const {
        return "Make Directory";
    }

    void
    ErrorMkdir::printDetail(ErrorPrinter &printer) const {
        FormatAppend(printer.getFormater(),
                     "Could not create directory '%'\n",
                     m_path);
    };

    const SourceInfo&
    ErrorMkdir::getSourceInfo() const {
        return kNoInfo;
    }

    // ErrorOpenFile --------------------------------------------------------

    const ErrorCode  ErrorOpenFile::kCode =
            MakeErrorCode(0, 1);

    ErrorOpenFile::ErrorOpenFile(const StringRef path,
                                 const uint32_t flags):
        Error(kCode),
        m_path(path.toString()),
        m_flags(flags) {
        flagAsFatal();
    }

    StringRef
    ErrorOpenFile::getErrorName() const {
        return "Open File";
    }

    void
    ErrorOpenFile::printDetail(ErrorPrinter &printer) const {
        StringRef flagStr;
        if ((m_flags & ByteStream::kModeReadWrite) == ByteStream::kModeReadWrite) {
            flagStr = "read & write";
        } else if ((m_flags & ByteStream::kModeWrite) == ByteStream::kModeWrite) {
            flagStr = "write";
        } else if ((m_flags & ByteStream::kModeRead) == ByteStream::kModeRead) {
            flagStr = "read";
        } else {
            flagStr = "unknown";
        }
        FormatAppend(printer.getFormater(),
                     "Could not open file '%' for %\n",
                     m_path, flagStr);
    };

    const SourceInfo&
    ErrorOpenFile::getSourceInfo() const {
        return kNoInfo;
    }

    // ErrorCCompile --------------------------------------------------------

    const ErrorCode  ErrorCCompile::kCode =
            MakeErrorCode(10, 1);

    ErrorCCompile::ErrorCCompile(std::string&& msg):
        Error(kCode),
        m_msg(std::move(msg)) {
        flagAsFatal();
    }

    StringRef
    ErrorCCompile::getErrorName() const {
        return "C Compilation";
    }

    void
    ErrorCCompile::printDetail(ErrorPrinter &printer) const {
        FormatAppend(printer.getFormater(),
                     "%\n",
                     m_msg);
    };

    const SourceInfo&
    ErrorCCompile::getSourceInfo() const {
        return kNoInfo;
    }
}
