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


#include <gtest/gtest.h>
#include <yal/util/stringref.h>
#include <iostream>

namespace testing {
    namespace internal {

        template <>
        inline void PrintTo<yal::StringRef>(const yal::StringRef& value, std::ostream *o) {
            const char quote = '\"';
            o->write(&quote, 1);
            o->write(value.data(), value.size());
            o->write(&quote, 1);
        }

    }
}
