#ifndef __YAL_BYTECODEPRINTER_H__
#define __YAL_BYTECODEPRINTER_H__

#include <yalvm/yalvm_bytecode.h>
#include "yal/bytecode/bytecode_utils.h"
#include "yal/util/inputsink.h"
#include "yal/util/outputformater.h"
#include <yalvm/yalvm_binary.h>

namespace yal
{
class ByteCodeBuffer;
class ByteCodeBuilder;
class ByteCodePrinter
{
public:
    ByteCodePrinter(InputSink& input,
                    OutputSink& sink);

    ~ByteCodePrinter();

    bool process();

protected:
    bool print(const yalvm_func_header_t& function_header);

    bool print(const yalvm_static_code_hdr_t& static_hdr);

    bool print(const size_t max);

    void print3Args(const yalvm_bytecode_t code);

    void print2Argsi(const yalvm_bytecode_t code);

    void print2Argsu(const yalvm_bytecode_t code);

    void print1Argsu(const yalvm_bytecode_t code);

    void print1Argsi(const yalvm_bytecode_t code);

    void print2Regs(const yalvm_bytecode_t code);

    void print1Reg(const yalvm_bytecode_t code);

protected:
    InputSink& _input;
    OutputSink& _sink;
    OutputFormater _formater;
};

}

#endif
