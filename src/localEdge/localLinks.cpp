/* This file is part of the the dci project. Copyright (C) 2013-2021 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#include "localLinks.hpp"

namespace dci::module::stiac::localEdge
{
    namespace
    {
        link::LocalId idx2Id(uint64 idx)
        {
            return static_cast<link::LocalId>(idx+1);
        }

        uint64 id2Idx(link::LocalId id)
        {
            return static_cast<uint64>(id)-1;
        }
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    LocalLinks::LocalLinks()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    LocalLinks::~LocalLinks()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    link::LocalId LocalLinks::emplace(link::BasePtr&& link)
    {
        dbgAssert(link);

        if(_freeList.empty())
        {
            _links.emplace_back(std::move(link));
            dbgAssert(!_zombieList.contains(_links.size()-1));
            return idx2Id(_links.size()-1);
        }

        auto idx = *_freeList.begin();
        dbgAssert(!_zombieList.contains(idx));
        _freeList.erase(_freeList.begin());

        dbgAssert(!_links[idx]);
        _links[idx] = std::move(link);
        return idx2Id(idx);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    link::Base* LocalLinks::get(link::LocalId id)
    {
        uint64 idx = id2Idx(id);

        if(idx >= _links.size())
        {
            return nullptr;
        }

        return _links[idx].get();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    bool LocalLinks::remove(link::LocalId id)
    {
        uint64 idx = id2Idx(id);

        if(_zombieList.contains(idx))
        {
            return false;
        }

        if(idx >= _links.size())
        {
            return false;
        }

        if(!_links[idx])
        {
            return false;
        }

        _links[idx].reset();
        _freeList.insert(idx);

        while(!_links.empty() && !_links.back())
        {
            _links.pop_back();
            _freeList.erase(_links.size());
        }

        return true;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    bool LocalLinks::beginRemove(link::LocalId id)
    {
        uint64 idx = id2Idx(id);

        if(idx >= _links.size())
        {
            return false;
        }

        if(!_links[idx])
        {
            return false;
        }

        return _zombieList.insert(idx).second;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    bool LocalLinks::endRemove(link::LocalId id)
    {
        uint64 idx = id2Idx(id);

        auto iter = _zombieList.find(idx);

        if(_zombieList.end() == iter)
        {
            return false;
        }

        if(idx >= _links.size())
        {
            return false;
        }

        if(!_links[idx])
        {
            return false;
        }

        _zombieList.erase(iter);
        _links[idx].reset();
        _freeList.insert(idx);

        while(!_links.empty() && !_links.back())
        {
            _links.pop_back();
            _freeList.erase(_links.size());
        }

        return true;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void LocalLinks::deinitialize()
    {
        for(const link::BasePtr& link : _links)
        {
            if(link)
            {
                link->deinitialize();
            }
        }
    }
}
