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

#pragma once

#if defined(YAL_OS_UNIX)
#include "yal/os/unix/processunix.h"
#else
#error "No implementation of process for current platform"
#endif

#include "yal/io/bytestream.h"
#include "yal/util/stringref.h"

#include <memory>
#include <optional>
#include <vector>

namespace yal {

    enum class ProcessStatus {
        Ok,
        FailedArgumentCount,
        FailedInputRedirect,
        FailedOutputRedirect,
        FailedInputSupply,
        FailedLaunch
    };

    struct ProcessArgs {
        StringRef binPath;
        std::vector<StringRef> args;
        ByteStream* input = nullptr;
        bool captureOutput = false;
    };

    struct ProcessResult {
        ProcessStatus status;
        std::unique_ptr<ByteStream> output;
    };

    class Process : public ProcessBase
    {
    public:
        Process() = default;

        YAL_NO_COPY_CLASS(Process);

        ProcessResult run(ProcessArgs& args);

        int join() {
            return ProcessBase::joinImpl();
        }
    };

}
