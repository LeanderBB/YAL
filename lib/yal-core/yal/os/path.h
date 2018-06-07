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
#include <optional>

#if defined(YAL_OS_LINUX)
#include <linux/limits.h>
#endif

#if defined(YAL_OS_WIN32)
#include <Windows.h>
#endif

namespace yal {

#if defined(YAL_OS_UNIX)
    using OSPathCharType = char;
    static constexpr size_t GetMaxPathSize() {
        return PATH_MAX;
    }

#elif defined(YAL_OS_WIN32)
    using OSPathCharType = wchar_t;
    static constexpr size_t GetMaxPathSize() {
        return MAX_PATH;
    }

#else
#error "Unsupport OS paltform"
#endif

    using OSPathBuffer = std::array<OSPathCharType, GetMaxPathSize()>;

    class Path {
    public:

        static const char kPathSep;
        static const StringRef kPathSepStr;

        static bool ToOsPath(OSPathBuffer& osPath,
                             const StringRef& path);

        template<typename ...Args>
        static std::string JoinV(const Args&...args) {
            std::string output;
            const size_t size = JoinVGetSize(args...);
            output.reserve(size);
            JoinVImpl(output, args...);
            return output;
        }

        static std::string Join(const StringRef p1,
                                const StringRef p2);

        static void Join(std::string& output,
                         const StringRef p1,
                         const StringRef p2);

        static void Join(std::string& output,
                         const StringRef p1);

        static StringRef GetExtension(const StringRef path);

        static StringRef RemoveExtension(const StringRef path);

        static StringRef GetBasename(const StringRef path);

        static StringRef GetPath(const StringRef path);

        static std::optional<std::string> GetRealPath(const StringRef path);

        static bool MakeDirectory(const StringRef path);

        static bool Exists(const StringRef path);

        static bool IsFile(const StringRef path);

        static bool IsDirectory(const StringRef path);

        static bool IsSymlink(const StringRef path);

        static bool Remove(const StringRef path);

        static bool Rename(const StringRef oldPath,
                           const StringRef newPath);

    private:

        template <typename T>
        static size_t JoinVGetSize(const T& str) {
            static_assert(std::is_same<T, yal::StringRef>::value && "Can't use on non-string types!");
            return str.size();
        }

        template <typename T,typename ...Args>
        static size_t JoinVGetSize(const T& str, const Args& ...args) {
            static_assert(std::is_same<T, yal::StringRef>::value && "Can't use on non-string types!");
            return str.size() + 1 + JoinVGetSize(args...);
        }

        template<typename T, typename ...Args>
        static void JoinVImpl(std::string& output, const T& str) {
            static_assert(std::is_same<T, yal::StringRef>::value && "Can't use on non-string types!");
            Join(output, str);
        }

        template<typename T, typename ...Args>
        static void JoinVImpl(std::string& output, const T& str, const Args& ...args) {
            static_assert(std::is_same<T, yal::StringRef>::value && "Can't use on non-string types!");
            Join(output, str);
            JoinVImpl(output, args...);
        }

    };
}
