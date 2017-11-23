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
#include "yal/ast/type.h"
#include "yal/io/sourcemanager.h"
#include "yal/util/cast.h"
#include "yal/util/stringref.h"
namespace yal{

    class Module;
    class AstVisitor;

    class DeclBase
    {
    public:

        static void* operator new(std::size_t bytes,
                                  Module& ctx);

        DeclBase(Module& module,
                 const AstType type,
                 StringRef name);

        YAL_NO_COPY_CLASS(DeclBase);

        virtual ~DeclBase();


        const SourceInfo& getSourceInfo() const {
            return m_sourceInfo;
        }

        SourceInfo& getSourceInfo() {
            return const_cast<SourceInfo&>(
                        static_cast<const DeclBase*>(this)->getSourceInfo());
        }

        void setSourceInfo(const SourceInfo& sourceInfo);

        AstType getAstType() const {
            return m_astType;
        }

        const StringRef getName() const {
            return m_name;
        }

        Module& getModule() {
            return m_module;
        }

        const Module& getModule() const {
            return m_module;
        }

        virtual void acceptVisitor(AstVisitor& visitor) = 0;

    protected:
        Module& m_module;
        const AstType m_astType;
        SourceInfo m_sourceInfo;
        StringRef m_name;
    };

    template<>
    struct cast_typeid<DeclBase> {
        typedef AstType type;
    };

    inline AstType get_typeid(const DeclBase& decl) {
        return decl.getAstType();
    }

}
