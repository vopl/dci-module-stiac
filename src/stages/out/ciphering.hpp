/* This file is part of the the dci project. Copyright (C) 2013-2023 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#pragma once

#include "pch.hpp"
#include "../base.hpp"
#include "../../crypto/symmetric.hpp"

namespace dci::module::stiac::stages::out
{
    class Ciphering
        : public Base
        , public crypto::Symmetric
    {
    public:
        using Base::Base;

        void urgent(MessageType mt, const void* data, uint32 dataSize);
        void allowPayload();

    private:
        uint16 getWantedEmptyPrefix() const override;
        void input(Bytes&& payload) override;
        Bytes flushOutput() override;

    private:
        void flushPayload();
        void encrypt(Bytes&& chunk, bool mixCiphertext2Hash);

    private:
        bool    _payloadAllowed = false;
        Bytes   _payload;
    };

    using CipheringPtr = std::unique_ptr<Ciphering>;
}
