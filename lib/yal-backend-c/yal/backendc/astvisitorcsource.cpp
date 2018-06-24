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
#include "yal/frontend/ast/astnodes.h"
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
    AstVisitorCSource::visit(yal::frontend::DeclFunction& node) {
        WriteSourceInfo(m_writer, node.getModule(),node.getSourceInfo());
        CTypeGen::GenDelcFunction(m_writer, m_typeCache, node);
        m_writer.write("{");
        m_writer.ident();
        m_writer.write();
        auto& body = node.getFunctionBody();
        for (auto& statment : body) {
            statment->acceptVisitor(*this);
            m_writer.write(";\n");
        }
        m_writer.unident();
        m_writer.write("}\n\n");
    }

    void
    AstVisitorCSource::visit(yal::frontend::DeclTypeFunction& node) {
        WriteSourceInfo(m_writer, node.getModule(),node.getSourceInfo());
        CTypeGen::GenDelcFunction(m_writer, m_typeCache, node);
        m_writer.write("{");
        m_writer.ident();
        m_writer.write();
        auto& body = node.getFunctionBody();
        for (auto& statment : body) {
            statment->acceptVisitor(*this);
            m_writer.write(";\n");
        }
        m_writer.unident();
        m_writer.write("}\n\n");
    }

    void
    AstVisitorCSource::visit(yal::frontend::DeclStruct&) {
        // do nothing
    }

    void
    AstVisitorCSource::visit(yal::frontend::DeclVar& node) {
        const yf::QualType qt = node.getQualType();
        const CType* ctype = m_typeCache.getCType(*qt.getType());
        YAL_ASSERT(ctype != nullptr);
        CTypeGen::GenQualType(m_writer, qt, *ctype);
        m_writer.write("%", node.getName());
        auto exprOpt = node.getExpression();
        if (exprOpt.has_value()) {
            m_writer.write(" = ");
            exprOpt.value()->acceptVisitor(*this);
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
        auto exprOpt = node.getExpression();
        if (exprOpt.has_value()) {
            m_writer.write("return ");
            exprOpt.value()->acceptVisitor(*this);
        } else {
            m_writer.write("return");
        }
    }

    void
    AstVisitorCSource::visit(yal::frontend::StmtDecl& node) {
        node.getDecl()->acceptVisitor(*this);
    }

    void
    AstVisitorCSource::visit(yal::frontend::StmtAssign& node) {
        node.getLeftExpr()->acceptVisitor(*this);
        m_writer.write(" = ");
        node.getRightExpr()->acceptVisitor(*this);
    }

    void
    AstVisitorCSource::visit(yal::frontend::ExprVarRef& node) {
        const yf::DeclVar& decl =node.getDeclVar();
        const bool isMovedDeclParm =
                decl.getAstType() == yf::AstType::DeclParamVar
                || decl.getAstType() == yf::AstType::DeclParamVarSelf;

        //TODO:: improve!!!
        if (m_state.varRefMakeReference) {
            const yf::QualType qt = node.getQualType();
            if (!qt.isReference()
                    && !qt.isTriviallyCopiable()
                    && !isMovedDeclParm) {
                m_writer.write("&");
            }
        }

        const bool shouldDeref = isMovedDeclParm
                && decl.getQualType().isMovedType()
                && !m_state.varRefMakeReference;
        if (shouldDeref) {
            m_writer.write("(*%)", decl.getName());
        } else {
            m_writer.write("%", decl.getName());
        }
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
        m_writer.write("%%",
                       (qt.isReference()) ? "->" : ".",
                       node.getMemberName()
                       );
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
            m_writer.write("(");
            CTypeGen::GenQualType(m_writer, qtLeft, *ctypeLeft);
            m_writer.write(")");
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
    AstVisitorCSource::visit(yal::frontend::ExprFloatLiteral& node) {
        if (node.getDecimalType() == yf::DecimalType::Decimal32) {
            m_writer.write( "%", node.getLiteralValueAsF32());
        } else {
            m_writer.write("%", node.getLiteralValueAsF64());
        }
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
                arg->acceptVisitor(*this);
                m_writer.write(")");
            } else {
                arg->acceptVisitor(*this);
            }
            first = false;
            ++declParamIter;
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
                arg->acceptVisitor(*this);
                m_writer.write(")");
            } else {
                arg->acceptVisitor(*this);
            }
            first = false;
            ++declParamIter;
        }
        m_writer.write(")");
    }

    void
    AstVisitorCSource::visit(yal::frontend::ExprCast& node) {
        const yf::QualType qtExpr = node.getExpression().getQualType();
        const yf::QualType qtTarget = node.getQualType();
        const CType* ctypeTarget = m_typeCache.getCType(*qtTarget.getType());
        const CType* ctypeExpr= m_typeCache.getCType(*qtExpr.getType());
        YAL_ASSERT(ctypeTarget != nullptr && ctypeExpr != nullptr);

        bool doNomralConversion = true;
        if(qtTarget.getType()->isBuilitin() && qtExpr.getType()->isBuilitin()) {
            const yf::TypeBuiltin* typeBtTarget = dyn_cast<yf::TypeBuiltin>(qtTarget.getType());
            const yf::TypeBuiltin* typeBtExpr = dyn_cast<yf::TypeBuiltin>(qtExpr.getType());
            if (CTypeBuilitin::GenConversion(m_writer, *typeBtExpr, *typeBtTarget)) {
                m_writer.write("(");
                node.getExpression().acceptVisitor(*this);
                m_writer.write(")");
                doNomralConversion = false;
            }
        }

        if (doNomralConversion) {
            m_writer.write("((");
            CTypeGen::GenQualType(m_writer, qtTarget, *ctypeTarget);
            m_writer.write(")");
            node.getExpression().acceptVisitor(*this);
            m_writer.write(")");
        }
    }

    void
    AstVisitorCSource::visit(yal::frontend::ExprStructInit& node) {
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
            memberInit->acceptVisitor(*this);
            isFirst = false;
        }

        m_writer.unident();
        m_writer.write("\n}");
    }

    void
    AstVisitorCSource::visit(yal::frontend::StructMemberInit& node) {
        m_writer.write(".% = ", node.getMemberName());
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
        m_writer.write("// Auto Generated with YAL Backend C %, do not modify!\n",
                       YAL_BACKEND_C_VERSION_STR);
        m_writer.write("// Source for module %\n\n", m_module.getPath());
        m_writer.write("#include \"%\"\n\n", headerPath);

        //TODO: Move this to a builtin module that can
        // be generated when projects have been setup
        CTypeBuilitin::GenBuiltinSource(m_writer);

        m_module.getDeclNode()->acceptVisitor(*this);

        return true;
    }
}
