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

#include "yal/frontend/ast/astvisitor.h"
#include "yal/frontend/ast/declfunction.h"
#include "yal/frontend/ast/declmodule.h"
#include "yal/frontend/ast/decltypefunction.h"
#include "yal/frontend/ast/declstruct.h"
#include "yal/frontend/ast/declparamvar.h"
#include "yal/frontend/ast/declvar.h"
#include "yal/frontend/ast/exprbinaryoperator.h"
#include "yal/frontend/ast/exprboolliteral.h"
#include "yal/frontend/ast/exprfloatliteral.h"
#include "yal/frontend/ast/exprfncall.h"
#include "yal/frontend/ast/exprintegerliteral.h"
#include "yal/frontend/ast/exprrangecast.h"
#include "yal/frontend/ast/exprstructinit.h"
#include "yal/frontend/ast/exprstructvarref.h"
#include "yal/frontend/ast/exprtypefncall.h"
#include "yal/frontend/ast/exprunaryoperator.h"
#include "yal/frontend/ast/exprvarref.h"
#include "yal/frontend/ast/stmtassign.h"
#include "yal/frontend/ast/stmtdecl.h"
#include "yal/frontend/ast/stmtlistscoped.h"
#include "yal/frontend/ast/stmtreturn.h"
#include "yal/frontend/ast/structmemberinit.h"
#include "yal/frontend/module.h"
