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

#include "yal/frontend/passes/parser/passparser.h"

#include "yal/compiler/compiler.h"
#include "yal/frontend/lexer/lexer.h"
#include "yal/frontend/module.h"
#include "yal/frontend/parser/stparser.h"
#include "yal/frontend/passes/passes.h"
#include "yal/error/errorreporter.h"

namespace yal::frontend {

    PassParser::PassParser() {

    }

    bool
    PassParser::execute(PassOptions & options) {

        Lexer lexer(options.srcItem.getByteStream());
        STParser parser(lexer,
                        options.module.getSTContext(),
                        options.errReporter,
                        options.srcItem);
        return parser.parse();
    }

}
