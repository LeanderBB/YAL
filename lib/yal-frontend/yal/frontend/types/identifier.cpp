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

#include "yal/frontend/types/identifier.h"
#include "yal/frontend/module.h"
#include "yal/frontend/parser/sttype.h"
#include "yal/util/hash.h"

namespace yal::frontend {


    static std::string
    BuildTypeNameWithModule(const StringRef& name,
                            const StringRef& target,
                            const StringRef& moduleName) {

        std::string result;
        result.reserve(name.size() + moduleName.size() + 4 +  target.size());
        if (moduleName.length() != 0) {
            result.append(moduleName.data(), moduleName.size());
            result += "::";
        }
        if (target.length() != 0) {
            result.append(target.data(), target.size());
            result += "::";
        }
        result.append(name.data(), name.size());
        return result;
    }

    Identifier::Identifier() {

    }

    Identifier::Identifier(const StringRef idString):
        m_name(idString),
        m_target(),
        m_module() {
        buildHash();
    }

    Identifier::Identifier(const StringRef idString,
                           const Module& module):
        m_name(idString),
        m_target(),
        m_module(module.getName()){
        buildHash();
    }


    Identifier::Identifier(const StringRef idString,
                           const StringRef targetString,
                           const frontend::Module& module):
        m_name(idString),
        m_target(targetString),
        m_module(module.getName()) {
        buildHash();
    }

    Identifier::Identifier(const StringRef idString,
                           const StringRef targetString,
                           const StringRef module):
        m_name(idString),
        m_target(targetString),
        m_module(module) {
        buildHash();
    }

    void
    Identifier::setIdString(const StringRef idString) {
        m_name = idString;
        m_target.clear();
        m_module.clear();
        buildHash();
    }

    void
    Identifier::setIdString(const StringRef idString,
                            const Module& module) {
        m_name = idString;
        m_target.clear();
        m_module = module.getName();
        buildHash();
    }

    void
    Identifier::setIdString(const StringRef idString,
                            const StringRef targetString,
                            const Module& module) {
        m_name = idString;
        m_target = targetString;
        m_module = module.getName();
        buildHash();
    }

    std::string
    Identifier::getFullIdentifier() const {
        return BuildTypeNameWithModule(m_name,
                                       m_target,
                                       m_module);
    }

    bool
    Identifier::operator == (const Identifier& other) const {

        if (m_hash == other.m_hash) {
            return m_name == other.m_name
                    && m_target == other.m_target
                    && m_module == other.m_module;
        }
        return false;
    }

    bool
    Identifier::operator != (const Identifier& other) const {
        return !operator ==(other);
    }

    void
    Identifier::buildHash() {
        HashStr hash;
        hash.begin();
        if (m_module.size() != 0) {
            hash.consume(m_module);
            hash.consume("::");
        }
        hash.consume(m_module);
        if (m_target.size() != 0) {
            hash.consume(m_target);
            hash.consume("::");
        }
        hash.consume(m_name);
        hash.end();
        m_hash = hash.get();
    }

}
