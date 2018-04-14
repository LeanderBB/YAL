/*
 *  Copyright 2018 by Leander Beernaert (leanderbb@gmail.com)
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
#include "yal/util/allocatorstack.h"
#include <memory>

namespace yal {

    class Log;
    class Lexer;
    class STDeclModule;
    struct TokenInfo;
    class SourceItem;
    class ErrorReporter;

    class STParser {
    public:

        static void OnParseError(STParser& parser);

        STParser(Lexer& lexer,
                 ErrorReporter& reporter,
                 SourceItem& sourceItem);

        STDeclModule* getDeclModule() const {
            return m_declModule;
        }

        bool parse();

        ErrorReporter& getErrorReporter() {
            return m_errorReporter;
        }

        template <typename T, typename... ARGS>
        T* createNode(ARGS&& ...args) {
            return m_allocator.construct<T>(std::forward<ARGS>(args)...);
        }

        SourceManager::Handle getSourceHandle() const;

    private:
        AllocatorStack m_allocator;
        std::unique_ptr<void, void(*)(void*)> m_parserImpl;
        Lexer& m_lexer;
        ErrorReporter& m_errorReporter;
        SourceItem& m_sourceItem;
        STDeclModule* m_declModule;
    };

    SourceInfo CreateSourceInfo(const TokenInfo& start,
                                const TokenInfo& end,
                                const SourceManager::Handle handle);

    SourceInfo CreateSourceInfo(const TokenInfo& start,
                                const SourceInfo& end,
                                const SourceManager::Handle handle);

    SourceInfo CreateSourceInfo(const SourceInfo& start,
                                const TokenInfo& end,
                                const SourceManager::Handle handle);

    SourceInfo CreateSourceInfo(const SourceInfo& start,
                                const SourceInfo& end,
                                const SourceManager::Handle handle);
}
