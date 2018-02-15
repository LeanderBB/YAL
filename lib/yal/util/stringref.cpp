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
#include "yal/yal.h"
#include "yal/util/stringref.h"
#include <cstring>
#include <limits>
#include <algorithm>
namespace yal {

    const size_t StringRef::npos = std::numeric_limits<size_t>::max();

    StringRef::StringRef(const char* str):
        m_str(str),
        m_size(str != nullptr ? ::strlen(str) : 0) {

    }

    StringRef::StringRef(const char* str,
                         const size_t len):
        m_str(str),
        m_size(len){
    }

    StringRef::StringRef(const std::string& str) {
        m_str = str.c_str();
        m_size = str.size();
        if (m_size == 0) {
            m_str = nullptr;
        }
    }

    StringRef::StringRef(const StringRefPod& pod):
        m_str(pod.str),
        m_size(pod.size) {

    }

    StringRef& StringRef::operator=(const StringRefPod& pod) {
        m_str = pod.str;
        m_size = pod.size;
        return *this;
    }

    StringRef::StringRef(StringRef&& other):
        m_str(other.m_str),
        m_size(other.m_size) {
        other.m_size = 0;
        other.m_str = nullptr;
    }

    StringRef&
    StringRef::operator=(StringRef&& other) {
        if (this != &other) {
            std::swap(m_str,other.m_str);
            std::swap(m_size,other.m_size);
        }
        return *this;
    }

    size_t
    StringRef::length() const {
        return m_size;
    }

    size_t
    StringRef::size() const {
        return m_size;
    }

    bool
    StringRef::operator == (const StringRef& other) const
    {
        if (other.m_str == nullptr && m_str == nullptr){
            return true;
        }

        if (other.m_str == nullptr || m_str == nullptr){
            return false;
        }

        if (other.m_size != m_size) {
            return false;
        }

        return ::strncmp(m_str, other.m_str,m_size) == 0;
    }

    bool
    StringRef::operator != (const StringRef& other) const {
        return !(*this == other);
    }

    bool
    StringRef::operator == (const std::string& other) const{
        if (other.empty() && m_str == nullptr){
            return true;
        }

        if (other.empty() || m_str == nullptr){
            return false;
        }

        if (other.size() != m_size) {
            return false;
        }

        return ::strncmp(m_str, other.c_str(),m_size) == 0;
    }

    bool
    StringRef::operator != (const std::string& other) const {
        return !(*this == other);
    }

    bool
    StringRef::operator == (const char* other) const {
        if (other == nullptr && m_str == nullptr){
            return true;
        }

        if (other == nullptr || m_str == nullptr){
            return false;
        }

        const size_t otherSize = strlen(other);
        if (otherSize != m_size) {
            return false;
        }

        return ::strncmp(m_str, other,m_size) == 0;
    }

    bool
    StringRef::operator != (const char* other) const {
        return !(*this == other);
    }

    char
    StringRef::operator[](size_t i) const {
        YAL_ASSERT(i < m_size);
        return m_str[i];
    }

    std::string
    StringRef::toString() const {
        return std::string(m_str, m_size);
    }

    StringRefPod
    StringRef::toStringRefPod() const {
        StringRefPod result;
        result.str = m_str;
        result.size = m_size;
        return result;
    }

    void
    StringRef::toCStr(char* dst,
                      const size_t size) const {
        const size_t copySize = std::min(size, size - 1);
        memcpy(dst, m_str, copySize);
        dst[copySize] = '\0';
    }

    size_t
    StringRef::findFirstOf(const size_t start,
                           const char ch) const {
        if (m_size == 0 || start >= m_size) {
            return npos;
        }

        const void* result = memchr(m_str + start, ch, m_size);
        if (result == nullptr) {
            return npos;
        }

        const std::ptrdiff_t resultPtr = reinterpret_cast<std::ptrdiff_t>(result);
        const std::ptrdiff_t dataPtr = reinterpret_cast<std::ptrdiff_t>(m_str);
        return resultPtr - dataPtr;
    }

#if defined(YAL_OS_WIN32)
    const void* yal_memrchr(const void* ptr,
                            const int intChar,
                            const size_t size) {
        //TODO: Optimize?
        const char ch = static_cast<char>(intChar);
        const char* chPtr = reinterpret_cast<const char*>(ptr);

        size_t curIndex = 0;
        for (size_t i = 1; i <= size; ++i) {
            curIndex = size - i;
            if (chPtr[curIndex] == ch) {
                return &chPtr[curIndex];
            }
        }
        return nullptr;
    }
#elif defined(YAL_OS_UNIX)
#define yal_memrchr memrchr
#else 
#error "Unknown OS"
#endif


    size_t
    StringRef::findLastOf(const size_t start,
                          const char ch) const {
        if (m_size == 0 || start >= m_size) {
            return npos;
        }

        const void* result = yal_memrchr(m_str + start, ch, m_size - start);
        if (result == nullptr) {
            return npos;
        }

        const std::ptrdiff_t resultPtr = reinterpret_cast<std::ptrdiff_t>(result);
        const std::ptrdiff_t dataPtr = reinterpret_cast<std::ptrdiff_t>(m_str);
        return resultPtr - dataPtr;
    }

    StringRef
    StringRef::subStr(const size_t start,
                      const size_t len) const {
        const size_t verifiedStart = std::min(start, m_size);
        size_t verifiedLen = len;
        if (verifiedStart + len > m_size) {
            verifiedLen = m_size - verifiedStart;
        }
        return StringRef(m_str + verifiedStart, verifiedLen);
    }

    StringRef
    StringRef::subStr(const size_t start) const {
        const size_t verifiedStart = std::min(start, m_size);
        const  size_t verifiedLen = m_size - verifiedStart;
        return StringRef(m_str + verifiedStart, verifiedLen);
    }

    std::string
    StringRef::replace(const StringRef pattern,
                        const StringRef with) const {
        //TODO: Boyer-Moore string search!
        if (pattern.size() > m_size
                || pattern.data() == nullptr
                || with.data() == nullptr) {
            return toString();
        }

        std::string result;
        result.reserve(m_size);

        size_t offset = 0;
        size_t lastFoundOffset = 0;

        while(offset + pattern.size() <= m_size) {
            if (StringRef(m_str + offset, pattern.size()) == pattern){
                result.append(m_str + lastFoundOffset, offset-lastFoundOffset);
                result.append(with.data(), with.size());
                offset += pattern.size();
                lastFoundOffset = offset;
            } else {
                offset += 1;
            }
        }

        if (offset-lastFoundOffset != 0) {
            result.append(m_str + lastFoundOffset, m_size-lastFoundOffset);
        }
        return result;
    }
}
