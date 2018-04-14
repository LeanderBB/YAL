/*
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

#include "yal/io/sourcemanager.h"

namespace yal {

    class Log;
    class ModuleManager;
    class MemoryStream;
    class ByteStream;
    class Module;
    class ErrorReporter;

    class Compiler {
    public:

        enum Flags {

        };

        Compiler(Log& log,
                 ErrorReporter& errorReporter,
                 SourceManager& srcManager,
                 ModuleManager& moduleManager);

        Log& getLog() {
            return m_log;
        }

        SourceManager& getSourceManager() {
            return m_srcManager;
        }

        ModuleManager& getModuleManager() {
            return m_moduleManager;
        }

        ErrorReporter& getErrorReporter() {
            return m_errorReporter;
        }

        const ErrorReporter& getErrorReporter() const {
            return m_errorReporter;
        }

        void setFlags(const uint64_t flags);

        Module* compile(const SourceManager::Handle source);

    private:
        Log& m_log;
        SourceManager& m_srcManager;
        ModuleManager& m_moduleManager;
        ErrorReporter& m_errorReporter;
        uint64_t m_flags;
    };

}
