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

#include "yal/util/stringref.h"
#include "yal/util/formattypes.h"
#include <string>
#include <unordered_map>

namespace yal::frontend {

    class Module;
    class Identifier
    {
    public:

        Identifier();

        explicit Identifier(const StringRef idString);

        explicit Identifier(const StringRef idString,
                            const frontend::Module& module);

        explicit Identifier(const StringRef idString,
                            const StringRef targetString,
                            const frontend::Module& module);

        explicit Identifier(const StringRef idString,
                            const StringRef targetString,
                            const StringRef module);

        StringRef getName() const {
            return m_name;
        }

        StringRef getTarget() const {
            return m_target;
        }

        StringRef getModule() const {
            return m_module;
        }

        size_t getHash() const {
            return m_hash;
        }

        std::string getFullIdentifier() const;

        void setIdString(const StringRef idString);

        void setIdString(const StringRef idString,
                         const Module& module);

        void setIdString(const StringRef idString,
                         const StringRef targetString,
                         const Module& module);

        bool operator == (const Identifier& other) const;
        bool operator != (const Identifier& other) const;

    private:

        void buildHash();

    private:
        size_t m_hash;
        StringRef m_name;
        StringRef m_target;
        StringRef m_module;
    };


    inline size_t FormatType(FormatTypeArgs& loc,
                             const Identifier& value) {
        return FormatType(loc, value.getFullIdentifier());
    }


    struct IdentifierPtrHasher {
        std::size_t operator()(const Identifier* i) const {
            return i->getHash();
        }
    };

    struct IdentifierPtrKeyComp {
        bool operator()(const Identifier* i1,
                        const Identifier* i2) const {
            return *i1 == *i2;
        }
    };

    template<typename T>
    using IdentifierPtrMap = std::unordered_map<const Identifier*, T,
    IdentifierPtrHasher,
    IdentifierPtrKeyComp>;
}

namespace std {
    template<> struct hash<yal::frontend::Identifier> {
        typedef yal::frontend::Identifier argument_type;
        typedef std::size_t result_type;
        result_type operator()(argument_type const& s) const noexcept
        {
            return s.getHash();
        }
    };
}
