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

        static void* operator new(std::size_t bytes,
                                  Module& module) {
            ASTContext& astctx = module.getASTContext();
            void* ptr = astctx.allocate(bytes);
            YAL_ASSERT_MESSAGE(ptr != nullptr,
                               "NodeContainer: Failed to allocate memory");
            return ptr;
        }

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

        typename ContainerType::iterator childBegin() {
            return m_nodes.begin();
        }

        typename ContainerType::iterator childEnd() {
            return m_nodes.end();
        }

    protected:
        ContainerType m_nodes;
    };

}
