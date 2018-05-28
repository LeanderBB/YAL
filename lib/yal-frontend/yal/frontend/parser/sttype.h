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
#include "yal/util/stringref.h"

namespace yal::frontend {

    class STIdentifier {
    public:

        STIdentifier(const StringRef identifier);

        void setSourceInfo(const SourceInfo& sourceInfo);

        const SourceInfo& getSourceInfo() const {
            return m_sourceInfo;
        }

        StringRef getString() const {
            return m_identifier;
        }

    protected:
        const StringRef m_identifier;
        SourceInfo m_sourceInfo;
    };


    class STType {
    public:

        enum class Type {
            Bool,
            Int8,
            Int16,
            Int32,
            Int64,
            UInt8,
            UInt16,
            UInt32,
            UInt64,
            Float32,
            Float64,
            Custom
        };

        STType(const Type type);

        STType(const StringRef type);

        void setSourceInfo(const SourceInfo& sourceInfo);

        const SourceInfo& getSourceInfo() const {
            return m_sourceInfo;
        }

        Type getType() const {
            return m_type;
        }

        StringRef getIdentifier() const {
            return m_identifier;
        }

    private:
        const Type m_type;
        const StringRef m_identifier;
        SourceInfo m_sourceInfo;
    };

    class STQualType {
    public:

        enum Qualifiers {
            kQualReference = 1 << 0,
            kQualMutable = 1 << 1
        };

        STQualType(const STType* type,
                   const uint32_t qualifiers);

        const SourceInfo& getSourceInfo() const {
            YAL_ASSERT(m_type != nullptr);
            return m_type->getSourceInfo();
        }

    public:
        const STType* m_type;
        const uint32_t m_qualifiers;
    };

}
