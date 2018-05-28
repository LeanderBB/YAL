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

#include "yal/io/sourcemanager.h"
#include "yal/frontend/parser/stcontext.h"
#include <memory>


namespace yal {
    class SourceItem;
    class ErrorReporter;
}

namespace yal::frontend {

    class Lexer;
    struct TokenInfo;

    class STParser {
    public:

        static void OnParseError(STParser& parser);

        STParser(Lexer& lexer,
                 STContext& stcontext,
                 ErrorReporter& reporter,
                 SourceItem& sourceItem);

        STDeclModule* getDeclModule() const {
            return m_stcontext.getDeclModule();
        }

        bool parse();

        ErrorReporter& getErrorReporter() {
            return m_errorReporter;
        }

        template <typename T, typename... ARGS>
        T* createNode(ARGS&& ...args) {
            return m_stcontext.create<T>(std::forward<ARGS>(args)...);
        }

        SourceManager::Handle getSourceHandle() const;


        template<typename T>
        std::vector<T, StdAllocatorWrapperStack<T>>* createVector() {
            auto allocator = m_stcontext.getStdAllocatorWrapper<T>();
            return m_stcontext.create<std::vector<T, StdAllocatorWrapperStack<T>>>(allocator);
        }

        /*
        template<typename T>
        std::vector<T>* createVector() {
            return m_stcontext.create<std::vector<T>>();
        }*/

    private:
        std::unique_ptr<void, void(*)(void*)> m_parserImpl;
        STContext& m_stcontext;
        Lexer& m_lexer;
        ErrorReporter& m_errorReporter;
        SourceItem& m_sourceItem;
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
