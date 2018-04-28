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

#include "yal/frontend/types/qualtype.h"

namespace yal::frontend {

    QualType
    QualType::Create(const Qualifier& qualifier,
                     const Type* type) {
        QualType qt;
        qt.m_qualifier = qualifier;
        qt.m_type = type;
        return qt;
    }

    QualType
    QualType::Create() {
        QualType qt;
        qt.m_qualifier = Qualifier();
        qt.m_type = nullptr;
        return qt;
    }

    bool
    QualType::isTriviallyCopiable() const {
        YAL_ASSERT(m_type != nullptr);
        return m_type->isTriviallyCopiable();
    }

    bool
    QualType::isMovedType() const {
        return !isReference() && !isTriviallyCopiable();
    }
}
