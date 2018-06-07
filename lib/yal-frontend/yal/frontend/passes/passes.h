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

namespace yal {
    class ErrorReporter;
    class SourceManager;
    class SourceItem;
}

namespace yal::frontend {


    enum class PassTypeCode : uint16_t {
        Parser = 3,
        Decl = 4,
        FnRet = 5,
        Type = 6,
        Move = 7,
    };


    class Module;

    struct PassOptions {
        PassOptions(ErrorReporter& errReporter_,
                    SourceManager& srcManager_,
                    Module& module_,
                    SourceItem& srcItem_):
            errReporter(errReporter_),
            srcManager(srcManager_),
            module(module_),
            srcItem(srcItem_){
        }

        ErrorReporter& errReporter;
        SourceManager& srcManager;
        Module& module;
        SourceItem& srcItem;
    };
};
