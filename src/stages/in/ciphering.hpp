/* This file is part of the the dci project. Copyright (C) 2013-2021 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include "pch.hpp"
#include "../base.hpp"
#include "../../crypto/symmetric.hpp"

namespace dci::module::stiac::stages::in
{
    class Ciphering
        : public Base
        , public crypto::Symmetric
    {
    public:
        using Base::Base;

    private:
        void input(Bytes&& data) override;

    private:
        bool readType();
        bool readSize();
        bool readPayload();

    private:
        Bytes       _input;

        enum class State
        {
            awaitType,
            awaitSize,
            awaitPayload,
            bad,
        } _state {State::awaitType};

        MessageType _messageType = MessageType::fakeNull;
        uint32      _messageSize = 0;
        bool        _messageMix2Hash = false;

        Bytes       _payload;
    };

    using CipheringPtr = std::unique_ptr<Ciphering>;
}
