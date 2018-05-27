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

#include "yal/os/unix/processunix.h"

#include "yal/os/process.h"
#include "yal/os/unix/bytestreamunixfd.h"

#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>
#include <memory>

namespace yal {

    ProcessUnix::ProcessUnix():
        m_pid(-1)
    {

    }

    ProcessUnix::~ProcessUnix() {
        joinImpl();
    }

    void ProcessUnix::execute(ProcessResult &result,
                              ProcessArgs& args) {

        const bool hasOutput = args.captureOutput;
        const bool hasInput = args.input != nullptr;

        YAL_ASSERT_MESSAGE(512 <= MaxArgCount(),
                           "char buffer not enough to hold all argument counts for process exec");

        // build process arguments
        const char* processArgs[512];
        processArgs[0] = args.binPath.data();
        for (size_t i = 0; i < args.args.size(); ++i) {
            processArgs[i+1] = args.args[i].data();
        }
        processArgs[args.args.size()+1] = nullptr;

        int pipeOutput[2];
        int pipeInput[2];
        int controlChannel[2];

        if (pipe(controlChannel) < 0) {
            result.status = ProcessStatus::FailedOutputRedirect;
            return;
        }

        // setup parent/child communication
        if (hasOutput && pipe(pipeOutput) < 0) {
            result.status = ProcessStatus::FailedOutputRedirect;
            return;
        }

        if (hasInput && pipe(pipeInput) < 0) {
            result.status = ProcessStatus::FailedInputRedirect;
            return;
        }

        pid_t pidChild = fork();
        if (pidChild == -1) {
            // failed to create child
            result.status = ProcessStatus::FailedLaunch;
            return;
        }

        if (pidChild == 0) {
            // redirect input
            // close input if not available

            if (hasInput) {
                close(STDIN_FILENO);
                dup2(pipeInput[0], STDIN_FILENO);
                close(pipeInput[1]);
            } else {
                close(0);
            }

            if (hasOutput) {
                // redirect output
                dup2(pipeOutput[1], STDERR_FILENO);
                dup2(pipeOutput[1], STDOUT_FILENO);
                close(pipeOutput[0]);
            } else {
                close(STDERR_FILENO);
                close(STDOUT_FILENO);
            }

            // close pipe input
            close(controlChannel[0]);
            // execute process
            ::execv(args.binPath.data(),
                    const_cast<char**>(processArgs));

            // something went wrong, report back
            ::write(controlChannel[1], "err", 3);
            close(controlChannel[1]);
            exit(-1);
        } else {
            // parent,
            m_pid = pidChild;

            if (hasOutput) {
                // close child pipe
                close(pipeOutput[1]);
                // create output reader
                auto stream = std::make_unique<ByteStreamUnixFd>();
                stream->setFd(pipeOutput[0], ByteStream::kModeRead);
                result.output = std::move(stream);
            }

            // Read input if everything is fine
            if (hasInput) {
                // close unused pipe
                close(pipeInput[0]);

                // write to child
                ByteStream& stream = *args.input;
                enum { kReadBufferSize = 512};
                char buffer[kReadBufferSize];
                size_t bytesRead = 1;
                while (bytesRead != 0) {
                    bytesRead = stream.read(buffer,kReadBufferSize);
                    if (bytesRead != 0) {
                        const ssize_t bytesWritten = ::write(pipeInput[1], buffer, bytesRead);
                        if (bytesWritten < 0) {
                            close(pipeInput[1]);
                            result.status = ProcessStatus::FailedInputSupply;
                            return;
                        }
                    }
                }
                close(pipeInput[1]);
            }

            // check control
            close(controlChannel[1]);
            char controllBuffer[4];
            const size_t childControlRead = ::read(controlChannel[0],
                    controllBuffer,
                    4);
            close(controlChannel[0]);
            // if we return anything here we failed to actually start
            // the child process.
            if (childControlRead != 0) {
                result.status = ProcessStatus::FailedLaunch;
                return;
            }
        }
        result.status = ProcessStatus::Ok;
    }

    int
    ProcessUnix::joinImpl() {
        int resultChild = -1;
        if (m_pid != -1) {
            waitpid(m_pid, &resultChild, 0);
            m_pid = -1;
        }
        return resultChild;
    }

    size_t
    ProcessUnix::MaxArgCount() {
        static long int maxArgCount = std::numeric_limits<long int>::max();
        if (maxArgCount == std::numeric_limits<long int>::max()) {
            maxArgCount = sysconf(_SC_ARG_MAX);
            if (maxArgCount < 0) {
                // set conservative value
                maxArgCount = 64;
            }
        }
        return static_cast<size_t>(maxArgCount);
    }
}
