
#include "yal/lexer/tokens.h"
#include "yal/yal.h"
namespace yal
{

    const char* TokenToString(const Token token){
        static const char* sLookupTable[] ={
            "Var",
            "Let",
            "True",
            "False",
            "If",
            "Elif",
            "Else",
            "And",
            "Or",
            "Not",
            "Return",
            "Function",
            "While",
            "For",
            "TypeBool",
            "TypeInt8",
            "TypeUInt8",
            "TypeInt16",
            "TypeUInt16",
            "TypeInt32",
            "TypeUInt32",
            "TypeInt64",
            "TypeUInt64",
            "TypeFlaot",
            "TypeDouble",
            "TypeString",
            "This",
            "Mod",
            "Dot",
            "BeginArray",
            "EndArray",
            "BeginScope",
            "EndScope",
            "ShiftRight",
            "ShiftLeft",
            "BitXor",
            "BitTil",
            "BitOr",
            "CompareGe",
            "CompareGt",
            "CompareLe",
            "CompareLt",
            "CompareEq",
            "CompareNe",
            "Assign",
            "AssignPlus",
            "AssignMinus",
            "AssignMult",
            "AssignDiv",
            "AssignAnd",
            "AssignOr",
            "AssignXor",
            "AssignShiftRight",
            "AssignShiftLeft",
            "Plus",
            "Minus",
            "Mult",
            "Div",
            "Identifier",
            "Colon",
            "SemiColon",
            "Type",
            "BeginParentheses",
            "EndParentheses",
            "IntegerLiteral",
            "DecimalLiteral",
            "Comma"
        };

        static_assert(sizeof(sLookupTable)/sizeof(sLookupTable[0]) ==
                static_cast<uint32_t>(Token::TokenCount),
                "Table size must match Token::TokenCount value");

        return sLookupTable[static_cast<uint32_t>(token)];
    }

}
