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

#include <yal/yal.h>
#include <vector>

namespace yal {

    class MemoryStream;

    class SourceItem;
    class SourceManager
    {
    public:
        class Handle{
        public:
            Handle();
            Handle(const uint32_t id);
            YAL_APPLY_CLASS_DEFAULT(Handle);
            bool isValid() const;
            inline bool operator == (const Handle& other) const {
               return m_id == other.m_id;
            }
            inline bool operator != (const Handle& other) const {
               return m_id != other.m_id;
            }
        private:
            friend class SourceManager;
            uint32_t m_id;
        };

        SourceManager();

        Handle add(std::unique_ptr<SourceItem>&& item);

        SourceItem* getItem(const Handle handle) const;

        inline SourceItem* getItem(const Handle handle) {
            const SourceManager* const_this = this;
            return const_cast<SourceItem*>(const_this->getItem(handle));
        }

    private:
        using SourceList = std::vector<std::unique_ptr<SourceItem>>;
        SourceList m_sources;
    };

    class SourceItem {
       public:

        SourceItem() = default;

        virtual ~SourceItem();

        YAL_NO_COPY_CLASS(SourceItem);

        const SourceManager::Handle getHanlde() const {
            return m_handle;
        }

        inline MemoryStream& getByteStream() {
            const SourceItem* const_this = this;
            return const_cast<MemoryStream&>(const_this->getByteStream());
        }

        virtual const MemoryStream& getByteStream() const = 0;

        virtual const char* getPath() const = 0;

    private:
        friend class SourceManager;
        SourceManager::Handle m_handle;
    };


    struct SourceLocation {
        size_t line = 0;
        size_t column = 0;
        size_t streamOffset = 0;
    };

    struct SourceInfo {
        SourceManager::Handle handle;
        SourceLocation begin;
        SourceLocation end;
    };
}
