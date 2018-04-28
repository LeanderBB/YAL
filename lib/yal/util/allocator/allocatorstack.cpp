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
#include "yal/util/allocator/allocatorstack.h"
namespace yal {

//#define ALLOCATOR_DEBUG
#if defined(ALLOCATOR_DEBUG)
    enum {
        kMemMarker = 0xDEADBEEF
    };
#endif

    AllocatorStack::Stack::~Stack() {
        if (ptr != nullptr){
            ::free(ptr);
        }
    }

    AllocatorStack::Stack::Stack(Stack&& stack):
        ptr(stack.ptr),
        offsetBytes(stack.offsetBytes) {
        stack.ptr = nullptr;
        stack.offsetBytes = 0;
    }

    AllocatorStack::Stack&
    AllocatorStack::Stack::operator=(Stack&& stack) {
        if (&stack != this) {
            std::swap(ptr, stack.ptr);
            std::swap(offsetBytes, stack.offsetBytes);
        }
        return *this;
    }

    AllocatorStack::AllocatorStack(const size_t stackSizeBytes):
        m_stackSizeBytes(stackSizeBytes),
        m_activeStack(nullptr),
        m_stackList(),
        m_dtors() {
        m_dtors.reserve(128);
    }

    AllocatorStack::~AllocatorStack() {
        reset();
    }

    void*
    AllocatorStack::allocate(const size_t bytes) {
        if (m_stackList.empty()) {
            if(!allocateNewStack()) {
                return nullptr;
            }
        }

        YAL_ASSERT(m_activeStack != nullptr);


        if (bytes > m_stackSizeBytes) {
            YAL_ASSERT_MESSAGE(false, "Can't allocate bigger than page size!");
            return nullptr;
        }

#if defined(ALLOCATOR_DEBUG)
        const size_t bytesDebug = 4;
#else
        const size_t bytesDebug = 0;
#endif
        const size_t newOffset = m_activeStack->offsetBytes + bytes + bytesDebug;
        YAL_ASSERT_MESSAGE(newOffset > m_activeStack->offsetBytes,
                           "Overflow detected");

        void* result = nullptr;
        if (newOffset > m_stackSizeBytes) {
            const bool allocResult = allocateNewStack();
            if (!allocResult) {
                YAL_ASSERT_MESSAGE(false,"Faile to allocate stack memory");
                return nullptr;
            }
            result = m_activeStack->ptr;
            m_activeStack->offsetBytes = bytes + bytesDebug;
        } else {
            void* correctedPtr =
                    reinterpret_cast<unsigned char*>(m_activeStack->ptr) +
                    m_activeStack->offsetBytes;
#if defined(ALLOCATOR_DEBUG)
            uint32_t* ptr =  reinterpret_cast<uint32_t*>(correctedPtr);
            YAL_ASSERT(*ptr != kMemMarker);
            if (m_activeStack->offsetBytes !=0) {
                --ptr;
                YAL_ASSERT(*ptr == kMemMarker);
            }
#endif
            m_activeStack->offsetBytes = newOffset;
            result = correctedPtr;
        }
#if defined(ALLOCATOR_DEBUG)
        uint32_t* ptr = reinterpret_cast<uint32_t*>(reinterpret_cast<unsigned char*>(m_activeStack->ptr) +
                                                    m_activeStack->offsetBytes);
        --ptr;
        *ptr = kMemMarker;
#endif
        return result;
    }

    void
    AllocatorStack::reset() {
        m_activeStack = nullptr;
        for(auto& dtor: m_dtors) {
            dtor.second(dtor.first);
        }
        m_stackList.clear();
    }

    bool
    AllocatorStack::allocateNewStack() {
        void* ptr = ::malloc(m_stackSizeBytes);
        if (ptr == nullptr) {
            return false;
        }

        const size_t curStackListSize = m_stackList.size();
        Stack stack;
        stack.ptr = ptr;
        m_stackList.emplace_back(std::move(stack));
        m_activeStack = &m_stackList[curStackListSize];
        return true;
    }

    size_t
    AllocatorStack::getAllocatedSizeBytes() const {
        size_t allocatedBytes = 0;
        for (auto& stack : m_stackList) {
            allocatedBytes += stack.offsetBytes;
        }
        return allocatedBytes;
    }

    size_t
    AllocatorStack::getMemorySizeBytes() const {
        return m_stackSizeBytes * m_stackList.size();
    }

    size_t
    AllocatorStack::getStackCount() const {
        return m_stackList.size();
    }

}
