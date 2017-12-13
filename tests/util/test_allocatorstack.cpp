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
#include <gtest/gtest.h>
#include <yal/yal.h>
#include <yal/util/allocatorstack.h>
#include <limits>
TEST(AllocatorStack, BasicAlloc) {

    enum {kStackSize = 128};
    yal::AllocatorStack allocator(kStackSize);

    {
        void* ptr = allocator.allocate(40);
        EXPECT_NE(ptr, nullptr);
    }

    {
        void* ptr = allocator.allocate(40);
        EXPECT_NE(ptr, nullptr);
    }

    EXPECT_EQ(allocator.getAllocatedSizeBytes(), size_t(80));
    EXPECT_EQ(allocator.getMemorySizeBytes(), size_t(kStackSize));
    EXPECT_EQ(allocator.getStackCount(), size_t(1));

    allocator.reset();
}

TEST(AllocatorStack, MultiStackAlloc) {

    enum {kStackSize = 32};
    yal::AllocatorStack allocator(kStackSize);

    {
        void* ptr = allocator.allocate(40);
        EXPECT_EQ(ptr, nullptr);
    }

    {
        void* ptr = allocator.allocate(16);
        EXPECT_NE(ptr, nullptr);
    }

    {
        void* ptr = allocator.allocate(12);
        EXPECT_NE(ptr, nullptr);
    }

    {
        void* ptr = allocator.allocate(24);
        EXPECT_NE(ptr, nullptr);
    }


    EXPECT_EQ(allocator.getAllocatedSizeBytes(), size_t(52));
    EXPECT_EQ(allocator.getMemorySizeBytes(), size_t(kStackSize) * 2);
    EXPECT_EQ(allocator.getStackCount(), size_t(2));
}

TEST(AllocatorStack, TestOverFlow) {

    enum {kStackSize = 32};

    yal::AllocatorStack allocator(kStackSize);

    {
        void* ptr = allocator.allocate(20);
        EXPECT_NE(ptr, nullptr);
    }

    {
        void* ptr = allocator.allocate(std::numeric_limits<size_t>::max());
        EXPECT_EQ(ptr, nullptr);
    }

    EXPECT_EQ(allocator.getAllocatedSizeBytes(), size_t(20));
    EXPECT_EQ(allocator.getMemorySizeBytes(), size_t(kStackSize));
    EXPECT_EQ(allocator.getStackCount(), size_t(1));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
