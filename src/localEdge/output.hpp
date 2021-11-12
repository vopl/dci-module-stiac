/* This file is part of the the dci project. Copyright (C) 2013-2021 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include "../pch.hpp"

namespace dci::module::stiac::localEdge
{
    class Output
        : public link::Hub4Sink
    {
    public:
        Output(Bytes& data);
        ~Output() override;

        link::Sink makeSink(uint32 reserveIfCan);

        link::LocalId emplaceLink(link::BasePtr&& link) override = 0;
        void finalize(link::Sink& sink, bytes::Alter&& buffer) override;
        std::pair<uint32, bool> mapTuid(const std::array<uint8, 16>& tuid) override;

    private:
        Bytes& _data;

        bool _hasActiveSink = false;
        uint32 _reserved = 0;

        using TuidMap = std::map<std::array<uint8, 16>, uint32>;
        TuidMap _tuidMap;
    };
}
