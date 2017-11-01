/*
 *  Copyright 2017 by Leander Beernaert (leanderbb@gmail.com)
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
#include "yal/ast/typefunction.h"
#include "yal/ast/declfunction.h"
namespace yal{

    TypeFunction::TypeFunction(const Module* module,
                               const DeclFunction* functionDecl):
    Type(module, Kind::TypeFunction) {
        m_name = functionDecl->getFunctioName().toString();
        buildTypeNameWithModule();
    }

    const DeclFunction*
    TypeFunction::getDeclFunction() const {
        const DeclFunction* result =
                static_cast<const DeclFunction*>(m_declNode);
        return result;
    }

    QualType
    TypeFunction::getReturnType() const {
       return QualType();
    }

    bool
    TypeFunction::hasFunctionArguments() const {
        return getDeclFunction()->hasFunctionParameters();
    }

    bool
    TypeFunction::hasReturnValue() const {
       return getDeclFunction()->hasReturnValue();
    }

}
