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

#pragma once

#include "yal/yal.h"
#include "yal/util/allocatorstack.h"

namespace yal::frontend {

    class STDeclModule;

    class STContext {
    public:

        STContext();

        YAL_NO_COPY_CLASS(STContext);

        template <typename T, typename... ARGS>
        T* create(ARGS&& ...args) {
            return m_allocator.construct<T>(std::forward<ARGS>(args)...);
        }

        STDeclModule* getDeclModule() const {
            return m_declModule;
        }

    private:
        AllocatorStack m_allocator;
        STDeclModule* m_declModule;
    };

}
