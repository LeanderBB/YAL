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
#include "yal/compiler/stages/stageexprlistbreakdown.h"
#include "yal/compiler/compiler.h"
#include "yal/ast/astnodes.h"
#include "yal/util/prettyprint.h"
#include "yal/util/log.h"
#include "yal/util/stackjump.h"
#include "yal/util/format.h"
namespace yal {

    static const StringRef kTempVarPrefix("_tmpvar_");

    class ExprBreakVisitor final : public AstVisitor
    {
    public:
        ExprBreakVisitor(Module& module):
            m_module(module),
            m_formater(),
            m_tmpVarCount(0) {
        }

#define YAL_AST_NODE_TYPE(TYPE) virtual void visit(TYPE&) override final;
#include "yal/ast/astnodes.def"
#undef YAL_AST_NODE_TYPE

        bool didError() const {
            return false;
        }

    private:

        bool requiresTmpStorage(const StmtExpression& expr) const;

        StmtExpression* processExpression(StmtExpression* expr,
                                          DeclScope* scope);

        StringRef nextTmpVarName(DeclScope *scope);

        StatementList::IteratorType processQueuedChanges(StatementList &list,
                                                         StatementList::IteratorType it);

        void pushScope(DeclScope* scope) {
            m_curScope = scope;
        }

        void popScope() {
            m_curScope = const_cast<DeclScope*>(m_curScope->getParentScope());
        }

    private:
        struct ExprReorderInfo {
            DeclVar* decl;
            StmtExpression* expr;
        };

        Module& m_module;
        DeclScope* m_curScope;
        FormaterStack<256> m_formater;
        std::vector<ExprReorderInfo> m_stageInfo;
        uint32_t m_tmpVarCount;
    };

    bool
    ExprBreakVisitor::requiresTmpStorage(const StmtExpression& expr) const {
        AstType type = expr.getAstType();

        const ExprUnaryOperator* exprUnaryOp = dyn_cast<ExprUnaryOperator>(&expr);

        // If we have an unary operator, check its expression type instead
        if (exprUnaryOp != nullptr) {
            type = exprUnaryOp->getExpression()->getAstType();
        }

        return type != AstType::ExprVarRef
                && type != AstType::ExprVarRefSelf
                && type != AstType::ExprIntegerLiteral
                && type != AstType::ExprFloatLiteral
                && type != AstType::ExprBoolLiteral;
    }

    StmtExpression*
    ExprBreakVisitor::processExpression(StmtExpression* expr,
                                        DeclScope* scope) {
        YAL_ASSERT_MESSAGE(scope != nullptr, "Forgot to set scope");
        const StringRef tmpVar = nextTmpVarName(scope);

        // Create Var Decl
        DeclVar* tmpVarDecl = m_module.newASTNode<DeclVar>(tmpVar,
                                                           scope,
                                                           expr->getQualType(),
                                                           expr);
        // Register with scop
        scope->addDecl(tmpVarDecl);

        // create replacement expr
        ExprVarRef* newExpr = m_module.newASTNode<ExprVarRef>(tmpVarDecl);
        m_stageInfo.push_back(ExprReorderInfo{tmpVarDecl, expr});

        // return replacment expr
        return newExpr;
    }

    inline static StringRef copyToAstAllocator(Module& module,
                                               const StringRef str) {
        void* ptr = module.getASTContext().getAllocator().allocate(str.size());
        if (ptr != nullptr)
        {
            memcpy(ptr, str.data(), str.size());
            return StringRef(reinterpret_cast<const char*>(ptr), str.size());
        }
        return StringRef();
    }

    StringRef ExprBreakVisitor::nextTmpVarName(DeclScope* scope) {
        // Allocate new variable name for temporary storage
        StringRef tmpVarName;
        while(true) {
            Format(m_formater, "%%", kTempVarPrefix, m_tmpVarCount);
            tmpVarName =m_formater.toStringRef();
            const Identifier id(tmpVarName);
            ++m_tmpVarCount;
            if (!scope->hasDecl(id, true)) {
                tmpVarName = copyToAstAllocator(m_module, tmpVarName);
                YAL_ASSERT(tmpVarName.data() != nullptr);
                break;
            }
        }
        return tmpVarName;
    }


    StatementList::IteratorType
    ExprBreakVisitor::processQueuedChanges(StatementList& list,
                                           StatementList::IteratorType it)
    {
        if (!m_stageInfo.empty()) {
            for (auto rit = m_stageInfo.rbegin(); rit != m_stageInfo.rend(); ++rit) {
                StmtDecl* stmt = m_module.newASTNode<StmtDecl>(rit->decl);
                it = list.insertAfter(it, stmt);
                ++it;
            }
            m_stageInfo.clear();
        }
        return it;
    }

    void
    ExprBreakVisitor::visit(DeclModule& node) {
        auto& decls = node.getDeclarations();
        for (auto it = decls.begin(); it != decls.end(); ++it) {
            (*it)->acceptVisitor(*this);
        }
    }

    void
    ExprBreakVisitor::visit(StructMemberInitList& node) {
        for (auto it = node.childBegin(); it != node.childEnd(); ++it) {
            StructMemberInit* memberInit = *it;
            memberInit->acceptVisitor(*this);
            // Possibily only require for non c99 struct init?
            /*StmtExpression* expr = memberInit->getInitExpr();
            if (requiresTmpStorage(*expr)) {
                StmtExpression* replacementExpr = processExpression(expr,m_curScope);
                memberInit->setInitExpr(replacementExpr);
            }*/
        }
    }

    void
    ExprBreakVisitor::visit(DeclStructMembers&) {
    }

    void
    ExprBreakVisitor::visit(ExprList& node) {
        for (auto it = node.childBegin(); it != node.childEnd(); ++it) {
            StmtExpression* expr = *it;
            expr->acceptVisitor(*this);
            if (requiresTmpStorage(*expr)) {
                StmtExpression* replacementExpr = processExpression(expr,m_curScope);
                *it = replacementExpr;
            }
        }
    }

    void
    ExprBreakVisitor::visit(StatementList& node) {
        for (auto it = node.childBegin(); it != node.childEnd(); ++it) {
            (*it)->acceptVisitor(*this);
            it = processQueuedChanges(node, it);
        }
    }

    void
    ExprBreakVisitor::visit(DeclParamVarContainer&) {
    }

    void
    ExprBreakVisitor::visit(DeclFunction& node) {
        m_tmpVarCount = 0;
        pushScope(node.getFunctionDeclScope());
        if (node.getFunctionBody() != nullptr) {
            node.getFunctionBody()->acceptVisitor(*this);
        }
        popScope();
    }

    void
    ExprBreakVisitor::visit(DeclTypeFunction& node) {
        m_tmpVarCount = 0;
        pushScope(node.getFunctionDeclScope());
        if (node.getFunctionBody() != nullptr) {
            node.getFunctionBody()->acceptVisitor(*this);
        }
        popScope();
    }

    void
    ExprBreakVisitor::visit(DeclStruct&) {
    }

    void
    ExprBreakVisitor::visit(DeclVar& node) {
        StmtExpression* initExpr = node.getExpression();
        initExpr->acceptVisitor(*this);
    }

    void
    ExprBreakVisitor::visit(DeclParamVar&) {
        YAL_ASSERT(false);
    }

    void
    ExprBreakVisitor::visit(DeclStrongAlias&) {

    }

    void
    ExprBreakVisitor::visit(DeclWeakAlias&) {

    }

    void
    ExprBreakVisitor::visit(RefType& ) {
        YAL_ASSERT(false);
    }

    void
    ExprBreakVisitor::visit(StmtReturn& node) {
        if (node.getExpression()) {
            node.getExpression()->acceptVisitor(*this);
        }
    }

    void
    ExprBreakVisitor::visit(StmtDecl& node) {
        node.getDecl()->acceptVisitor(*this);
    }

    void
    ExprBreakVisitor::visit(StmtAssign& node) {
        StmtExpression* valueExpr = node.getValueExpr();
        StmtExpression* destExpr = node.getDestExpr();
        destExpr->acceptVisitor(*this);
        valueExpr->acceptVisitor(*this);
    }

    void
    ExprBreakVisitor::visit(ExprVarRef&) {

    }

    void
    ExprBreakVisitor::visit(ExprUnaryOperator& node) {
        node.getExpression()->acceptVisitor(*this);
    }

    void
    ExprBreakVisitor::visit(ExprBinaryOperator& node) {
        node.getExpressionRight()->acceptVisitor(*this);
        node.getExpressionLeft()->acceptVisitor(*this);
    }

    void
    ExprBreakVisitor::visit(ExprBoolLiteral&) {

    }

    void
    ExprBreakVisitor::visit(ExprIntegerLiteral& ) {

    }

    void
    ExprBreakVisitor::visit(ExprFloatLiteral&) {

    }

    void
    ExprBreakVisitor::visit(ExprStructVarRef& node) {
        node.getExpression()->acceptVisitor(*this);
    }

    void
    ExprBreakVisitor::visit(ExprFnCall& node) {
        if (node.getFunctionArgs() != nullptr) {
            node.getFunctionArgs()->acceptVisitor(*this);
        }
    }

    void
    ExprBreakVisitor::visit(ExprTypeFnCall& node) {
        if (node.getFunctionArgs() != nullptr) {
            node.getFunctionArgs()->acceptVisitor(*this);
        }
    }

    void
    ExprBreakVisitor::visit(ExprTypeFnCallStatic& node) {
        if (node.getFunctionArgs() != nullptr) {
            node.getFunctionArgs()->acceptVisitor(*this);
        }
    }

    void
    ExprBreakVisitor::visit(DeclParamVarSelf&) {

    }

    void
    ExprBreakVisitor::visit(ExprVarRefSelf& node) {
        YAL_ASSERT(node.getQualType().getType() != nullptr);
    }

    void
    ExprBreakVisitor::visit(ExprRangeCast& node) {
        node.getExpression()->acceptVisitor(*this);
    }

    void
    ExprBreakVisitor::visit(ExprStructInit& node) {
        if (node.getMemberInitList() != nullptr) {
            node.getMemberInitList()->acceptVisitor(*this);

        }
    }

    void
    ExprBreakVisitor::visit(StructMemberInit& node) {
        node.getInitExpr()->acceptVisitor(*this);
    }

    //----------------------------------------------------------------------

    StageExprListBreakdown::StageExprListBreakdown(Compiler& compiler,
                                                   Module& module):
        m_compiler(compiler),
        m_module(module){
        (void) m_compiler;
    }

    bool
    StageExprListBreakdown::execute(DeclBase* decl) {
        ExprBreakVisitor visitor(m_module);
        decl->acceptVisitor(visitor);
        return !visitor.didError();
    }
}
