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
#pragma once
#include <algorithm>
namespace yal
{

    template <typename A>
    struct cast_typeid;

    template <typename A>
    constexpr typename cast_typeid<A>::type get_typeid();

    template <typename A>
    typename cast_typeid<A>::type get_typeid_instance(const A& type);


    template <typename A, typename B,
              typename std::enable_if<std::is_same<A,B>::type>::type>
    inline const A* dyn_cast(const B* other) {
        return other;
    }

    template <typename A, typename B>
    inline const A* dyn_cast(const B* other) {
        static_assert(std::is_same<typename cast_typeid<A>::type,typename cast_typeid<B>::type>::value,
                      "dyn_cast only works for classes with same cast type");

        if (yal::get_typeid<A>() == yal::get_typeid_instance<B>(*other)) {
            return static_cast<const A*>(other);
        }
        return nullptr;
    }

    template <typename A, typename B>
    inline A* dyn_cast(B* other) {
        const B* cother = other;
        A* result = const_cast<A*>(dyn_cast<A>(cother));
        return result;
    }


}
