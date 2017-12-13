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

#include "yal/ast/identifier.h"
#include "yal/ast/module.h"

namespace yal {


    static std::string
    BuildTypeNameWithModule(const StringRef name,
                            const Module& module) {

        std::string result;
        StringRef moduleName = module.getName();

        result.reserve(name.size() + moduleName.size() + 2);
        result.append(moduleName.data(), moduleName.size());
        result += "::";
        result.append(name.data(), name.size());
        return result;
    }

    Identifier::Identifier() {

    }

    Identifier::Identifier(StringRef idString):
        m_name(idString),
        m_idString(idString.toString()) {

    }

    Identifier::Identifier(StringRef idString,
                           const Module* module):
        m_name(idString){
        if (module != nullptr) {
            m_idString =BuildTypeNameWithModule(idString, *module);
        } else {
            m_idString = idString.toString();
        }
    }


    Identifier::Identifier(StringRef idString,
                           const Module& module):
        m_name(idString),
        m_idString(BuildTypeNameWithModule(idString, module))
    {

    }

    StringRef
    Identifier::getAsString() const {
        return m_idString;
    }

    void
    Identifier::setIdString(StringRef idString) {
        m_idString = idString.toString();
    }

    void
    Identifier::setIdString(StringRef idString,
                            const Module& module) {
        m_idString =BuildTypeNameWithModule(idString, module);
    }

    bool
    Identifier::operator == (const Identifier& other) const {
        return m_idString == other.m_idString;
    }

    bool
    Identifier::operator != (const Identifier& other) const {
        return m_idString != other.m_idString;
    }

}
