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

#include "yal/frontend/passes/decl/errorspassdecl.h"

#include "yal/error/errorreporter.h"
#include "yal/error/errorprinter.h"
#include "yal/frontend/module.h"
#include "yal/frontend/parser/stdeclfunction.h"
#include "yal/frontend/parser/stdeclstruct.h"
#include "yal/frontend/passes/passes.h"
#include "yal/frontend/types/type.h"
#include "yal/frontend/types/typefunction.h"
#include "yal/frontend/types/typestruct.h"
#include "yal/util/format.h"

namespace yal::frontend {

    const ErrorCode  ErrorDuplicateDecl::kCode =
            MakeErrorCode(static_cast<uint16_t>(PassTypeCode::Decl), 1);

    ErrorDuplicateDecl::ErrorDuplicateDecl(const Module& module,
                                           const Type& newType,
                                           const Type& oldType):
        Error(kCode),
        m_module(module),
        m_newType(newType),
        m_oldType(oldType) {

    }

    StringRef
    ErrorDuplicateDecl::getErrorName() const {
        return "Duplicate Type Declaration";
    }

    void
    ErrorDuplicateDecl::printDetail(ErrorPrinter &printer) const {
        FormatAppend(printer.getFormater(),
                     "Can not re-declare '%', type previously declared here:\n",
                     m_newType.getIdentifier());
        const SourceInfo& oldSrcInfo = getSourceInfoFromType(m_oldType);

        auto srcHandle = m_module.getSourceHandle();
        SourceItem* item = printer.getSourceManager().getItem(srcHandle);
        printer.printSourceInfo(printer.getFormater(),* item, oldSrcInfo);
    };

    const SourceInfo&
    ErrorDuplicateDecl::getSourceInfo() const {
        return getSourceInfoFromType(m_newType);
    }

    const SourceInfo&
    ErrorDuplicateDecl::getSourceInfoFromType(const Type& type) const {
        static const SourceInfo kNoSourceInfo;
        const TypeFunction* typeFunction = dyn_cast<TypeFunction>(&type);
        if (typeFunction != nullptr) {
            return typeFunction->getSTDecl()->getSourceInfo();
        }

        const TypeStruct* typeStruct = dyn_cast<TypeStruct>(&type);
        if (typeStruct != nullptr) {
            return typeStruct->getSTDecl()->getSourceInfo();
        }

        return kNoSourceInfo;
    }

}
