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

#include "yal/frontend/types/qualifier.h"

namespace yal::frontend {
    Qualifier:: Qualifier():
        m_mutable(0),
        m_reference(0),
        m_pointer(0),
        m_reqReplace(0),
        m_lvalue(0){
    }

    void
    Qualifier::setMutable() {
        m_mutable = 1;
    }

    void
    Qualifier::setImmutable() {
        m_mutable = 0;
    }

    void
    Qualifier::setReference() {
        m_reference = 1;
        m_pointer = 0;
        m_lvalue = 1;
    }

    void
    Qualifier::setPointer() {
        m_pointer = 1;
        m_reference = 0;
    }

    void
    Qualifier::setRequiresReplace(const bool v) {
        m_reqReplace = v == true ? 1 : 0;
    }

    void
    Qualifier::setLValue(const bool v) {
        m_lvalue = v == true ? 1 : 0;
    }

    bool
    Qualifier::isMutable() const {
        return m_mutable == 1;
    }

    bool
    Qualifier::isImmutable() const {
        return m_mutable == 0;
    }

    bool
    Qualifier::isReference() const {
        return m_reference == 1;
    }

    bool
    Qualifier::isPointer() const {
        return m_pointer == 1;
    }

    bool
    Qualifier::requiresReplace() const {
        return m_reqReplace == 1;
    }

    bool
    Qualifier::isLValue() const {
        return m_lvalue == 1;
    }

    bool
    Qualifier::isRValue() const {
        return m_lvalue == 0;
    }
}
