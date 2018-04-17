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

#include "yal/frontend/types/identifier.h"
#include "yal/frontend/module.h"

namespace yal::frontend {


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

    static std::string
    BuildTypeNameWithModule(const StringRef name,
                            const StringRef target,
                            const Module& module) {

        std::string result;
        StringRef moduleName = module.getName();

        result.reserve(name.size() + moduleName.size() + 4 +  target.size());
        result.append(moduleName.data(), moduleName.size());
        result += "::";
        result.append(target.data(), target.size());
        result += "::";
        result.append(name.data(), name.size());
        return result;
    }

    Identifier::Identifier() {

    }

    Identifier::Identifier(const StringRef idString):
        m_name(idString),
        m_idString(idString.toString()) {

    }

    Identifier::Identifier(const StringRef idString,
                           const Module& module):
        m_name(idString),
        m_idString(BuildTypeNameWithModule(idString, module)){

    }


    Identifier::Identifier(const StringRef idString,
                           const StringRef targetString,
                           const frontend::Module& module):
        m_name(idString),
        m_idString(BuildTypeNameWithModule(idString,
                                           targetString,
                                           module))
    {

    }

    StringRef
    Identifier::getAsString() const {
        return m_idString;
    }

    void
    Identifier::setIdString(const StringRef idString) {
        m_name = idString;
        m_idString = idString.toString();
    }

    void
    Identifier::setIdString(const StringRef idString,
                            const Module& module) {
        m_name = idString;
        m_idString =BuildTypeNameWithModule(idString, module);
    }

    void
    Identifier::setIdString(const StringRef idString,
                            const StringRef targetString,
                            const Module& module) {
        m_name = idString;
        m_idString =BuildTypeNameWithModule(idString, targetString, module);
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
