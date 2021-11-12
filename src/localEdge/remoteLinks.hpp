/* This file is part of the the dci project. Copyright (C) 2013-2021 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include "../pch.hpp"
#include "linkIdOperations.hpp"

namespace dci::module::stiac::localEdge
{
    class RemoteLinks
    {
    public:
        RemoteLinks();
        ~RemoteLinks();

        bool emplace(link::BasePtr&& link, link::RemoteId id);
        link::Base* get(link::RemoteId id);
        bool remove(link::RemoteId id);

        bool beginRemove(link::RemoteId id);
        bool endRemove(link::RemoteId id);

        void deinitialize();

    private:
        std::deque<link::BasePtr>   _links;
        std::set<uint64>            _zombieList;
    };
}
