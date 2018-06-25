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
    class STDecl;
    class STStatement;
    class STExpression;
    class STStructMemberInit;
    class STStructMember;
    class STDeclParam;

    template <typename T>
    class ParseList {
    public:
        using DestinationType = std::vector<const T*, StdAllocatorWrapperStack<const T*>>;
        template<typename O>
        using DestinationTypeOther = std::vector<const O*, StdAllocatorWrapperStack<const O*>>;

        struct Range {
            size_t begin;
            size_t end;
        };

        ParseList() {
            m_list.reserve(32);
        }

        ParseList(const ParseList<T>&) = delete;
        ParseList<T>& operator = (const ParseList<T>&) = delete;

        size_t getSize() const {
            return m_list.size();
        }

        Range getRangeEmpty() const {
            return Range {0,0};
        }

        Range add(const T* value) {
            Range r;
            r.begin = m_list.size();
            r.end = r.begin + 1;
            m_list.push_back(value);
            return r;
        }

        Range add(const Range& range,
                  const T* value) {
            Range r;
            r.begin = range.begin;
            r.end = m_list.size() + 1;
            m_list.push_back(value);
            return r;
        }

        size_t moveRange(DestinationType &dest,
                         const Range& range) {
            if( range.begin < m_list.size()
                    && range.end > range.begin
                    && range.end <=m_list.size()) {
                const size_t size = range.end - range.begin;
                dest.clear();
                dest.reserve(size);

                auto itBegin = m_list.begin() + range.begin;
                auto itEnd = m_list.begin() + range.end;
                dest.assign(itBegin,itEnd);
                m_list.erase(itBegin, itEnd);
                return size;
            }
            return 0;
        }

        template<typename O, typename C>
        size_t moveRange(DestinationTypeOther<O> &dest,
                         const typename ParseList<C>::Range& range) {
            if( range.begin < m_list.size()
                    && range.end > range.begin
                    && range.end <=m_list.size()) {
                const size_t size = range.end - range.begin;
                dest.clear();
                dest.reserve(size);

                auto itBegin = m_list.begin() + range.begin;
                auto itEnd = m_list.begin() + range.end;
                for (auto it = itBegin; it != itEnd; ++it)
                {
                    dest.push_back(static_cast<const O*>(*it));
                }
                m_list.erase(itBegin, itEnd);
                return size;
            }
            return 0;
        }

    private:
        std::vector<const T*> m_list;
    };

    using ParseListDecl = ParseList<STDecl>;
    using ParseListExpr = ParseList<STExpression>;
    using ParseListStmt = ParseList<STStatement>;
    using ParseListStructInit = ParseList<STStructMemberInit>;
    using ParseListStructMember = ParseList<STStructMember>;
    using ParseListDeclParam = ParseList<STDeclParam>;

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


        ParseListDecl& getDeclList() {
            return m_listDecl;
        }

        ParseListExpr& getExprList() {
            return m_listExpr;
        }

        ParseListStmt& getStmtList() {
            return m_listStmt;
        }

        ParseListStructInit& getStructInitList() {
            return m_listStructInit;
        }

        ParseListStructMember& getStructMemberList() {
            return m_listStructMember;
        }

        ParseListDeclParam& getDeclParamList() {
            return m_listDeclParam;
        }

        STContext& getSTContext() {
            return m_stcontext;
        }

    private:
        std::unique_ptr<void, void(*)(void*)> m_parserImpl;
        ParseListDecl m_listDecl;
        ParseListExpr m_listExpr;
        ParseListStmt m_listStmt;
        ParseListStructInit m_listStructInit;
        ParseListStructMember m_listStructMember;
        ParseListDeclParam m_listDeclParam;
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
