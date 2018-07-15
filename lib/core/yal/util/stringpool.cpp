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
#include "yal/util/stringpool.h"

namespace yal {

    StringRef
    StringPool::getOrCreate(const StringRef string) {
        std::string& value = m_map[string];
        if (value.empty()) {
            value = string.toString();
        }
        return StringRef(value);
    }

    StringRef
    StringPool::getOrCreate(std::string&& string) {
        std::string& value = m_map[string];
        if (value.empty()) {
            value = std::move(string);
        }
        return StringRef(value);
    }

    std::optional<StringRef>
    StringPool::get(const StringRef string) const {
        auto it = m_map.find(string);
        if (it == m_map.end()) {
            return std::optional<StringRef>();
        }
        return StringRef(it->second);
    }

}
