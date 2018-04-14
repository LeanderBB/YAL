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

#include "yal/ast/asttypes.h"
#include "yal/io/sourcemanager.h"
#include "yal/util/iteratorrange.h"
#include <vector>
#include <type_traits>
#include <algorithm>

namespace yal {

    namespace frontend{
        class Module;
    }

    template<typename T, bool reverse>
    struct iterator_fn;

    template<typename T>
    struct iterator_fn<T, true>{

        static typename T::reverse_iterator begin(T& t) {
            return t.rbegin();
        }

        static typename T::reverse_iterator end(T& t) {
            return t.rend();
        }

        static typename T::const_reverse_iterator cbegin(const T& t) {
            return t.crbegin();
        }

        static typename T::const_reverse_iterator cend(const T& t) {
            return t.crend();
        }

    };

    template<typename T>
    struct iterator_fn<T, false>{

        static typename T::iterator begin(T& t) {
            return t.begin();
        }

        static typename T::iterator end(T& t) {
            return t.end();
        }

        static typename T::const_iterator cbegin(const T& t) {
            return t.cbegin();
        }

        static typename T::const_iterator cend(const T& t) {
            return t.cend();
        }

    };


    template<typename NodeType, AstType TYPE, bool ReverseIter>
    class NodeContainer {

        static_assert(std::is_pointer<NodeType>::value,
        "NodeType needs to be a pointer type");

        using ThisType =  NodeContainer<NodeType,TYPE,ReverseIter>;

    public:
        using ContainerType = std::vector<NodeType>;
        using IteratorType = typename std::conditional<ReverseIter,
            typename ContainerType::reverse_iterator,
            typename ContainerType::iterator>::type;
        using ConstIteratorType = typename std::conditional<ReverseIter,
            typename ContainerType::const_reverse_iterator,
            typename ContainerType::const_iterator>::type;
        using ChildType = NodeType;
        using NodeRange = IteratorRange<IteratorType>;
        using ConstNodeRange = IteratorRange<ConstIteratorType>;

        NodeContainer(frontend::Module&) {

        }

        NodeContainer(frontend::Module&,
                      ContainerType&& nodes) :
        m_nodes(std::move(nodes)) {

        }

        NodeContainer(const ThisType&) = delete;
        NodeContainer(ThisType&&) = delete;
        NodeContainer& operator=(const ThisType&) = delete;
        NodeContainer& operator=(ThisType&&) = delete;

        virtual ~NodeContainer() {}

        AstType getAstType() const {
            return TYPE;
        }

        size_t getCount() const {
            return m_nodes.size();
        }

        IteratorType childBegin() {
            return iterator_fn<ContainerType, ReverseIter>::begin(m_nodes);
        }

        IteratorType childEnd() {
            return iterator_fn<ContainerType, ReverseIter>::end(m_nodes);
        }

        const SourceInfo& getSourceInfo() const {
            return m_sourceInfo;
        }

        SourceInfo& getSourceInfo() {
            return const_cast<SourceInfo&>(
                        static_cast<const ThisType*>(this)->getSourceInfo());
        }

        void setSourceInfo(const SourceInfo& sourceInfo) {
            m_sourceInfo = sourceInfo;
        }


        NodeRange getChildRange() {
            return NodeRange(iterator_fn<ContainerType, ReverseIter>::begin(m_nodes),
                             iterator_fn<ContainerType, ReverseIter>::end(m_nodes));
        }

        ConstNodeRange getChildRangeConst() const {
            return ConstNodeRange(iterator_fn<ContainerType, ReverseIter>::cbegin(m_nodes),
                                  iterator_fn<ContainerType, ReverseIter>::cend(m_nodes));
        }


        IteratorType insertAfter(IteratorType it,
                                 ChildType child) {
            return m_nodes.insert(it, child);
        }

        void updateSourceInfo() {
            if (m_nodes.size() == 1) {
                m_sourceInfo = m_nodes[0]->getSourceInfo();
            } else {
                auto back = m_nodes.front()->getSourceInfo();
                auto front = m_nodes.back()->getSourceInfo();
                m_sourceInfo.begin.line = front.begin.line;
                m_sourceInfo.begin.column = front.begin.column;
                m_sourceInfo.begin.streamOffset = front.begin.streamOffset;
                m_sourceInfo.end.line = back.end.line;
                m_sourceInfo.end.column = back.end.column;
                m_sourceInfo.end.streamOffset = back.end.streamOffset;
            }
        }

        NodeType& operator [](const size_t i) {
            return m_nodes[i];
        }

        const NodeType& operator [](const size_t i) const {
            return m_nodes[i];
        }

    protected:
        ContainerType m_nodes;
        SourceInfo m_sourceInfo;
    };

}
