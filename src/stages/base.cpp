/* This file is part of the the dci project. Copyright (C) 2013-2021 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#include "base.hpp"
#include "../protocol.hpp"

namespace dci::module::stiac::stages
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Base::Base(Protocol* protocol)
        : _protocol(protocol)
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Base::~Base()
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    bool Base::initialize()
    {
        return true;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void Base::setIndexInChain(std::size_t index)
    {
        _indexInChain = index;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    std::size_t Base::getIndexInChain() const
    {
        return _indexInChain;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void Base::setWantedEmptyPrefix(uint16 size)
    {
        _wantedEmptyPrefix = size;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    uint16 Base::getWantedEmptyPrefix() const
    {
        return _wantedEmptyPrefix + 0;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void Base::input(Bytes&& msg)
    {
        //дефолтная реализация проводит трафик 1:1 со входа на выход
        accumulateOutput(std::move(msg));
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    bool Base::hasOutput() const
    {
        return !_output.empty();
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Bytes Base::flushOutput()
    {
        return std::move(_output);
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Bytes& Base::outputBuffer()
    {
        return _output;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    void Base::accumulateOutput(Bytes&& msg)
    {
        if(msg.empty())
        {
            return;
        }

        _output.end().write(std::move(msg));
        _protocol->linkHasOutput(this);
    }
}
