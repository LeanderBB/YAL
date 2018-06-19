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
    class QualType;

    class AstVisitorExprType final : public AstVisitor {
    public:
        AstVisitorExprType(PassOptions& options);

        void execute();

#define YAL_AST_NODE_TYPE(TYPE) void visit(TYPE&) override final;
#include "yal/frontend/ast/astnodes.def"
#undef YAL_AST_NODE_TYPE

    private:
        class DeclScopeGuard;

        void onError(std::unique_ptr<Error>&& error);

        void isValidTypeConversion(const QualType qtFrom,
                                   const QualType qtTo,
                                   const SourceInfo& location);

        void isValidCastConversion(const ExprCast& cast);
    private:
        Module& m_module;
        ErrorReporter& m_errReporter;
        const DeclScope* m_activeScope;
        StackJump m_jump;
    };

}
