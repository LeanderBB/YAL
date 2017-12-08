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
#include "yal/ast/astcontext.h"
#include <cstdint>
#include <cinttypes>
#include "yal/ast/declbase.h"
#include "yal/ast/declmodule.h"
#include "yal/ast/module.h"
namespace yal{

    enum {
        kStackSizeBytes = 1024 * 1024
    };

    ASTContext::ASTContext() :
        m_allocator(kStackSizeBytes){
    }

    void
    ASTContext::addNode(DeclBase* node) {
        if (&node->getModule().getASTContext() == this) {
            m_declNodes.push_back(node);
        }
    }

    void
    ASTContext::addNode(DeclModule* node) {
        if (&node->getModule().getASTContext() == this) {
            m_moduleNodes.push_back(node);
        }
    }
}
