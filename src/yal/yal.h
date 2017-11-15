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

#include <yal/yalconfig.h>
#include <cstdlib>
#include <cinttypes>
#include <cstdint>

#if defined(YAL_DEBUG)
#include <cassert>
#define YAL_ASSERT(x) assert(x)
#define YAL_ASSERT_MESSAGE(x,y) assert((x) && y)
#else
#define YAL_ASSERT(x)
#define YAL_ASSERT_MESSAGE(x,y)
#endif

#define YAL_ASSERT_NOT_IMPLEMENTED() YAL_ASSERT_MESSAGE(false, "Not yet implemented")

#include <limits>
#include <memory>
#include <cstring>
#include <string>
#include <new>

#define YAL_NO_COPY_CLASS(CLASS) \
    CLASS(const CLASS& other) = delete; \
    CLASS(CLASS&& other) = delete; \
    CLASS & operator=(const CLASS& other) = delete; \
    CLASS & operator=(CLASS&& other) = delete

#define YAL_MOVE_ONLY_CLASS(CLASS) \
    CLASS(const CLASS& other) = delete; \
    CLASS(CLASS&& other) = default; \
    CLASS & operator=(const CLASS& other) = delete; \
    CLASS & operator=(CLASS&& other) = default

#define YAL_APPLY_CLASS_DEFAULT(CLASS) \
    CLASS(const CLASS& other) = default; \
    CLASS(CLASS&& other) = default; \
    CLASS & operator=(const CLASS& other) = default; \
    CLASS & operator=(CLASS&& other) = default
