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

#include "yal/transpiler/c/codegenc.h"
#include "yal/transpiler/c/gentypesc.h"
#include "yal/ast/astnodes.h"
#include "yal/io/bytestream.h"
#include <algorithm>
namespace yal {

    class CodeGenC::FwdDeclVisitor : public AstVisitor {
    public:

        FwdDeclVisitor(CodeWriter& stream):
            m_writer(stream){
        }

        virtual void visit(DeclStruct& node) override {
            m_strBuffer = GenTypesC::GenIdentifier(node.getIdentifier());
            m_writer.write("struct %;\n\n", m_strBuffer);
        }

        virtual void visit(DeclFunction& node) override {
            if (node.hasReturnValue()) {
                GenTypesC::GenFromQualType(m_writer, node.getReturnQualType());
            } else {
                m_writer.write("void ");
            }

            m_writer.write();

            m_strBuffer = GenTypesC::GenIdentifier(node.getIdentifier());
            m_writer.write("%", m_strBuffer);

            if (node.hasFunctionParameters()) {
                node.getParams()->acceptVisitor(*this);
            } else {
                m_writer.write("();\n");
            }

            m_writer.write();
        }

        virtual void visit(DeclTypeFunction& node) override {
            if (node.hasReturnValue()) {
                GenTypesC::GenFromQualType(m_writer, node.getReturnQualType());
            } else {
                m_writer.write("void ");
            }

            m_writer.write();

            m_strBuffer = GenTypesC::GenIdentifier(node.getIdentifier());
            m_writer.write("%", m_strBuffer);

            if (node.hasFunctionParameters()) {
                node.getParams()->acceptVisitor(*this);
            } else {
                m_writer.write("();\n");
            }
            m_writer.write();
        }

        virtual void visit(DeclParamVarContainer& node) override {
            bool first = true;
            m_writer.write("(\n");
            m_writer.ident();
            for (auto& param : node.getChildRangeConst()) {
                if (!first) {
                    m_writer.write(",");
                } else {
                    m_writer.write(" ");
                }
                GenTypesC::GenFromQualType(m_writer, param->getQualType());
                m_writer.write("%\n", param->getName());
                first = false;
            }
            m_writer.uindent();
            m_writer.write(");\n");
        }

    private:
        CodeWriter& m_writer;
        std::string m_strBuffer;
    };

    enum { kIdentLevel = 4};

    CodeGenC::CodeGenC(Module &module,
                       ByteStream &streamSrc,
                       ByteStream &streamHdr):
        m_module(module),
        m_streamSrc(streamSrc, kIdentLevel),
        m_streamHdr(streamHdr, kIdentLevel),
        m_formater(),
        m_identLevel(0) {

    }

    bool
    CodeGenC::writeFwdDecls() {
        m_streamHdr.write("// Auto generated code for module %\n", m_module.getName());
        m_streamHdr.write("// Do not modify\n\n");

        FwdDeclVisitor fwdDeclVistior(m_streamHdr);
        for(auto& decl : m_module.getDeclNode()->getDeclarations()) {
            decl->acceptVisitor(fwdDeclVistior);
        }
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
