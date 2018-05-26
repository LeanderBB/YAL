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


#include "yal/util/stringref.h"
#include <string>
namespace yal {

    class Path {
    public:
        static std::string Join(const StringRef p1,
                                const StringRef p2);

        static StringRef GetExtension(const StringRef path);

        static StringRef RemoveExtension(const StringRef path);

        static StringRef GetBasename(const StringRef path);

        static bool MakeDirectory(const StringRef path);

        static bool Exists(const StringRef path);

        static bool IsFile(const StringRef path);

        static bool IsDirectory(const StringRef path);

        static bool IsSymlink(const StringRef path);

        static bool Remove(const StringRef path);

        static bool Rename(const StringRef oldPath,
                           const StringRef newPath);

    };
}
