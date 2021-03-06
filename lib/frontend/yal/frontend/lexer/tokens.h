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
#pragma once

#include <cstdlib>
#include "yal/util/stringref.h"

namespace yal::frontend {

    enum class Token {
        Var= 0,
        Let,
        True,
        False,
        If,
        Elif,
        Else,
        And,
        Or,
        Not,
        Return,
        Function,
        While,
        For,
        TypeBool,
        TypeInt8,
        TypeUInt8,
        TypeInt16,
        TypeUInt16,
        TypeInt32,
        TypeUInt32,
        TypeInt64,
        TypeUInt64,
        TypeFloat32,
        TypeFloat64,
        TypeString,
        This,
        Mod,
        Dot,
        BeginArray,
        EndArray,
        BeginScope,
        EndScope,
        ShiftRight,
        ShiftLeft,
        BitXor,
        BitTil,
        BitOr,
        CompareGe,
        CompareGt,
        CompareLe,
        CompareLt,
        CompareEq,
        CompareNe,
        Assign,
        AssignPlus,
        AssignMinus,
        AssignMult,
        AssignDiv,
        AssignAnd,
        AssignOr,
        AssignXor,
        AssignShiftRight,
        AssignShiftLeft,
        Plus,
        Minus,
        Mult,
        Div,
        Identifier,
        IdentifierMulti,
        Colon,
        SemiColon,
        Type,
        BeginPar,
        EndPar,
        Comma,
        Struct,
        Mutable,
        Reference,
        Self,
        As,
        Cast,
        IntegerLiteralI8,
        IntegerLiteralI16,
        IntegerLiteralI32,
        IntegerLiteralI64,
        IntegerLiteralU8,
        IntegerLiteralU16,
        IntegerLiteralU32,
        IntegerLiteralU64,
        DecimalLiteral32,
        DecimalLiteral64,
        IntegerLiteral,
        DecimalLiteral,
        Impl,
        Alias,
        From,
        Import,
        TokenCount
    };

    const char* TokenToString(const Token token);


    struct TokenInfo {
        Token token;
        size_t lineStart;
        size_t lineEnd;
        size_t columnStart;
        size_t columnEnd;
        size_t tokenOffsetInStream;
        StringRefPod tokenStr;
    };

}
