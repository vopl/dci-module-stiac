/* This file is part of the the dci project. Copyright (C) 2013-2023 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include "pch.hpp"
#include "../base.hpp"

namespace dci::module::stiac::stages::in
{
    class Cutting
        : public Base
    {
        Cutting(const Cutting&) = delete;
        void operator=(const Cutting&) = delete;

    public:
        Cutting(Protocol* protocol, bool doIntegrityChecking);

    private:
        void input(Bytes&& msg) override;

    private:
        bool _doIntegrityChecking = true;

        using Crc = boost::crc_optimal<64, 0xad93d23594c935a9, 0, 0, true, true>;

    private:
        Bytes   _input;
        uint32  _chunkSize = 0;
        bool    _chunkFinal = false;
        Bytes   _message;
    };

    using CuttingPtr = std::unique_ptr<Cutting>;
}
