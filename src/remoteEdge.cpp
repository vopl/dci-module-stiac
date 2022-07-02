/* This file is part of the the dci project. Copyright (C) 2013-2022 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#include "remoteEdge.hpp"
#include "protocol.hpp"

namespace dci::module::stiac
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    RemoteEdge::RemoteEdge(Protocol* protocol, const api::RemoteEdge<>::Opposite& interface)
        : stages::Base(protocol)
        , _interface(interface)
    {
        _interface.involvedChanged() += this * [this](bool v)
        {
            if(!v)
            {
                _protocol->remoteEdgeWantRemove(this);
            }
        };

        //пользователь -> цепь, увести в следующий линк
        _interface->input() += this * [this](Bytes&& data)
        {
            Base::accumulateOutput(std::move(data));
        };
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    RemoteEdge::~RemoteEdge()
    {
        sbs::Owner::flush();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void RemoteEdge::input(Bytes&& msg)
    {
        //цепь -> пользователь
        _interface->output(std::move(msg));
    }
}
