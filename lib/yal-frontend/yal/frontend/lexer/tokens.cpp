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
#include "yal/frontend/lexer/tokens.h"
#include "yal/yal.h"

namespace yal::frontend
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
            "Comma",
            "Struct",
            "Mutable",
            "Self",
            "Reference",
            "As",
            "Cast",
            "IntegerLiteralI8",
            "IntegerLiteralI16",
            "IntegerLiteralI32",
            "IntegerLiteralI64",
            "IntegerLiteralU8",
            "IntegerLiteralU16",
            "IntegerLiteralU32",
            "IntegerLiteralU64",
            "DecimalLiteral32",
            "DecimalLiteral64",
            "IntegerLiteral",
            "DecimalLiteral",
            "Impl"
        };

        static_assert(sizeof(sLookupTable)/sizeof(sLookupTable[0]) ==
                static_cast<uint32_t>(Token::TokenCount),
                "Table size must match Token::TokenCount value");

        return sLookupTable[static_cast<uint32_t>(token)];
    }

}
