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

#if defined(YAL_OS_WIN32)
#include <Windows.h>
#include <direct.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <wchar.h>
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
#error "Unsupport OS paltform"
#endif

    static const char kExtSep = '.';

    using OSPathBuffer = std::array<OSPathCharType, GetMaxPathSize()>;

    static bool ConvertToOSPath(OSPathBuffer& osPath,
                                const StringRef& path){
#if defined(YAL_OS_UNIX)
        path.toCStr(osPath.data(), GetMaxPathSize());
        return true;
#elif defined(YAL_OS_WIN32)
        const int nLenWide = MultiByteToWideChar(CP_UTF8, 
                                                 0, 
                                                 path.data(),
                                                 path.length(), 
                                                 NULL, 
                                                 0);

        if (nLenWide == 0 || static_cast<size_t>(nLenWide) >= osPath.size()) {
            return false;
        }

        const int convResult = MultiByteToWideChar(CP_UTF8,
                                                   0,
                                                   path.data(),
                                                   path.length(),
                                                   osPath.data(),
                                                   osPath.size());
        return convResult != 0;
#else
#error "Unsupport OS paltform"
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
    static inline int yal_mkdir(const OSPathCharType* path) {
        return ::mkdir(path, (S_IWUSR | S_IRUSR | S_IXUSR));
    }

    static inline OSPathCharType* yal_strchr(OSPathCharType* str,
                                             const OSPathCharType ch) {
        return strchr(str, ch);
    }
#elif defined (YAL_OS_WIN32)
    static inline int yal_mkdir(const OSPathCharType* path) {
        return _wmkdir(path);
    }

    static inline OSPathCharType* yal_strchr(OSPathCharType* str,
                                             const OSPathCharType ch) {
        return wcschr(str, ch);
    }
#else
#error Missing implementation of mkdir
#endif

    static bool MakeDirectoryRecursive(OSPathCharType *path,
                                       OSPathCharType* offset) {
        errno = 0;
        OSPathCharType* char_loc = yal_strchr(offset, kPathSep);
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


#if defined(YAL_OS_UNIX)
    typedef struct stat yal_stat;
    #define yal_statfn stat
    #define yal_remove remove
    #define yal_rename rename
#elif defined(YAL_OS_WIN32)
    typedef struct _stat64 yal_stat;
    #define yal_statfn _wstat64
    #define S_ISREG(m) (m & _S_IFREG == _S_IFREG)
    #define S_ISDIR(m) (m & _S_IFDIR == _S_IFDIR)
    #define S_ISLNK(m) (false)
    #define yal_remove _wremove
    #define yal_rename _wrename
#else
#error "Unknown platform"
#endif

    bool
    Path::Exists(const StringRef path) {
        YAL_ASSERT(path.data() != nullptr);
        OSPathBuffer osPath;
        if (!ConvertToOSPath(osPath, path)) {
            return false;
        }
        yal_stat file_info;
        return (yal_statfn(osPath.data(), &file_info) == 0);
    }

    bool
    Path::IsFile(const StringRef path) {
        YAL_ASSERT(path.data() != nullptr);
        OSPathBuffer osPath;
        if (!ConvertToOSPath(osPath, path)) {
            return false;
        }

        bool ret = false;
        yal_stat file_info;
        ret = (yal_statfn(osPath.data(), &file_info) == 0);
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
        yal_stat file_info;
        ret = (yal_statfn(osPath.data(), &file_info) == 0);
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
        yal_stat file_info;
        ret = (yal_statfn(osPath.data(), &file_info) == 0);
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
        return yal_remove(osPath.data()) == 0;
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
        return yal_rename(osPathOld.data(), osPathNew.data()) == 0;
    }
}
