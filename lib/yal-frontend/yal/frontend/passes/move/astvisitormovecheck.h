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

#include "yal/frontend/ast/astvisitor.h"
#include "yal/util/stackjump.h"

#include <unordered_map>
#include <memory>

namespace yal {
    class ErrorReporter;
    class Error;
    struct SourceInfo;
}


namespace yal::frontend {

    struct PassOptions;
    class Module;
    class DeclScope;
    class DeclBase;
    class Statement;

    class AstVisitorMoveCheck : public AstVisitor {
    public:
        AstVisitorMoveCheck(PassOptions& options);

        void execute();

#define YAL_AST_NODE_TYPE(TYPE) void visit(TYPE&) override final;
#include "yal/frontend/ast/astnodes.def"
#undef YAL_AST_NODE_TYPE

    private:
        class DeclScopeGuard;
        class EvalMoveStateScope;

        struct MoveState {
            const Statement* stmtWhereMoved;
            bool moved = true;
        };

        struct EvalMoveState {
            const DeclScope* scope = nullptr;
            bool enabled = false;
            bool exit = false;
            bool structReplace = false;
        };

        void onError(std::unique_ptr<Error>&& error);

        MoveState& findState(const DeclBase& node);

    private:
        using MoveMap  = std::unordered_map<const DeclBase*, MoveState>;
        Module& m_module;
        ErrorReporter& m_errReporter;
        const DeclScope* m_activeScope;
        StackJump m_jump;
        MoveMap m_moveMap;
        EvalMoveState m_evalMoveState;
    };

}
