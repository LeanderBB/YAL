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
#include "yal/util/stringref.h"
#include <algorithm>
#include <cstring>
#include <cstdio>
#include <cinttypes>
namespace yal {

    struct FormatTypeArgs {
        const size_t length;
        char* ptr;
    };


    template <typename T>
    struct FormatTypeHelper {
        typedef const T& type;
    };

    template <typename T>
    struct FormatTypeHelper<const T>{
        typedef const T& type;
    };

    template <typename T>
    struct FormatTypeHelper<const T* const> {
        typedef const T* const type;
    };

    template <typename T>
    struct FormatTypeHelper<const T* const&> {
        typedef const T* const type;
    };

    template <typename T>
    struct FormatTypeHelper<const T&> {
        typedef const T& type;
    };

    template <typename T>
    struct FormatTypeHelper<T*> {
        typedef const T* const type;
    };

    template<typename T>
    struct FormatIdent {
        const size_t size;
        const char character;
        typename FormatTypeHelper<T>::type arg;

        FormatIdent(const size_t amount,
                    const char ch,
                    typename FormatTypeHelper<T>::type value):
            size(amount),
            character(ch),
            arg(value) {

        }
    };


    inline size_t FormatType(FormatTypeArgs& args,
                             const char* const value) {
        const size_t strLen = ::strlen(value);
        const size_t copySize = std::min(strLen, args.length);
        if (copySize > 0) {
            ::memcpy(args.ptr, value, copySize);
        }
        return copySize;
    }

    inline size_t FormatType(FormatTypeArgs& args,
                             const StringRef& value) {
        const size_t copySize = std::min(value.size(), args.length);
        if (copySize > 0) {
            ::memcpy(args.ptr, value.data(), copySize);
        }
        return copySize;
    }

    inline size_t FormatType(FormatTypeArgs& loc,
                             const std::string& value) {
        const size_t copySize = std::min(value.size(), loc.length);
        if (copySize > 0) {
            ::memcpy(loc.ptr, value.c_str(), copySize);
        }
        return copySize;
    }

    inline size_t FormatType(FormatTypeArgs& args,
                             const char& value) {
        const size_t copySize = std::min(size_t(1), args.length);
        if (copySize > 0) {
            ::memcpy(args.ptr,&value, copySize);
        }
        return copySize;
    }


    inline size_t FormatType(FormatTypeArgs& loc,
                             const uint8_t& value) {
        const int result = ::snprintf(loc.ptr, loc.length, "%" PRIu8, value);
        if (result < 0) {
            return 0;
        }
        const size_t sresult= size_t(result);
        return std::min(loc.length -1, sresult);
    }

    inline size_t FormatType(FormatTypeArgs& loc,
                             const int8_t& value) {
        const int result = ::snprintf(loc.ptr, loc.length, "%" PRId8, value);
        if (result < 0) {
            return 0;
        }
        const size_t sresult= size_t(result);
        return std::min(loc.length -1, sresult);
    }

    inline size_t FormatType(FormatTypeArgs& loc,
                             const uint16_t& value) {
        const int result = ::snprintf(loc.ptr, loc.length, "%" PRIu16, value);
        if (result < 0) {
            return 0;
        }
        const size_t sresult= size_t(result);
        return std::min(loc.length -1, sresult);
    }

    inline size_t FormatType(FormatTypeArgs& loc,
                             const int16_t& value) {
        const int result = ::snprintf(loc.ptr, loc.length, "%" PRId16, value);
        if (result < 0) {
            return 0;
        }
        const size_t sresult= size_t(result);
        return std::min(loc.length -1, sresult);
    }

    inline size_t FormatType(FormatTypeArgs& loc,
                             const uint32_t& value) {
        const int result = ::snprintf(loc.ptr, loc.length, "%" PRIu32, value);
        if (result < 0) {
            return 0;
        }
        const size_t sresult= size_t(result);
        return std::min(loc.length -1, sresult);
    }

    inline size_t FormatType(FormatTypeArgs& loc,
                             const int32_t& value) {
        const int result = ::snprintf(loc.ptr, loc.length, "%" PRId32, value);
        if (result < 0) {
            return 0;
        }
        const size_t sresult= size_t(result);
        return std::min(loc.length -1, sresult);
    }

    inline size_t FormatType(FormatTypeArgs& loc,
                             const uint64_t& value) {
        const int result = ::snprintf(loc.ptr, loc.length, "%" PRIu64, value);
        if (result < 0) {
            return 0;
        }
        const size_t sresult= size_t(result);
        return std::min(loc.length -1, sresult);
    }

    inline size_t FormatType(FormatTypeArgs& loc,
                             const int64_t& value) {
        const int result = ::snprintf(loc.ptr, loc.length, "%" PRId64, value);
        if (result < 0) {
            return 0;
        }
        const size_t sresult= size_t(result);
        return std::min(loc.length -1, sresult);
    }

    inline size_t FormatType(FormatTypeArgs& loc,
                             const float& value) {
        const int result = ::snprintf(loc.ptr, loc.length, "%f", value);
        if (result < 0) {
            return 0;
        }
        const size_t sresult= size_t(result);
        return std::min(loc.length -1, sresult);
    }

    inline size_t FormatType(FormatTypeArgs& loc,
                             const double& value) {
        const int result = ::snprintf(loc.ptr, loc.length, "%lf", value);
        if (result < 0) {
            return 0;
        }
        const size_t sresult= size_t(result);
        return std::min(loc.length -1, sresult);
    }

    inline size_t FormatType(FormatTypeArgs& loc,
                             const bool& value) {
        return FormatType(loc, (value ? "true" :"false"));
    }

    template<typename T>
    inline size_t FormatType(FormatTypeArgs& args,
                             const FormatIdent<T>& value) {
        const size_t copySize = std::min(value.size, args.length);
        ::memset(args.ptr, value.character, copySize);

        if (args.length > copySize) {
            FormatTypeArgs nextArgs{args.length - copySize,
                        args.ptr + copySize};
            return copySize + FormatType(nextArgs,
                                         value.arg);
        } else {
            return copySize;
        }
    }
}
