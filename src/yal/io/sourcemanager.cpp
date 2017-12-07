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

#include "yal/io/sourcemanager.h"
#include <limits>
namespace yal{

    static  constexpr uint32_t GetInvalidHanldeId() {
        return std::numeric_limits<uint32_t>::max();
    }


    SourceManager::Handle::Handle():
        m_id(GetInvalidHanldeId()) {

    }

    SourceManager::Handle::Handle(const uint32_t id):
        m_id(id){
    }

    bool
    SourceManager::Handle::isValid() const {
        return m_id != GetInvalidHanldeId();
    }

    SourceManager::SourceManager() {

    }

    SourceManager::Handle
    SourceManager::add(std::unique_ptr<SourceItem>&& item) {
        YAL_ASSERT(item);
        Handle result(static_cast<uint32_t>(m_sources.size()));
        item->m_handle = result;
        m_sources.emplace_back(std::move(item));
        return result;
    }

    SourceItem *SourceManager::getItem(const Handle handle) const {
        const uint32_t idx = handle.m_id;
        return (idx < m_sources.size())
                ? m_sources[idx].get()
                : nullptr;
    }

    SourceItem::~SourceItem() {

    }

}
