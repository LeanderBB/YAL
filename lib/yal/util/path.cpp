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

#include "yal/util/path.h"
#include "yal/yal.h"
#include <sys/stat.h>
#include <array>
#if defined (YAL_OS_UNIX)
#include <errno.h>
#endif

#if defined(YAL_OS_LINUX)
#include <linux/limits.h>
#endif

namespace yal {

#if defined(YAL_OS_UNIX)
    using OSPathCharType = char;
    static const char kPathSep = '/';

    static constexpr size_t GetMaxPathSize() {
        return PATH_MAX;
    }

#elif defined(YAL_OS_WIN32)
    using OSPathCharType = wchar_t;
    static const char kPathSep = '\\';

    static constexpr size_t GetMaxPathSize() {
        return MAX_PATH;
    }

#else
#erorr "Unsupport OS paltform"
#endif

    static const char kExtSep = '.';
    using OSPathBuffer = std::array<OSPathCharType, GetMaxPathSize()>;

    static bool ConvertToOSPath(OSPathBuffer& osPath,
                                const StringRef& path){
#if defined(YAL_OS_UNIX)
        path.toCStr(osPath.data(), GetMaxPathSize());
        return true;
#else
#erorr "Unsupport OS paltform"
#endif
    }

    std::string
    Path::Join(const StringRef p1,
               const StringRef p2) {
        YAL_ASSERT(p1.data() != nullptr && p2.data() != nullptr);
        std::string result;
        result.reserve(p1.size() + p2.size() + 1);
        result.append(p1.data(), p1.size());
        if (p1[p1.size() -1 ] != kPathSep) {
            result += kPathSep;
        }
        result.append(p2.data(), p2.size());
        return result;
    }

    StringRef
    Path::GetExtension(const StringRef path) {
        YAL_ASSERT(path.data() != nullptr);
        const size_t pos = path.findLastOf(kExtSep);
        if (pos != StringRef::npos) {
            return path.subStr(pos + 1);
        }
        return StringRef();
    }

    StringRef
    Path::RemoveExtension(const StringRef path) {
        YAL_ASSERT(path.data() != nullptr);
        const size_t pos = path.findLastOf(kExtSep);
        if (pos != StringRef::npos) {
            return path.subStr(0, pos);
        }
        return path;
    }

    StringRef
    Path::GetBasename(const StringRef path) {
        YAL_ASSERT(path.data() != nullptr);
        const size_t pos = path.findLastOf(kPathSep);
        if (pos != StringRef::npos) {
            return path.subStr(pos + 1);
        }
        return path;
    }


#if defined(YAL_OS_UNIX)
    static inline int yal_mkdir(const char* path) {
        return ::mkdir(path, (S_IWUSR | S_IRUSR | S_IXUSR));
    }
#elif defined (YAL_OS_WIN32)
    static inline int yal_mkdir(const char* path) {
        return _mkdir(path);
    }
#else
#error Missing implementation of mkdir
#endif

    static bool MakeDirectoryRecursive(OSPathCharType *path,
                                       OSPathCharType* offset) {
        errno = 0;
        OSPathCharType* char_loc = strchr(offset, kPathSep);
        if (char_loc) {
            *char_loc = '\0';
            const int errCode = yal_mkdir(path);
            *char_loc = kPathSep;
            if ( errCode == -1) {
                if (errno != EEXIST) {
                    return false;
                }
            }
            *char_loc= kPathSep;
            return MakeDirectoryRecursive(path, char_loc + 1);
        } else {
            if (yal_mkdir(path) == -1) {
                if (errno != EEXIST) {
                    return false;
                }
            }
            return true;
        }
    }

    bool
    Path::MakeDirectory(const StringRef path) {
        YAL_ASSERT(path.data() != nullptr);
        OSPathBuffer osPath;
        if (!ConvertToOSPath(osPath, path)) {
            return false;
        }
        return MakeDirectoryRecursive(osPath.data(), osPath.data());
    }

    bool
    Path::Exists(const StringRef path) {
        YAL_ASSERT(path.data() != nullptr);
        OSPathBuffer osPath;
        if (!ConvertToOSPath(osPath, path)) {
            return false;
        }
        struct stat file_info;
        return (stat(osPath.data(), &file_info) == 0);
    }

    bool
    Path::IsFile(const StringRef path) {
        YAL_ASSERT(path.data() != nullptr);
        OSPathBuffer osPath;
        if (!ConvertToOSPath(osPath, path)) {
            return false;
        }

        bool ret = false;
        struct stat file_info;
        ret = (stat(osPath.data(), &file_info) == 0);
        if (ret) {
            ret = S_ISREG(file_info.st_mode);
        }
        return ret;
    }

    bool
    Path::IsDirectory(const StringRef path) {
        YAL_ASSERT(path.data() != nullptr);
        OSPathBuffer osPath;
        if (!ConvertToOSPath(osPath, path)) {
            return false;
        }
        bool ret = false;
        struct stat file_info;
        ret = (stat(osPath.data(), &file_info) == 0);
        if (ret) {
            ret = S_ISDIR(file_info.st_mode);
        }
        return ret;
    }

    bool
    Path::IsSymlink(const StringRef path) {
        YAL_ASSERT(path.data() != nullptr);
        OSPathBuffer osPath;
        if (!ConvertToOSPath(osPath, path)) {
            return false;
        }
        bool ret = false;
        struct stat file_info;
        ret = (stat(osPath.data(), &file_info) == 0);
        if (ret) {
            ret = S_ISLNK(file_info.st_mode);
        }
        return ret;
    }

    bool
    Path::Remove(const StringRef path) {
        YAL_ASSERT(path.data() != nullptr);
        OSPathBuffer osPath;
        if (!ConvertToOSPath(osPath, path)) {
            return false;
        }
        return remove(osPath.data()) == 0;
    }

    bool
    Path::Rename(const StringRef oldPath,
                 const StringRef newPath) {
        YAL_ASSERT(oldPath.data() != nullptr && newPath != nullptr);
        OSPathBuffer osPathOld;
        if (!ConvertToOSPath(osPathOld, oldPath)) {
            return false;
        }
        OSPathBuffer osPathNew;
        if (!ConvertToOSPath(osPathNew, newPath)) {
            return false;
        }
        return rename(osPathOld.data(), osPathNew.data()) == 0;
    }
}
