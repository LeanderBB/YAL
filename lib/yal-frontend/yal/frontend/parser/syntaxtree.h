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
#include "yal/frontend/parser/sttype.h"
#include "yal/util/allocator/allocatorstack.h"
#include "yal/util/cast.h"
#include "yal/util/stringref.h"

namespace yal::frontend {

    template<typename T>
    using STVector = std::vector<T, StdAllocatorWrapperStack<T>>;

    //template<typename T>
    //using STVector = std::vector<T>;

    enum class SyntaxTreeType {
#define YAL_ST_NODE_TYPE(type) type,
#include "yal/frontend/parser/syntaxtreetypes.def"
#undef YAL_ST_NODE_TYPE
        TypeInvalid,
    };

#define YAL_ST_NODE_TYPE(CLASS) \
    class CLASS;
#include "yal/frontend/parser/syntaxtreetypes.def"
#undef YAL_ST_NODE_TYPE

}

namespace yal {
#define YAL_ST_NODE_TYPE(CLASS) \
    template<> struct cast_typeid< yal::frontend::CLASS >{typedef yal::frontend::SyntaxTreeType type;}; \
    template<> constexpr cast_typeid< yal::frontend::CLASS >::type get_typeid< yal::frontend::CLASS >() {return yal::frontend::SyntaxTreeType::CLASS ;}
#include "yal/frontend/parser/syntaxtreetypes.def"
#undef YAL_ST_NODE_TYPE
}

namespace yal::frontend {

    class SyntaxTreeVisitor;
    class SyntaxTree {
    public:

        virtual ~SyntaxTree(){}

        virtual void acceptVisitor(SyntaxTreeVisitor& visitor) const = 0;

        SyntaxTreeType getSyntaxTreeType() const {
            return m_treeType;
        }

        const SourceInfo& getSourceInfo() const {
            return m_sourceInfo;
        }

        void setSourceInfo(const SourceInfo& sourceInfo);

    protected:
        SyntaxTree(const SyntaxTreeType type);

    private:
        const SyntaxTreeType m_treeType;
        SourceInfo m_sourceInfo;
    };

    class STDecl : public SyntaxTree {
    public:

        const STIdentifier& getName() const {
            return *m_name;
        }

    protected:
        STDecl(const SyntaxTreeType type,
               const STIdentifier* name);

    protected:
        const STIdentifier* m_name;
    };
}

namespace yal {

    template<>
    struct cast_typeid<frontend::SyntaxTree> {
        typedef frontend::SyntaxTreeType type;
    };

    inline frontend::SyntaxTreeType get_typeid(const frontend::SyntaxTree& st) {
        return st.getSyntaxTreeType();
    }

    template<>
    struct cast_typeid<frontend::STDecl> {
        typedef frontend::SyntaxTreeType type;
    };

    inline frontend::SyntaxTreeType get_typeid(const frontend::STDecl& st) {
        return st.getSyntaxTreeType();
    }
}

