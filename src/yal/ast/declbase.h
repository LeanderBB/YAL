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

#include <string>
#include <new>

namespace yal{

    class Module;

    class DeclBase
    {
    public:

        static void* operator new(std::size_t bytes,
                                  Module& ctx);

        enum class Kind {
#define YAL_AST_DECLBASE_TYPE(type) type,
#include "yal/ast/declbasetypes.def"
#undef YAL_AST_DECLBASE_TYPE
        };

        DeclBase(Module& module,
                 const Kind kind,
                 const ASTType type);

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

        Kind getKind() const {
            return m_kind;
        }

        ASTType getASTType() const {
            return m_astType;
        }

        void setName(const char* name);

        const char* getName() const {
            return m_name.c_str();
        }

        Module& getModule() {
            return m_module;
        }

        const Module& getModule() const {
            return m_module;
        }

    protected:
        Module& m_module;
        const ASTType m_astType;
        const Kind m_kind;
        SourceInfo m_sourceInfo;
        std::string m_name;
    };


}
