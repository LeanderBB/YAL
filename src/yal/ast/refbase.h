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
#include "yal/ast/asttypes.h"
#include "yal/io/sourcemanager.h"
#include "yal/util/cast.h"

namespace yal {

    class Module;

    class RefBase {
    public:

        static void* operator new(std::size_t bytes,
                                  Module& ctx);

   protected:
        RefBase(Module& module,
                const ASTType astType,
                const Type* type);

    public:
        virtual ~RefBase();

        bool isTypeDefined() const {
            return m_type != nullptr;
        }

        const Type* getType() const {
            return m_type;
        }

        ASTType getASTType() const {
            return m_astType;
        }

    protected:
        Module& m_module;
        const ASTType m_astType;
        const Type* m_type;
    };

    template<>
    struct cast_typeid<RefBase> {
        typedef ASTType type;
    };

    inline ASTType get_typeid(const RefBase& ref) {
        return ref.getASTType();
    }
}
