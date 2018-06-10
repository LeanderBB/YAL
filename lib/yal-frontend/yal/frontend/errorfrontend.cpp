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

#include "yal/frontend/errorfrontend.h"
#include "yal/error/errorprinter.h"
#include "yal/io/sourcemanager.h"

namespace yal::frontend {

    ErrorFrontend::ErrorFrontend(const ErrorCode code):
        Error(code) {

    }

    void
    ErrorFrontend::print(ErrorPrinter& printer) const {
        Formater& formater = printer.getFormater();
        SourceManager& manager = printer.getSourceManager();
        const SourceInfo& srcInfo = getSourceInfo();
        SourceItemOpt item =  manager.getItem(srcInfo.handle);
        if (item.has_value()) {
            printer.printSourceInfo(formater, *item.value(), srcInfo);
        }

        FormatAppend(formater, "> ");
        printDetail(printer);
    }

}
