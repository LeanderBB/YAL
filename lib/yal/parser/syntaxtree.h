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
#include "yal/util/cast.h"
#include "yal/util/stringref.h"
#include "yal/parser/sttype.h"
namespace yal {

    enum class SyntaxTreeType {
#define YAL_ST_NODE_TYPE(type) type,
#include "yal/parser/syntaxtreetypes.def"
#undef YAL_ST_NODE_TYPE
        TypeInvalid,
    };

#define YAL_ST_NODE_TYPE(CLASS) \
    class CLASS; \
    template<> struct cast_typeid< CLASS >{typedef SyntaxTreeType type;}; \
    template<> constexpr cast_typeid< CLASS >::type get_typeid< CLASS >() {return SyntaxTreeType::CLASS ;}
#include "yal/parser/syntaxtreetypes.def"
#undef YAL_ST_NODE_TYPE

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

    template<>
    struct cast_typeid<SyntaxTree> {
        typedef SyntaxTreeType type;
    };

    inline SyntaxTreeType get_typeid(const SyntaxTree& st) {
        return st.getSyntaxTreeType();
    }


    class STDecl : public SyntaxTree {
    public:

    protected:
        STDecl(const SyntaxTreeType type);
    };

    template<>
    struct cast_typeid<STDecl> {
        typedef SyntaxTreeType type;
    };

    inline SyntaxTreeType get_typeid(const STDecl& st) {
        return st.getSyntaxTreeType();
    }
}
