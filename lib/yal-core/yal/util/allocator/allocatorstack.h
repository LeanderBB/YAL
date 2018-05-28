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

        template<typename T, typename... ARGS>
        T* constructNoDtor(ARGS&& ...args) {
            void* mem = allocate(sizeof(T));
            YAL_ASSERT(mem != nullptr);
            return new (mem) T(std::forward<ARGS>(args)...);
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

    template<typename T>
    class StdAllocatorWrapperStack {
    public:
        using value_type = T;
        using propagate_on_container_move_assignment = std::false_type;
        using propagate_on_container_copy_assignment = std::false_type;
        using propagate_on_container_swap = std::false_type;

        inline explicit StdAllocatorWrapperStack(AllocatorStack& allocator):
            m_allocator(allocator){
        }

        inline ~StdAllocatorWrapperStack() {}

        inline explicit StdAllocatorWrapperStack(const StdAllocatorWrapperStack& other):
            m_allocator(other.m_allocator) {

        }

        inline explicit StdAllocatorWrapperStack(StdAllocatorWrapperStack&& other):
            m_allocator(other.m_allocator) {

        }

        inline T* allocate(std::size_t n) {
            return static_cast<T*>(m_allocator.allocate(n * sizeof(T)));
        }
        inline void deallocate(T*, std::size_t) {
            // do nothing, memory freed on
        }

    private:
        AllocatorStack& m_allocator;
    };

    // Determines if memory from antoher allocator can be
    // deallocate with this one
    template<class T, class U>
    inline bool operator == (const StdAllocatorWrapperStack<T>&,
                             const StdAllocatorWrapperStack<U>&){
        return false;
    }

    template<class T, class Other>
    inline bool operator != (const StdAllocatorWrapperStack<T>&,
                             const Other&) {
        return false;
    }

}
