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

#include "yal/yal.h"
#include <vector>

namespace yal{

    /// Single threaded stack allocator
    class AllocatorStack
    {
    public:
        AllocatorStack(const size_t stackSizeBytes);

        AllocatorStack(const AllocatorStack&) = delete;
        AllocatorStack& operator=(const AllocatorStack& ) = delete;

        AllocatorStack(AllocatorStack&& other);
        AllocatorStack& operator=(AllocatorStack&& other);

        ~AllocatorStack();

        void* allocate(const size_t bytes);

        void reset();

        size_t getAllocatedSizeBytes() const;

        size_t getMemorySizeBytes() const;

        size_t getStackCount() const;

        template<typename T, typename... ARGS>
        T* construct(ARGS&& ...args) {
            void* mem = allocate(sizeof(T));
            YAL_ASSERT(mem != nullptr);
            T* ptr =new (mem) T(std::forward<ARGS>(args)...);
            m_dtors.emplace_back(ptr, [](void* ptr){
                T* typePtr = reinterpret_cast<T*>(ptr);
                typePtr->~T();
            });
            return ptr;
        }

    private:

        bool allocateNewStack();

        struct Stack {
            Stack() = default;
            ~Stack();
            Stack(const Stack& ) = delete;
            Stack& operator=(const Stack&) = delete;
            Stack(Stack&& stack);
            Stack& operator=(Stack&& stack);

            void* ptr = nullptr;
            size_t offsetBytes = 0;
        };

        const size_t m_stackSizeBytes;
        using StackList = std::vector<Stack>;
        using DtorList = std::vector<std::pair<void*,void(*)(void*)>>;
        Stack* m_activeStack;
        StackList m_stackList;
        DtorList m_dtors;
    };
}
