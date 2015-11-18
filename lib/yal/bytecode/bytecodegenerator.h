#ifndef __YAL_BYTECODEGENERATOR_H__
#define __YAL_BYTECODEGENERATOR_H__

#include "yal/ast/astnodevisitor.h"
#include "yal/bytecode/bytecodebuffer.h"
#include "yal/bytecode/registerallocator.h"
#include "yal/bytecode/bytecodegenerator.h"
#include "yal/util/errorhandler.h"
#include "yal/module/module.h"
#include "yal/util/str_utils.h"
#include "yal/symbols/scopeaction.h"
#include <stack>

namespace yal
{

class FunctionCache;
class SymbolTable;
class StatementNode;
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

        Register(const Symbol* sym,
                 RegisterAllocator& alloctor);

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
    ByteCodeGenerator(Module& moduleInfo);

    virtual ~ByteCodeGenerator();

    void generateFunction(FunctionDeclNode &node);

    bool generate(StatementNode &node);

    const ByteCodeBuffer& buffer() const
    {
        return _buffer;
    }

    void reset();

    void addHaltInst();

    yal_u8 maxRegisterCount() const;

    bool onScopeBeginGlobal(const Scope &scope);

    bool onScopeEndGlobal(const Scope& scope);

protected:

#define YAL_NODE_LIST_FUNC(CLASS) virtual void visit(CLASS& node) override;
#include "yal/ast/astnodelist.h"
#undef YAL_NODE_LIST_FUNC

    /// Return true if function has a return type;
    bool setupFunction(const FunctionDeclBaseNode &node);

    bool registerScope(const Scope *scope);

    void unregisterScope(const Scope* scope);

    void pushRegister(const Register reg);

    const Register& topRegister() const;

    Register popRegister();

    void allocateRegister(const char* variable);

    void allocateRegisterTmp();

    void releaseRegister();

    void getGlobalVarIdx(yal_u32& idx,
                         const char* varName,
                         const AstBaseNode& node);

    void getFunctionIdx(yal_u32& idx,
                        const char* functionName,
                        const AstBaseNode& node);

    void getConstantIdx(yal_u32& idx,
                        const ConstantValue& val,
                        const AstBaseNode& node);

    bool onScopeBegin(const AstBaseNode& node);

    bool onScopeEnd(const AstBaseNode& node);

    bool onStatementScopeBegin(const StatementNode& node);

    bool onStatementScopeEnd(const StatementNode& node);

    void loadVariableIntoRegister(const char *varName,
                                  const AstBaseNode &node);

    void releaseObject(const Symbol& sym,
                       const Register& reg);

    Register registerForSymbol(const Symbol* sym);

    void clearGlobalMap();

    void handleConditionalOffset(const AstBaseNode &node,
                                 const size_t onTrueOffset,
                                 const size_t onFalseOffset);

    void onParentesesEnter(yal::ParentExpNode &node);

    void onParentesesExit(yal::ParentExpNode &node);

protected:
    typedef std::vector<size_t> OffsetVector_t;
    typedef size_t ConditionOffset_t;
    typedef OffsetVector_t ConitionOffsetVec_t;
    typedef OffsetVector_t ParenthesisOffsetVec_t;

    class ByteCodeGeneratorScopeActionVisitor : public ScopeActionVisitor
    {
    public:

        ByteCodeGeneratorScopeActionVisitor(ByteCodeGenerator& generator):
            _generator(generator)
        {
        }

        virtual ~ByteCodeGeneratorScopeActionVisitor() {}

#define YAL_SCOPE_ACTION_LIST_FUNC(x)\
    virtual void visitOnEnter(const x& action) override; \
    virtual void visitOnExit(const x& action) override;
#include "yal/symbols/scopeactionlist.h"
#undef YAL_SCOPE_ACTION_LIST_FUNC

    private:
        ByteCodeGenerator& _generator;
    };

    Module& _moduleInfo;
    std::stack<Register> _registerStack;
    ByteCodeBuffer _buffer;
    RegisterAllocator _regAllocator;
    ByteCodeGeneratorScopeActionVisitor _scopeVisitor;
    StrHashMap<Register> _globalMap;
    ConitionOffsetVec_t _conditionOffsets;
    bool _trackConditionOffsets;
    std::stack<ParenthesisOffsetVec_t> _parenthesisOffsetStack;
    ParenthesisOffsetVec_t* _curParenthesisOffsets;
};
}
#endif
