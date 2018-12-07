/*
 *  Copyright 2017 by Leander Beernaert (lbb-dev@pm.me)
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

#include "yal/frontend/ast/declfunction.h"
#include "yal/frontend/ast/declscope.h"

namespace yal::frontend {

    class DeclTypeFunction : public DeclFunction {
    public:

        DeclTypeFunction(Module& module,
                         TypeFunction* fnType,
                         DeclScope& scope,
                         const QualType& returnType,
                         const QualType& targetType,
                         Params&& params,
                         Body&& body);

        DeclTypeFunction(Module& module,
                         TypeFunction* fnType,
                         DeclScope& scope,
                         const QualType& returnType,
                         const QualType& targetType);

        const QualType& getTargetType() const {
            return m_targetType;
        }

        bool isStatic() const;

        bool isImmutable() const;

    private:
        QualType m_targetType;
    };
}
