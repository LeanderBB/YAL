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


#include "yal/backendc/astvisitorcsource.h"
#include "yal/backendc/backendc.h"
#include "yal/backendc/ctype.h"
#include "yal/backendc/ctypegen.h"
#include "yal/frontend/ast/astnodes.h"
#include "yal/frontend/module.h"
#include "yal/frontend/types/typefunction.h"
#include "yal/frontend/types/typestruct.h"

namespace yf = yal::frontend;
namespace yal::backend::c {

    struct AstVisitorCSource::ScopedState {

        ScopedState(AstVisitorCSource& visitor,
                    const AstVisitorCSource::State& state):
            m_visitor(visitor),
            m_prevState(visitor.m_state) {
            m_visitor.m_state = state;
        }

        ~ScopedState() {
            m_visitor.m_state = m_prevState;
        }

        AstVisitorCSource& m_visitor;
        const AstVisitorCSource::State m_prevState;
    };

    AstVisitorCSource::AstVisitorCSource(ByteStream& stream,
                                         yal::frontend::Module& module,
                                         const CTypeCache& cache):
        m_writer(stream),
        m_module(module),
        m_typeCache(cache) {

    }

    void
    AstVisitorCSource::visit(yal::frontend::DeclFunction& node) {
        CTypeGen::GenDelcFunction(m_writer, m_typeCache, node);
        Formater& formater = m_writer.getFormater();
        Format(formater, " {\n\n");
        m_writer.write(formater);
        m_writer.ident();
        auto& body = node.getFunctionBody();
        for (auto& statment : body) {
            statment->acceptVisitor(*this);
            Format(formater, ";\n");
            m_writer.write(formater);
        }
        m_writer.unident();
        Format(formater, "\n}\n\n");
        m_writer.write(formater);
    }

    void
    AstVisitorCSource::visit(yal::frontend::DeclTypeFunction& node) {
        CTypeGen::GenDelcFunction(m_writer, m_typeCache, node);
        Formater& formater = m_writer.getFormater();
        Format(formater, " {\n\n");
        m_writer.write(formater);
        m_writer.ident();
        auto& body = node.getFunctionBody();
        for (auto& statment : body) {
            statment->acceptVisitor(*this);
            Format(formater, ";\n");
            m_writer.write(formater);
        }
        m_writer.unident();
        Format(formater, "\n}\n\n");
        m_writer.write(formater);
    }

    void
    AstVisitorCSource::visit(yal::frontend::DeclStruct&) {
        // do nothing
    }

    void
    AstVisitorCSource::visit(yal::frontend::DeclVar& node) {
        Formater& formater = m_writer.getFormater();
        FormatReset(formater);

        const yf::QualType qt = node.getQualType();
        const CType* ctype = m_typeCache.getCType(*qt.getType());
        YAL_ASSERT(ctype != nullptr);
        CTypeGen::GenQualType(formater, qt, *ctype);
        FormatAppend(formater, "%", node.getName());
        auto exprOpt = node.getExpression();
        if (exprOpt.has_value()) {
            FormatAppend(formater, " = ");
            m_writer.write(formater);
            exprOpt.value()->acceptVisitor(*this);
        } else {
            m_writer.write(formater);
        }
    }


    void
    AstVisitorCSource::visit(yal::frontend::DeclModule& node) {
        auto& decls = node.getDeclarations();
        for (auto& decl : decls) {
            decl->acceptVisitor(*this);
        }
    }

    void
    AstVisitorCSource::visit(yal::frontend::DeclParamVar&) {
        YAL_ASSERT(false);
    }

    void
    AstVisitorCSource::visit(yal::frontend::DeclParamVarSelf&) {
        YAL_ASSERT(false);
    }

    void
    AstVisitorCSource::visit(yal::frontend::DeclStrongAlias&) {

    }

    void
    AstVisitorCSource::visit(yal::frontend::DeclWeakAlias&) {

    }

    void
    AstVisitorCSource::visit(yal::frontend::StmtReturn& node) {
        Formater& formater = m_writer.getFormater();
        auto exprOpt = node.getExpression();
        if (exprOpt.has_value()) {
            Format(formater, "return ");
            m_writer.write(formater);
            exprOpt.value()->acceptVisitor(*this);
        } else {
            Format(formater, "return");
            m_writer.write(formater);
        }
    }

    void
    AstVisitorCSource::visit(yal::frontend::StmtDecl& node) {
        node.getDecl()->acceptVisitor(*this);
    }

    void
    AstVisitorCSource::visit(yal::frontend::StmtAssign& node) {
        node.getLeftExpr()->acceptVisitor(*this);
        Formater& formater = m_writer.getFormater();
        Format(formater, " = ");
        m_writer.write(formater);
        node.getRightExpr()->acceptVisitor(*this);
    }

    void
    AstVisitorCSource::visit(yal::frontend::ExprVarRef& node) {
        const yf::DeclVar& decl =node.getDeclVar();
        const bool isMovedDeclParm =
                decl.getAstType() == yf::AstType::DeclParamVar
                || decl.getAstType() == yf::AstType::DeclParamVarSelf;

        //TODO:: improve!!!
        Formater& formater = m_writer.getFormater();
        if (m_state.varRefMakeReference) {
            const yf::QualType qt = node.getQualType();
            if (!qt.isReference()
                    && !qt.isTriviallyCopiable()
                    && !isMovedDeclParm) {
                FormatAppend(formater, "&");
            }
        }

        if (isMovedDeclParm
                && decl.getQualType().isMovedType()
                && !m_state.varRefMakeReference) {
            FormatAppend(formater, "*");
        }
        FormatAppend(formater, "%", decl.getName());
        m_writer.write(formater);
    }

    void
    AstVisitorCSource::visit(yal::frontend::ExprVarRefSelf& node) {
        yf::ExprVarRef& varRef = node;
        visit(varRef);
    }

    void
    AstVisitorCSource::visit(yal::frontend::ExprStructVarRef& node) {
        yf::StmtExpression* srcExpr = node.getExpression();
        srcExpr->acceptVisitor(*this);
        const yf::QualType qt = srcExpr->getQualType();
        Formater& formater = m_writer.getFormater();
        Format(formater, "%%",
               (qt.isReference()) ? "->" : ".",
               node.getMemberName()
               );
        m_writer.write(formater);
    }

    void
    AstVisitorCSource::visit(yal::frontend::ExprUnaryOperator& node) {
        // Do not write reference operator since it is handled when
        // variables are being referenced.
        const yf::UnaryOperatorType op = node.getOperatorType();
        if (op!= yf::UnaryOperatorType::Reference) {
            m_writer.write(CTypeGen::GenUnaryOperator(op));
        }

        State newState;
        if (node.getOperatorType() == yf::UnaryOperatorType::Reference) {
            newState.varRefMakeReference = true;
        }

        ScopedState guard(*this, newState);

        node.getExpression()->acceptVisitor(*this);
    }

    void
    AstVisitorCSource::visit(yal::frontend::ExprBinaryOperator& node) {
        yf::StmtExpression* exprLeft = node.getExpressionLeft();
        yf::StmtExpression* exprRight = node.getExpressionRight();
        exprLeft->acceptVisitor(*this);
        m_writer.write(" ");
        m_writer.write(CTypeGen::GenBinaryOperator(node.getOperatorType()));
        m_writer.write(" ");

        // cast type to left type when types don't match
        const yf::QualType qtLeft = exprLeft->getQualType();
        const yf::QualType qtRight = exprRight->getQualType();
        if (qtLeft.getType() != qtRight.getType()) {
            const CType* ctypeLeft = m_typeCache.getCType(*qtLeft.getType());
            YAL_ASSERT(ctypeLeft != nullptr);
            Formater& formater = m_writer.getFormater();
            Format(formater, "(");
            CTypeGen::GenQualType(formater,qtLeft, *ctypeLeft);
            FormatAppend(formater, ")");
            m_writer.write(formater);
        }

        exprRight->acceptVisitor(*this);
    }

    void
    AstVisitorCSource::visit(yal::frontend::ExprBoolLiteral& node) {
        if (node.getLiteralValue()) {
            m_writer.write("YAL_TRUE");
        } else {
            m_writer.write("YAL_FALSE");
        }
    }

    void
    AstVisitorCSource::visit(yal::frontend::ExprIntegerLiteral& node) {
        Formater& formater = m_writer.getFormater();
        switch(node.getIntegerType()) {
        case yf::IntegerType::I8:
            Format(formater, "%", node.getValueAsI8());
            break;
        case yf::IntegerType::U8:
            Format(formater, "%", node.getValueAsU8());
            break;
        case yf::IntegerType::I16:
            Format(formater, "%", node.getValueAsI16());
            break;
        case yf::IntegerType::U16:
            Format(formater, "%", node.getValueAsU16());
            break;
        case yf::IntegerType::I32:
            Format(formater, "%", node.getValueAsI32());
            break;
        case yf::IntegerType::U32:
            Format(formater, "%", node.getValueAsU32());
            break;
        case yf::IntegerType::I64:
            Format(formater, "%", node.getValueAsI64());
            break;
        case yf::IntegerType::U64:
            Format(formater, "%", node.getValueAsU64());
            break;
        default:
            YAL_ASSERT_MESSAGE(false, "Unknown integer type");
            Format(formater, "Unknown");
            break;
        }
        m_writer.write(formater);
    }

    void
    AstVisitorCSource::visit(yal::frontend::ExprFloatLiteral& node) {
        Formater& formater = m_writer.getFormater();
        if (node.getDecimalType() == yf::DecimalType::Decimal32) {
            Format(formater, "%", node.getLiteralValueAsF32());
        } else {
             Format(formater, "%", node.getLiteralValueAsF64());
        }
        m_writer.write(formater);
    }

    void
    AstVisitorCSource::visit(yal::frontend::ExprFnCall& node) {
        State newState;
        newState.varRefMakeReference = true;
        ScopedState guard(*this, newState);
        const yf::TypeFunction* typeFn = node.getFunctionType();
        const CType* ctypeFn = m_typeCache.getCType(*typeFn);
        YAL_ASSERT(ctypeFn != nullptr);

        m_writer.write(ctypeFn->getCIdentifier());
        m_writer.write("(");
        const auto& args = node.getFunctionArgs();
        bool first = true;
        for (auto& arg : args) {
            if (!first) {
                m_writer.write(" ,");
            }
            arg->acceptVisitor(*this);
            first = false;
        }
        m_writer.write(")");
    }

    void
    AstVisitorCSource::visit(yal::frontend::ExprTypeFnCall& node) {
        State newState;
        newState.varRefMakeReference = true;
        ScopedState guard(*this, newState);
        const yf::TypeFunction* typeFn = node.getFunctionType();
        const CType* ctypeFn = m_typeCache.getCType(*typeFn);
        YAL_ASSERT(ctypeFn != nullptr);

        bool first = true;
        m_writer.write(ctypeFn->getCIdentifier());
        m_writer.write("(");

        // if not static, generate code for expression
        if (!node.isStaticCall()) {
            auto exprOpt = node.getExpression();
            YAL_ASSERT(exprOpt.has_value());
            exprOpt.value()->acceptVisitor(*this);
            first = false;
        }

        const auto& args = node.getFunctionArgs();
        for (auto& arg : args) {
            if (!first) {
                m_writer.write(" ,");
            }
            arg->acceptVisitor(*this);
            first = false;
        }
        m_writer.write(")");
    }

    void
    AstVisitorCSource::visit(yal::frontend::ExprRangeCast&) {
        YAL_ASSERT_MESSAGE(false, "Deprecated, do not use!!!");
    }

    void
    AstVisitorCSource::visit(yal::frontend::ExprStructInit& node) {
        const yf::QualType qtStruct = node.getQualType();
        const CType* ctypeStruct = m_typeCache.getCType(*qtStruct.getType());
        YAL_ASSERT(ctypeStruct != nullptr);

        Formater& formater = m_writer.getFormater();
        Format(formater,"(%){\n", ctypeStruct->getCIdentifier());
        m_writer.write(formater);
        m_writer.ident();

        bool isFirst = true;
        auto& memberInits = node.getMemberInitExprList();
        for (auto& memberInit : memberInits) {
            if (!isFirst) {
                m_writer.write("\n,");
            }
            memberInit->acceptVisitor(*this);
            isFirst = false;
        }

        m_writer.unident();
        m_writer.write("}");
    }

    void
    AstVisitorCSource::visit(yal::frontend::StructMemberInit& node) {
        Formater& formater = m_writer.getFormater();
        Format(formater,".% = ", node.getMemberName());
        m_writer.write(formater);
        node.getInitExpr()->acceptVisitor(*this);
    }

    void AstVisitorCSource::visit(yal::frontend::StmtListScoped& node) {
        m_writer.write("\n{");
        m_writer.ident();

        for (auto& stmt : node.getStatements()) {
            stmt->acceptVisitor(*this);
            m_writer.write(";\n");
        }

        m_writer.unident();
        m_writer.write("}");
    }

    bool
    AstVisitorCSource::execute(const BackendOptions& options,
                               const StringRef headerPath) {
        (void) options;
        Formater& formater = m_writer.getFormater();
        Format(formater, "// Auto Generated with YAL Backend C %, do not modify!\n",
               YAL_BACKEND_C_VERSION_STR);
        FormatAppend(formater, "// Source for module %\n\n", m_module.getPath());
        FormatAppend(formater, "#include \"%\"\n\n", headerPath);
        m_writer.write(formater);

        m_module.getDeclNode()->acceptVisitor(*this);

        return true;
    }
}
