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

#pragma once

#include "yal/ast/declmodule.h"
#include "yal/ast/declfunction.h"
#include "yal/ast/decltypefunction.h"
#include "yal/ast/declstruct.h"
#include "yal/ast/astvisitor.h"
#include "yal/ast/stmtassign.h"
#include "yal/ast/exprbinaryoperator.h"
#include "yal/ast/exprunaryoperator.h"
#include "yal/ast/exprvarref.h"
#include "yal/ast/declparamvar.h"
#include "yal/ast/exprfncall.h"
#include "yal/ast/exprtypefncall.h"
#include "yal/ast/exprstructvarref.h"
#include "yal/ast/stmtdecl.h"
#include "yal/ast/stmtreturn.h"
#include "yal/ast/statementlist.h"
#include "yal/ast/exprlist.h"
#include "yal/ast/reftype.h"
#include "yal/ast/typebuiltin.h"
#include "yal/ast/typedecl.h"
#include "yal/frontend/module.h"
#include "yal/ast/exprrangecast.h"
#include "yal/ast/exprstructinit.h"
#include "yal/ast/structmemberinit.h"
#include "yal/ast/exprtypefncallstatic.h"
#include "yal/ast/exprboolliteral.h"
#include "yal/ast/exprintegerliteral.h"
#include "yal/ast/exprfloatliteral.h"
