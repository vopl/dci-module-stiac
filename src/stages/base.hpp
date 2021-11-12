/* This file is part of the the dci project. Copyright (C) 2013-2021 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include "pch.hpp"

namespace dci::module::stiac
{
    class Protocol;
}

namespace dci::module::stiac::stages
{
    class Base
    {
    public:
        Base(Protocol* protocol);
        virtual ~Base();

        virtual bool initialize();

        void setIndexInChain(std::size_t index);
        std::size_t getIndexInChain() const;

        void setWantedEmptyPrefix(uint16 size);
        virtual uint16 getWantedEmptyPrefix() const;

        virtual void input(Bytes&& msg);

        virtual bool hasOutput() const;
        virtual Bytes flushOutput();

        Bytes& outputBuffer();

    protected:
        void accumulateOutput(Bytes&& msg);

    protected:
        Protocol* _protocol;
        std::size_t _indexInChain = ~std::size_t();

        uint16 _wantedEmptyPrefix = 0;

        Bytes _output;
    };
}
