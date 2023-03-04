/* This file is part of the the dci project. Copyright (C) 2013-2023 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#include "input.hpp"

namespace dci::module::stiac::localEdge
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Input::Input()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Input::~Input()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void Input::append(Bytes&& data)
    {
        _data.end().write(std::move(data));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    bool Input::empty() const
    {
        return _data.empty();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    link::Source Input::makeSource()
    {
        dbgAssert(!_hasActiveSource);
        dbgAssert(!empty());

        _hasActiveSource = true;
        return link::Source(this, _data.begin());
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void Input::finalize(link::Source& source, bytes::Alter&& buffer)
    {
        dbgAssert(_hasActiveSource);
        _hasActiveSource = false;

        (void)source;
        bytes::Alter{std::move(buffer)};
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    bool Input::mapTuid(uint32& mapped, const std::array<uint8, 16>& tuid)
    {
        auto res = _tuidMapFwd.try_emplace(tuid, _tuidMapFwd.size());

        if(!res.second)
        {
            return false;
        }

        mapped = res.first->second;
        dbgAssert(_tuidMapBwd.size() == mapped);

        _tuidMapBwd.emplace_back(tuid);

        return true;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    bool Input::unmapTuid(const uint32& mapped, std::array<uint8, 16>& tuid)
    {
        if(_tuidMapBwd.size() <= mapped)
        {
            return false;
        }

        tuid = _tuidMapBwd[mapped];
        return true;
    }
}
