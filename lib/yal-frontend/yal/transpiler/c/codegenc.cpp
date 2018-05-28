/*
 *  Copyright 2017 by Leander Beernaert (lbb-dev@pm.me)
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

#include "yal/transpiler/c/codegenc.h"
#include "yal/transpiler/c/gentypesc.h"
#include "yal/ast/astnodes.h"
#include "yal/io/bytestream.h"
#include <algorithm>
namespace yal {

    class CodeGenC::VisitorHeader : public AstVisitor {
    public:

        VisitorHeader(CodeWriter& stream):
            m_writer(stream){
        }

        virtual void visit(DeclStruct& node) override {
            GenTypesC::GenDeclStruct(m_writer,node);
        }

        virtual void visit(DeclFunction& node) override {
            GenTypesC::GenDeclFunctionIntro(m_writer, node);
            m_writer.write(";\n\n");
        }

        virtual void visit(DeclTypeFunction& node) override {
            GenTypesC::GenDeclFunctionIntro(m_writer, node);
            m_writer.write(";\n\n");
        }

    private:
        CodeWriter& m_writer;
    };

    class CodeGenC::VisitorSource : public AstVisitor {
    public:

        VisitorSource(const CodeGenCConfig& config,
                      CodeWriter& writer):
            m_writer(writer),
            m_config(config),
            m_varRefMkReference(false) {

        }

        void
        visit(DeclFunction& node) override {
            GenTypesC::GenDeclFunctionIntro(m_writer, node);
            m_writer.write(" {\n");
            m_writer.ident();
            StatementList* body = node.getFunctionBody();
            if (body != nullptr) {
                body->acceptVisitor(*this);
            }
            m_writer.uindent();
            m_writer.write("}\n\n");
        }

        void
        visit(DeclTypeFunction& node) override {
            GenTypesC::GenDeclFunctionIntro(m_writer, node);
            m_writer.write(" {\n");
            m_writer.ident();
            StatementList* body = node.getFunctionBody();
            if (body != nullptr) {
                body->acceptVisitor(*this);
            }
            m_writer.uindent();
            m_writer.write("}\n\n");
        }

        void
        visit(DeclStruct&) override {
            // do nothing
        }

        void
        visit(DeclStructMembers&) override {
            // do nothing
            YAL_ASSERT(false);
        }

        void
        visit(DeclVar& node) override {
            GenTypesC::GenFromQualType(m_writer, node.getQualType());
            m_writer.write("%", node.getName());
            if (node.getExpression() != nullptr) {
                m_writer.write(" = ");
                node.getExpression()->acceptVisitor(*this);
            }
        }


        void
        visit(DeclModule& node) override {
            auto& decls = node.getDeclarations();
            for (auto it = decls.begin(); it != decls.end(); ++it) {
                (*it)->acceptVisitor(*this);
            }
        }

        void
        visit(DeclParamVar&) override {
            YAL_ASSERT(false);
        }

        void
        visit(DeclParamVarSelf&) override {
            YAL_ASSERT(false);
        }


        void
        visit(DeclParamVarContainer&) override {
            YAL_ASSERT(false);
        }

        void
        visit(ExprList& node) override {
            bool first = true;
            m_writer.ident();
            for(auto& expr : node.getChildRange()) {
                if (!first) {
                    m_writer.write("\n,");
                }
                expr->acceptVisitor(*this);
                first = false;
            }
            m_writer.uindent();
        }

        void
        visit(DeclStrongAlias&) override {

        }

        void
        visit(DeclWeakAlias&) override {

        }

        void
        visit(RefType&) override {
            YAL_ASSERT(false);
        }


        void
        visit(StatementList& node) override {
            for (auto& stmt : node.getChildRangeConst()) {
                stmt->acceptVisitor(*this);
                m_writer.write(";\n");
            }
        }

        void
        visit(StmtReturn& node) override {
            StmtExpression* expr = node.getExpression();
            if (expr != nullptr) {
                m_writer.write("return ");
                expr->acceptVisitor(*this);
            } else {
                m_writer.write("return");
            }
        }

        void
        visit(StmtDecl& node) override {
            node.getDecl()->acceptVisitor(*this);
        }

        void
        visit(StmtAssign& node) override {
            node.getDestExpr()->acceptVisitor(*this);
            m_writer.write(" = ");
            node.getValueExpr()->acceptVisitor(*this);
        }

        void
        visit(ExprVarRef& node) override {
            const DeclParamVar* decl = dyn_cast<DeclParamVar>(node.getDeclVar());
            const bool isMovedDeclParm = decl != nullptr;

            if (m_varRefMkReference) {
                const QualType qt = node.getQualType();
                if (!qt.isReference()
                        && !qt.isTriviallyCopiable()
                        && !isMovedDeclParm) {
                    m_writer.write("&");
                }
            }


            if (isMovedDeclParm
                    && decl->getQualType().isMovedType()
                    && !m_varRefMkReference) {
                    m_writer.write("*");
            }

            m_writer.write("%", node.getDeclVar()->getName());
        }

        void
        visit(ExprVarRefSelf& node) override {
            m_writer.write("%", node.getDeclVar()->getName());
        }

        void
        visit(ExprStructVarRef& node) override {
            StmtExpression* srcExpr = node.getExpression();
            srcExpr->acceptVisitor(*this);
            const QualType qt = srcExpr->getQualType();
            m_writer.write("%%",
                           (qt.getQualifier().isReference()) ? "->" : ".",
                           node.getMemberName()
                           );
        }

        void
        visit(ExprUnaryOperator& node) override {
            // Do not write reference operator since it is handled when
            // variables are being referenced.
            if (node.getOperatorType() != UnaryOperatorType::Reference) {
                GenTypesC::GenUnaryOperator(m_writer,node.getOperatorType());
            }

            const bool varRefMkReferenceVal = m_varRefMkReference;

            if (node.getOperatorType() == UnaryOperatorType::Reference) {
                m_varRefMkReference = true;
            }

            node.getExpression()->acceptVisitor(*this);

            if (node.getOperatorType() == UnaryOperatorType::Reference) {
                m_varRefMkReference = varRefMkReferenceVal;
            }
        }

        void
        visit(ExprBinaryOperator& node) override {
            node.getExpressionLeft()->acceptVisitor(*this);
            m_writer.write(" ");
            GenTypesC::GenBinaryOperator(m_writer,node.getOperatorType());
            m_writer.write(" ");

            // cast type to left type when types don't match
            const QualType qtLeft = node.getExpressionLeft()->getQualType();
            if (qtLeft.getType() !=
                    node.getExpressionRight()->getQualType().getType()) {
                m_writer.write("(");
                GenTypesC::GenIdentifierFromType(m_writer,
                                                 *qtLeft.getType());
                m_writer.write(")");
            }

            node.getExpressionRight()->acceptVisitor(*this);
        }

        void
        visit(ExprBoolLiteral& node) override {
            if (node.getLiteralValue()) {
                m_writer.write("YAL_TRUE");
            } else {
                m_writer.write("YAL_FALSE");
            }
        }

        void
        visit(ExprIntegerLiteral& node) override {
            switch(node.getIntegerType()) {
            case IntegerType::I8:
                m_writer.write("%", node.getValueAsI8());
                break;
            case IntegerType::U8:
                m_writer.write("%", node.getValueAsU8());
                break;
            case IntegerType::I16:
                m_writer.write("%", node.getValueAsI16());
                break;
            case IntegerType::U16:
                m_writer.write("%", node.getValueAsU16());
                break;
            case IntegerType::I32:
                m_writer.write("%", node.getValueAsI32());
                break;
            case IntegerType::U32:
                m_writer.write("%", node.getValueAsU32());
                break;
            case IntegerType::I64:
                m_writer.write("%", node.getValueAsI64());
                break;
            case IntegerType::U64:
                m_writer.write("%", node.getValueAsU64());
                break;
            default:
                YAL_ASSERT_MESSAGE(false, "Unknown integer type");
            }
        }

        void
        visit(ExprFloatLiteral& node) override {
            m_writer.write("%", node.getLiteralValueAsF64());
        }

        void
        visit(ExprFnCall& node) override {
            m_varRefMkReference = true;
            GenTypesC::GenIdentifierFromType(m_writer,
                                             *node.getFunctionType()->getType());
            const bool hasFunctionArgs = node.getFunctionArgs() != nullptr;
            m_writer.write("(");
            if (hasFunctionArgs) {
                node.getFunctionArgs()->acceptVisitor(*this);
            }
            m_writer.write(")");
            m_varRefMkReference = false;
        }

        void
        visit(ExprTypeFnCall& node) override {
            m_varRefMkReference = true;
            GenTypesC::GenIdentifierFromType(m_writer,
                                             *node.getFunctionType()->getType());
            const bool hasFunctionArgs = node.getFunctionArgs() != nullptr;
            m_writer.write("(");
            m_writer.ident();
            node.getExpression()->acceptVisitor(*this);
            if (hasFunctionArgs) {
                m_writer.write("\n,");
            }
            m_writer.uindent();
            if (hasFunctionArgs) {
                node.getFunctionArgs()->acceptVisitor(*this);
            }
            m_writer.write(")");
            m_varRefMkReference = false;
        }

        void
        visit(ExprTypeFnCallStatic& node) override {
            m_varRefMkReference = true;
            GenTypesC::GenIdentifierFromType(m_writer,
                                             *node.getFunctionType()->getType());
            const bool hasFunctionArgs = node.getFunctionArgs() != nullptr;
            m_writer.write("(");
            if (hasFunctionArgs) {
                node.getFunctionArgs()->acceptVisitor(*this);
            }
            m_writer.write(")");
            m_varRefMkReference = false;
        }

        void
        visit(ExprRangeCast& node) override {
            m_writer.write("((");
            GenTypesC::GenFromQualType(m_writer, node.getQualType());
            m_writer.write(")");
            m_writer.write("");
            node.getExpression()->acceptVisitor(*this);
            m_writer.write(")");
        }

        void
        visit(ExprStructInit& node) override {
            YAL_ASSERT(m_config.modeC99);
            m_writer.write("(");
            GenTypesC::GenIdentifierFromType(m_writer, *node.getQualType().getType());
            m_writer.write(")");
            m_writer.write(" {\n");
            m_writer.ident();
            node.getMemberInitList()->acceptVisitor(*this);

            m_writer.write("}");
            m_writer.uindent();
        }

        void
        visit(StructMemberInitList& node) override {

            if (m_config.modeC99) {
                // use C99 initializer list mode when available
                bool first = true;
                for (auto& child : node.getChildRange()) {
                    if (first) {
                        m_writer.write(" ");
                        first = false;
                    } else {
                        m_writer.write(",");
                    }
                    child->acceptVisitor(*this);
                }
            } else {
                YAL_ASSERT_NOT_IMPLEMENTED();
                // Initialize in order!
            }
        }

        void
        visit(StructMemberInit& node) override {
            YAL_ASSERT(m_config.modeC99);
            m_writer.write(".% = ", node.getMemberName());
            node.getInitExpr()->acceptVisitor(*this);
            m_writer.write("\n");
        }

    private:
        CodeWriter& m_writer;
        const CodeGenCConfig& m_config;
        bool m_varRefMkReference;
    };

    enum { kIdentLevel = 4};

    CodeGenC::CodeGenC(const CodeGenCConfig& config,
                       Module &module,
                       ByteStream &streamSrc,
                       ByteStream &streamHdr):
        m_config(config),
        m_module(module),
        m_streamSrc(streamSrc, kIdentLevel),
        m_streamHdr(streamHdr, kIdentLevel) {

    }

    bool
    CodeGenC::writeHeader() {
        m_streamHdr.write("// Auto generated header file for module %\n",
                          m_module.getName());
        m_streamHdr.write("// Do not modify\n\n");

        if (m_config.pragmaHeaderGuard) {
            m_streamHdr.write("#pragma once\n\n");
        } else {
            std::string headerGuard = GenTypesC::GenIdentifier(m_module.getName());
            for(auto& ch : headerGuard) {
                ch = toupper(ch);
            }
            m_streamHdr.write("#ifndef %_H\n", headerGuard);
            m_streamHdr.write("#define %_H\n\n", headerGuard);
        }


        m_streamHdr.write("\n#if !defined(YAL_BUILTIN_TYPES_DEFINED)\n");
        m_streamHdr.write("#define YAL_BUILTIN_TYPES_DEFINED\n");
        m_streamHdr.write("#include <stdint.h>\n");
        m_streamHdr.write("typedef uint8_t yal_bool;\n");
        m_streamHdr.write("#define YAL_TRUE ((yal_bool)1)\n");
        m_streamHdr.write("#define YAL_FALSE ((yal_bool)0)\n");
        m_streamHdr.write("#endif //!defined(YAL_BUILTIN_TYPES_DEFINED)\n\n");

        VisitorHeader visitor(m_streamHdr);
        for(auto& decl : m_module.getDeclNode()->getDeclarations()) {
            decl->acceptVisitor(visitor);
        }

        if (!m_config.pragmaHeaderGuard) {
            m_streamHdr.write("#endif //End of module header\n");
        }

        return true;
    }

    bool
    CodeGenC::writeSource() {
        m_streamSrc.write("// Auto generated source file for module %\n",
                          m_module.getName());
        m_streamSrc.write("// Do not modify\n\n");

        m_streamSrc.write("#include <%>\n\n", m_config.relativeHeaderPath);

        VisitorSource visitor(m_config, m_streamSrc);
        m_module.getDeclNode()->acceptVisitor(visitor);
        return true;
    }

    void
    CodeGenC::scopeBegin(CodeWriter& stream) {
        stream.write("{\n");
        stream.ident();
    }

    void
    CodeGenC::scopeEnd(CodeWriter &stream,
                       const bool useSemiColon) {
        stream.uindent();
        if (useSemiColon) {
            stream.write("};\n");
        } else {
            stream.write("}\n");
        }
    }


}
