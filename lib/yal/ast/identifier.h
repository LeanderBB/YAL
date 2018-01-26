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
#include <string>
#include "yal/util/stringref.h"
#include "yal/util/formattypes.h"
namespace yal {

    class Module;
    class StringRef;
    class Identifier
    {
    public:

        Identifier();

        Identifier(StringRef idString,
                   const Module& module);

        Identifier(StringRef idString);

        Identifier(StringRef idString,
                   const Module* module);

        StringRef getName() const {
            return m_name;
        }

        StringRef getAsString() const;

        const std::string& getAsStdString() const {
            return m_idString;
        }

        void setIdString(StringRef idString);

        void setIdString(StringRef idString,
                         const Module& module);

        bool operator == (const Identifier& other) const;
        bool operator != (const Identifier& other) const;

    private:
        StringRef m_name;
        std::string m_idString;
    };


    inline size_t FormatType(FormatTypeArgs& loc,
                             const Identifier& value) {
       return FormatType(loc, value.getAsString());
    }
}
