/* This file is part of the the dci project. Copyright (C) 2013-2021 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include "pch.hpp"
#include "stages/base.hpp"

namespace dci::module::stiac
{
    class RemoteEdge
        : public stages::Base
        , private sbs::Owner
    {
    public:
        RemoteEdge(Protocol* protocol, const api::RemoteEdge<>::Opposite& interface);
        ~RemoteEdge() override;

    private:
        void input(Bytes&& msg) override;

    private:
        api::RemoteEdge<>::Opposite _interface;
    };

    using RemoteEdgePtr = std::unique_ptr<RemoteEdge>;
}
