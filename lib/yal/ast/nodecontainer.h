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
#include "yal/ast/astcontext.h"
#include "yal/ast/module.h"
#include "yal/io/sourcemanager.h"
#include "yal/util/iteratorrange.h"
#include <vector>
#include <type_traits>

namespace yal {

    template<typename NodeType, AstType TYPE>
    class NodeContainer {

        static_assert(std::is_pointer<NodeType>::value,
        "NodeType needs to be a pointer type");

        using ThisType =  NodeContainer<NodeType,TYPE>;

    public:
        using ContainerType = std::vector<NodeType>;
        using ChildType = NodeType;
        using NodeRange = IteratorRange<typename ContainerType::reverse_iterator>;

        NodeContainer(Module&) {

        }

        NodeContainer(Module&,
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

        typename ContainerType::reverse_iterator childBegin() {
            return m_nodes.rbegin();
        }

        typename ContainerType::reverse_iterator childEnd() {
            return m_nodes.rend();
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
            return NodeRange(std::rbegin(m_nodes), std::rend(m_nodes));
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

    protected:
        ContainerType m_nodes;
        SourceInfo m_sourceInfo;
    };

}
