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
#include "yal/backendc/ctypebuiltin.h"
#include "yal/backendc/ctypegen.h"
#include "yal/frontend/ast/astvisitorimpl.h"
#include "yal/frontend/module.h"
#include "yal/frontend/types/typebuiltin.h"
#include "yal/frontend/types/typefunction.h"
#include "yal/frontend/types/typestruct.h"

namespace yf = yal::frontend;
namespace yal::backend::c {

    /*
    static const StringRef kTmpVariableList [] {
        "yal_tmp_var_0",
        "yal_tmp_var_1",
        "yal_tmp_var_2",
        "yal_tmp_var_3",
        "yal_tmp_var_4",
        "yal_tmp_var_5",
        "yal_tmp_var_6",
        "yal_tmp_var_7",
        "yal_tmp_var_8",
        "yal_tmp_var_9",
        "yal_tmp_var_10",
        "yal_tmp_var_11",
        "yal_tmp_var_12",
        "yal_tmp_var_13",
        "yal_tmp_var_14",
        "yal_tmp_var_15",
        "yal_tmp_var_16",
        "yal_tmp_var_17",
        "yal_tmp_var_18",
        "yal_tmp_var_19",
        "yal_tmp_var_20",
        "yal_tmp_var_21",
        "yal_tmp_var_22",
        "yal_tmp_var_23",
        "yal_tmp_var_24",
        "yal_tmp_var_25",
        "yal_tmp_var_26",
        "yal_tmp_var_27",
        "yal_tmp_var_28",
        "yal_tmp_var_29",
        "yal_tmp_var_30",
        "yal_tmp_var_31",
        "yal_tmp_var_32",
        "yal_tmp_var_33",
        "yal_tmp_var_34",
        "yal_tmp_var_35",
        "yal_tmp_var_36",
        "yal_tmp_var_37",
        "yal_tmp_var_38",
        "yal_tmp_var_39",
        "yal_tmp_var_40",
        "yal_tmp_var_41",
        "yal_tmp_var_42",
        "yal_tmp_var_43",
        "yal_tmp_var_44",
        "yal_tmp_var_45",
        "yal_tmp_var_46",
        "yal_tmp_var_47",
        "yal_tmp_var_48",
        "yal_tmp_var_49",
    };

    static const unsigned int kTmpVariableListCount
        = sizeof(kTmpVariableList)/ sizeof(StringRef);
*/

    inline static void
    WriteSourceInfo(CodeWriter& writer,
                    const yal::frontend::Module& module,
                    const SourceInfo& info) {
        writer.write("// %.yal:%:%\n",
                     module.getName(),
                     info.begin.line,
                     info.begin.column);
    }

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
    AstVisitorCSource::visit(const yal::frontend::DeclFunction& node) {
        WriteSourceInfo(m_writer, node.getModule(),node.getSourceInfo());
        CTypeGen::GenDelcFunction(m_writer, m_typeCache, node);
        m_writer.write("{");
        m_writer.ident();
        m_writer.write();
        auto& body = node.getFunctionBody();
        for (auto& statment : body) {
            resolve(*statment);
            m_writer.write(";\n");
        }
        m_writer.unident();
        m_writer.write("}\n\n");
    }

    void
    AstVisitorCSource::visit(const yal::frontend::DeclTypeFunction& node) {
        WriteSourceInfo(m_writer, node.getModule(),node.getSourceInfo());
        CTypeGen::GenDelcFunction(m_writer, m_typeCache, node);
        m_writer.write("{");
        m_writer.ident();
        m_writer.write();
        auto& body = node.getFunctionBody();
        for (auto& statment : body) {
            resolve(*statment);
            m_writer.write(";\n");
        }
        m_writer.unident();
        m_writer.write("}\n\n");
    }

    void
    AstVisitorCSource::visit(const yal::frontend::DeclTypeFunctions& node) {
        for (auto& decl : node.getDecls()) {
            resolve(*decl);
        }
    }

    void
    AstVisitorCSource::visit(const yal::frontend::DeclStruct&) {
        // do nothing
    }

    void
    AstVisitorCSource::visit(const yal::frontend::DeclVar& node) {
        const yf::QualType qt = node.getQualType();
        const CType* ctype = m_typeCache.getCType(*qt.getType());
        YAL_ASSERT(ctype != nullptr);
        CTypeGen::GenQualType(m_writer, qt, *ctype);
        m_writer.write("%", node.getName());
        auto exprOpt = node.getExpression();
        if (exprOpt.has_value()) {
            m_writer.write(" = ");
           resolve(*exprOpt.value());
        }
    }


    void
    AstVisitorCSource::visit(const yal::frontend::DeclModule& node) {
        auto& decls = node.getDeclarations();
        for (auto& decl : decls) {
            resolve(*decl);
        }
    }

    void
    AstVisitorCSource::visit(const yal::frontend::DeclParamVar&) {
        YAL_ASSERT(false);
    }

    void
    AstVisitorCSource::visit(const yal::frontend::DeclParamVarSelf&) {
        YAL_ASSERT(false);
    }

    void
    AstVisitorCSource::visit(const yal::frontend::DeclAliasStrong&) {

    }

    void
    AstVisitorCSource::visit(const yal::frontend::DeclAliasWeak&) {

    }

    void
    AstVisitorCSource::visit(const yal::frontend::StmtReturn& node) {
        auto exprOpt = node.getExpression();
        if (exprOpt.has_value()) {
            m_writer.write("return ");
            resolve(*exprOpt.value());
        } else {
            m_writer.write("return");
        }
    }

    void
    AstVisitorCSource::visit(const yal::frontend::StmtDecl& node) {
        resolve(*node.getDecl());
    }

    void
    AstVisitorCSource::visit(const yal::frontend::StmtAssign& node) {
        resolve(*node.getLeftExpr());
        m_writer.write(" = ");
        resolve(*node.getRightExpr());
    }

    void
    AstVisitorCSource::visit(const yal::frontend::ExprVarRef& node) {
        const yf::DeclVar& decl =node.getDeclVar();
        const bool isDeclParam = decl.isDeclParam();

        if (m_state.varRefMakeReference) {
            const yf::QualType qt = node.getQualType();
            if (!qt.isReference()
                    && !isDeclParam) {
                m_writer.write("&");
            }
        }

        const bool shouldDeref = !m_state.varRefMakeReference
                && isDeclParam
                && decl.getQualType().isMovedType();
        if (shouldDeref) {
            m_writer.write("(*%)", decl.getName());
        } else {
            m_writer.write("%", decl.getName());
        }
    }

    void
    AstVisitorCSource::visit(const yal::frontend::ExprVarRefSelf& node) {
        const yf::ExprVarRef& varRef = node;
        visit(varRef);
    }

    void
    AstVisitorCSource::visit(const yal::frontend::ExprStructVarRef& node) {
        yf::StmtExpression* srcExpr = node.getExpression();
        resolve(*srcExpr);
        const yf::QualType qt = srcExpr->getQualType();
        m_writer.write("%%",
                       (qt.isReference()) ? "->" : ".",
                       node.getMemberName()
                       );
    }

    void
    AstVisitorCSource::visit(const yal::frontend::ExprUnaryOperator& node) {
        State newState = m_state;
        // Do not write reference operator since it is handled when
        // variables are being referenced.
        const yf::UnaryOperatorType op = node.getOperatorType();
        if (op != yf::UnaryOperatorType::Reference) {
            m_writer.write(CTypeGen::GenUnaryOperator(op));
        } else {
            newState.varRefMakeReference = true;
        }

        ScopedState guard(*this, newState);
        resolve(*node.getExpression());
    }

    void
    AstVisitorCSource::visit(const yal::frontend::ExprBinaryOperator& node) {
        const yf::StmtExpression* exprLeft = node.getExpressionLeft();
        const yf::StmtExpression* exprRight = node.getExpressionRight();
        resolve(*exprLeft);
        m_writer.write(" ");
        m_writer.write(CTypeGen::GenBinaryOperator(node.getOperatorType()));
        m_writer.write(" ");

        // cast type to left type when types don't match
        const yf::QualType qtLeft = exprLeft->getQualType();
        const yf::QualType qtRight = exprRight->getQualType();
        if (qtLeft.getType() != qtRight.getType()) {
            const CType* ctypeLeft = m_typeCache.getCType(*qtLeft.getType());
            YAL_ASSERT(ctypeLeft != nullptr);
            m_writer.write("(");
            CTypeGen::GenQualType(m_writer, qtLeft, *ctypeLeft);
            m_writer.write(")");
        }

        resolve(*exprRight);
    }

    void
    AstVisitorCSource::visit(const yal::frontend::ExprBoolLiteral& node) {
        if (node.getLiteralValue()) {
            m_writer.write("YAL_TRUE");
        } else {
            m_writer.write("YAL_FALSE");
        }
    }

    void
    AstVisitorCSource::visit(const yal::frontend::ExprIntegerLiteral& node) {
        switch(node.getIntegerType()) {
        case yf::IntegerType::I8:
            m_writer.write("%", node.getValueAsI8());
            break;
        case yf::IntegerType::U8:
            m_writer.write("%", node.getValueAsU8());
            break;
        case yf::IntegerType::I16:
            m_writer.write("%", node.getValueAsI16());
            break;
        case yf::IntegerType::U16:
            m_writer.write("%", node.getValueAsU16());
            break;
        case yf::IntegerType::I32:
            m_writer.write("%", node.getValueAsI32());
            break;
        case yf::IntegerType::U32:
            m_writer.write("%", node.getValueAsU32());
            break;
        case yf::IntegerType::I64:
            m_writer.write("%", node.getValueAsI64());
            break;
        case yf::IntegerType::U64:
            m_writer.write("%", node.getValueAsU64());
            break;
        default:
            YAL_ASSERT_MESSAGE(false, "Unknown integer type");
            m_writer.write("Unknown");
            break;
        }
    }

    void
    AstVisitorCSource::visit(const yal::frontend::ExprFloatLiteral& node) {
        if (node.getDecimalType() == yf::DecimalType::Decimal32) {
            m_writer.write( "%", node.getLiteralValueAsF32());
        } else {
            m_writer.write("%", node.getLiteralValueAsF64());
        }
    }

    void
    AstVisitorCSource::visit(const yal::frontend::ExprFnCall& node) {
        State newState;
        newState.varRefMakeReference = true;
        ScopedState guard(*this, newState);
        const yf::TypeFunction* typeFn = node.getFunctionType();
        const CType* ctypeFn = m_typeCache.getCType(*typeFn);
        YAL_ASSERT(ctypeFn != nullptr);

        m_writer.write(ctypeFn->getCIdentifier());
        m_writer.write("(");

        const auto& args = node.getFunctionArgs();
        const yf::DeclFunction& declFn = typeFn->getDecl();
        const yf::DeclFunction::Params& declParams = declFn.getParams();
        // Apply special conversion rules for moved types
        auto declParamIter = declParams.begin();
        bool first = true;
        for (auto& arg : args) {
            YAL_ASSERT(declParamIter != declParams.end());
            if (!first) {
                m_writer.write(", ");
            }
            const yf::QualType qtDeclParam = (*declParamIter)->getQualType();
            if (!qtDeclParam.isReference() && !qtDeclParam.isTriviallyCopiable()) {
                m_writer.write("&(");
                State otherState;
                otherState.varRefMakeReference = false;
                ScopedState guard(*this, otherState);
                resolve(*arg);
                m_writer.write(")");
            } else {
                resolve(*arg);
            }
            first = false;
            ++declParamIter;
        }
        m_writer.write(")");
    }

    void
    AstVisitorCSource::visit(const yal::frontend::ExprTypeFnCall& node) {
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
            resolve(*exprOpt.value());
            first = false;
        }

        const yf::DeclFunction& declFn = typeFn->getDecl();
        const yf::DeclFunction::Params& declParams = declFn.getParams();
        // Apply special conversion rules for moved types
        auto declParamIter = declParams.begin();
        if (!node.isStaticCall()) {
            declParamIter++;
        }
        const auto& args = node.getFunctionArgs();
        for (auto& arg : args) {
            YAL_ASSERT(declParamIter != declParams.end());
            if (!first) {
                m_writer.write(", ");
            }
            const yf::QualType qtDeclParam = (*declParamIter)->getQualType();
            if (!qtDeclParam.isReference() && !qtDeclParam.isTriviallyCopiable()) {
                m_writer.write("&(");
                State otherState;
                otherState.varRefMakeReference = false;
                ScopedState guard(*this, otherState);
                resolve(*arg);
                m_writer.write(")");
            } else {
                resolve(*arg);
            }
            first = false;
            ++declParamIter;
        }
        m_writer.write(")");
    }

    void
    AstVisitorCSource::visit(const yal::frontend::ExprCast& node) {
        const yf::QualType qtExpr = node.getExpression().getQualType();
        const yf::QualType qtTarget = node.getQualType();
        const CType* ctypeTarget = m_typeCache.getCType(*qtTarget.getType());
        YAL_ASSERT(ctypeTarget != nullptr);

        bool doNomralConversion = true;
        if(qtTarget.getType()->isBuilitin() && qtExpr.getType()->isBuilitin()) {
            const yf::TypeBuiltin* typeBtTarget = dyn_cast<yf::TypeBuiltin>(qtTarget.getType());
            const yf::TypeBuiltin* typeBtExpr = dyn_cast<yf::TypeBuiltin>(qtExpr.getType());
            if (CTypeBuilitin::GenConversion(m_writer, *typeBtExpr, *typeBtTarget)) {
                m_writer.write("(");
                resolve(node.getExpression());
                m_writer.write(")");
                doNomralConversion = false;
            }
        }

        if (doNomralConversion) {
            m_writer.write("((");
            CTypeGen::GenQualType(m_writer, qtTarget, *ctypeTarget);
            m_writer.write(")");
            resolve(node.getExpression());
            m_writer.write(")");
        }
    }

    void
    AstVisitorCSource::visit(const yal::frontend::ExprStructInit& node) {
        const yf::QualType qtStruct = node.getQualType();
        const CType* ctypeStruct = m_typeCache.getCType(*qtStruct.getType());
        YAL_ASSERT(ctypeStruct != nullptr);

        m_writer.write("(%){\n", ctypeStruct->getCIdentifier());
        m_writer.ident();

        bool isFirst = true;
        auto& memberInits = node.getMemberInitExprList();
        for (auto& memberInit : memberInits) {
            if (!isFirst) {
                m_writer.write("\n,");
            }
            m_writer.write(".% = ", memberInit->getMemberName());
            resolve(*memberInit->getInitExpr());
            isFirst = false;
        }

        m_writer.unident();
        m_writer.write("\n}");
    }

    void AstVisitorCSource::visit(const yal::frontend::StmtListScoped& node) {
        m_writer.write("\n{");
        m_writer.ident();

        for (auto& stmt : node.getStatements()) {
            resolve(*stmt);
            m_writer.write(";\n");
        }

        m_writer.unident();
        m_writer.write("}");
    }

    bool
    AstVisitorCSource::execute(const BackendOptions& options,
                               const StringRef headerPath) {
        (void) options;
        m_writer.write("// Auto Generated with YAL Backend C %, do not modify!\n",
                       YAL_BACKEND_C_VERSION_STR);
        m_writer.write("// Source for module %\n\n", m_module.getPath());
        m_writer.write("#include \"%\"\n\n", headerPath);

        //TODO: Move this to a builtin module that can
        // be generated when projects have been setup
        CTypeBuilitin::GenBuiltinSource(m_writer);

        resolve(*m_module.getDeclNode());

        return true;
    }
}
