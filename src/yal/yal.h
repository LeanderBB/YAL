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

#include <limits>
#include <memory>


#define YAL_NO_COPY_CLASS(CLASS) \
    CLASS(const CLASS& other) = delete; \
    CLASS(CLASS&& other) = delete; \
    CLASS & operator=(const CLASS& other) = delete; \
    CLASS & operator=(CLASS&& other) = delete
