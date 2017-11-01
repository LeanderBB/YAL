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

namespace yal {

    StringRef::StringRef(const char* str):
        m_str(str),
        m_size(::strlen(str)) {
        YAL_ASSERT(str != nullptr && m_size != 0);
    }

    StringRef::StringRef(const char* str,
                         const size_t len):
        m_str(str),
        m_size(len){
        YAL_ASSERT(str != nullptr && len != 0);
    }

    StringRef::StringRef(const std::string& str) {
        YAL_ASSERT(!str.empty());
        m_str = str.c_str();
        m_size = str.size();
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
}
