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

#include "yal/os/unix/bytestreamunixfd.h"

#include <unistd.h>

namespace yal {

    ByteStreamUnixFd::ByteStreamUnixFd():
        m_fd(-1),
        m_flags() {

    }

    ByteStreamUnixFd::~ByteStreamUnixFd() {
        close();
    }

    void
    ByteStreamUnixFd::setFd(const int fd,
                            const uint32_t flags) {
        close();
        m_fd = fd;
        m_flags = flags;
    }

    size_t
    ByteStreamUnixFd::read(void* buffer,
                           const size_t bytes) {
        if (!(m_flags & kModeRead))  {
            return 0;
        }

        ssize_t result = ::read(m_fd, buffer, bytes);
        if (result < 0) {
            return 0;
        } else {
            return static_cast<size_t>(result);
        }
    }

    size_t
    ByteStreamUnixFd::write(const void* buffer,
                            const size_t bytes) {
        if (!(m_flags & kModeWrite))  {
            return 0;
        }

        ssize_t result = ::write(m_fd, buffer, bytes);
        if (result < 0) {
            return 0;
        } else {
            return static_cast<size_t>(result);
        }
    }

    bool
    ByteStreamUnixFd::seek(const size_t) {
        return false;
    }

    size_t
    ByteStreamUnixFd::getPosition() const {
        return 0;
    }

    size_t
    ByteStreamUnixFd::getSizeBytes() const {
        return 0;
    }

    void
    ByteStreamUnixFd::skipLine() {

    }

    std::string
    ByteStreamUnixFd::readLine() {
        return "";
    }

    bool
    ByteStreamUnixFd::isSeekable() const {
        return false;
    }

    void
    ByteStreamUnixFd::close() {
        if (m_fd != -1) {
            ::close(m_fd);
        }
    }
}
