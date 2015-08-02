#ifndef __YAL_BYTECODEGENERATOR_H__
#define __YAL_BYTECODEGENERATOR_H__

#include "yal/ast/astnodevisitor.h"
#include "yal/bytecode/bytecodebuffer.h"
#include "yal/bytecode/registerallocator.h"
#include "yal/bytecode/bytecodegenerator.h"
#include "yal/util/errorhandler.h"
#include "yal/util/outputformater.h"
#include "yal/module/module.h"
#include <stack>

namespace yal
{

class FunctionCache;
class SymbolTable;
class ByteCodeGenerator : public AstNodeVisitor
{
private:
    class Register
    {
    public:
        Register();

        Register(RegisterAllocator& alloctor,
                 const char* variable,
                 const yal_u32 scopeLevel);

        Register(RegisterAllocator& alloctor);

        bool isValid() const;

        void release(RegisterAllocator& alloctor);

        yal_i8 registerIdx() const
        {
            return _registerIdx;
        }
    private:
        yal_i32 _registerIdx;
        yal_u32 _scopeLevel;
        bool _isTemporary;
    };

public:
    ByteCodeGenerator(Module& moduleInfo,
                      ErrorHandler& errorHandler);

    virtual ~ByteCodeGenerator();

    bool generateFunction(yal::FunctionDeclNode &node);

    bool generate(AstBaseNode& node);

    const ByteCodeBuffer& buffer() const
    {
        return _buffer;
    }

    void reset();

    void addHaltInst();

    yal_u8 maxRegisterCount() const;

protected:

#define YAL_NODE_LIST_FUNC(CLASS) virtual void visit(CLASS& node) override;
#include "yal/ast/astnodelist.h"
#undef YAL_NODE_LIST_FUNC

    bool didError() const
    {
        return _didError;
    }

    void setError()
    {
        _didError = true;
    }

    void logError(const AstBaseNode& node);

    /// Return true if function has a return type;
    bool setupFunction(const yal::FunctionDeclNode &node);

    bool registerScope(const SymbolTable *table);

    void unregisterScope(const SymbolTable* table);

    void pushAndSetRegister(const Register reg);

    void popRegister();

    void allocateRegister(const char* variable);

    void allocateRegisterTmp();

    void releaseRegister();

    bool getGlobalVarIdx(yal_u32& idx,
                         const char* varName);

    bool getFunctionIdx(yal_u32& idx,
                        const char* functionName);

    bool getConstantIdx(yal_u32& idx,
                        const ConstantValue& val);


protected:
    Module& _moduleInfo;
    ErrorHandler& _errorHandler;
    std::stack<Register> _registerStack;
    ByteCodeBuffer _buffer;
    RegisterAllocator _regAllocator;
    Register _currentRegister;
    OutputFormater _formater;
    bool _didError;
};

}
#endif
