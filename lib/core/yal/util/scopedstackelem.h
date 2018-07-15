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

#include "yal/yal.h"

namespace yal {

    template<typename STACK>
    class ScopedStackElem {
    public:
        static_assert(std::is_pointer_v<typename STACK::value_type>,
                      "Can only be used with pointer types");

        ScopedStackElem(STACK& stack,
                        typename STACK::value_type elem):
            m_stack(stack) {

#if defined(YAL_DEBUG) || defined(YAL_RELEASE_DEBUG)
            m_elemCopy = elem;
#endif
            m_stack.push(elem);
        }

        ~ScopedStackElem() {
            YAL_ASSERT(!m_stack.empty());
#if defined(YAL_DEBUG) || defined(YAL_RELEASE_DEBUG)
            YAL_ASSERT(m_stack.top() == m_elemCopy);
#endif
            m_stack.pop();
        }
    private:
       STACK& m_stack;
#if defined(YAL_DEBUG) || defined(YAL_RELEASE_DEBUG)
       typename STACK::value_type m_elemCopy = nullptr;
#endif
    };

}
