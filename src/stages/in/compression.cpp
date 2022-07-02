/* This file is part of the the dci project. Copyright (C) 2013-2022 vopl, shtoba.
   This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public
   License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.
   You should have received a copy of the GNU Affero General Public License along with this program. If not, see <https://www.gnu.org/licenses/>. */

#include "compression.hpp"
#include "../../protocol.hpp"

namespace dci::module::stiac::stages::in
{
    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Compression::Compression(Protocol* protocol)
        : Base(protocol)
        , _zds(nullptr)
    {
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Compression::~Compression()
    {
        if(_zds)
        {
            ZSTD_freeDStream(_zds);
        }
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    bool Compression::initialize()
    {
        if(_zds)
        {
            return true;
        }

        _zds = ZSTD_createDStream();
        if(!_zds)
        {
            _protocol->internalError(this, "unable to create zstd instance");
            return false;
        }

        auto handleError = [&](size_t res) -> bool
        {
            if(ZSTD_isError(res))
            {
                ZSTD_freeDStream(_zds);
                _zds = nullptr;

                _protocol->internalError(this, std::string{"zstd initialization failed: "}+ZSTD_getErrorName(res));
                return true;
            }

            return false;
        };

//        if(handleError(ZSTD_DCtx_setFormat(_zds, ZSTD_f_zstd1_magicless)))
//        {
//            return false;
//        }

        if(handleError(ZSTD_initDStream(_zds)))
        {
            return false;
        }

        return true;
    }

    /////////0/////////1/////////2/////////3/////////4/////////5/////////6/////////7
    Bytes Compression::flushOutput()
    {
        if(!_zds)
        {
            _protocol->internalError(this, "zstd uninitialized");
            return Bytes();
        }

        bytes::Alter src(_output.begin());

        Bytes output;
        bytes::Alter dst(output.begin());

        for(;;)
        {
            ZSTD_inBuffer inBuffer {src.continuousData(), src.continuousDataSize(), 0};

            uint32 writeBufferSize;
            ZSTD_outBuffer outBuffer {dst.prepareWriteBuffer(writeBufferSize), writeBufferSize, 0};

            size_t res = ZSTD_decompressStream(_zds, &outBuffer, &inBuffer);
            dst.commitWriteBuffer(static_cast<uint32>(outBuffer.pos));

            if(ZSTD_isError(res))
            {
                _protocol->internalError(this, std::string{"zstd decompression failed: "} + ZSTD_getErrorName(res));
                return output;
            }

            src.remove(static_cast<uint32>(inBuffer.pos));

            if(src.atEnd())
            {
                if(outBuffer.pos < outBuffer.size)
                {
                    return output;
                }

                break;
            }
        }

        ZSTD_inBuffer inBufferNull {nullptr, 0, 0};
        for(;;)
        {
            uint32 writeBufferSize;
            ZSTD_outBuffer outBuffer {dst.prepareWriteBuffer(writeBufferSize), writeBufferSize, 0};

            size_t res = ZSTD_decompressStream(_zds, &outBuffer, &inBufferNull);
            dst.commitWriteBuffer(static_cast<uint32>(outBuffer.pos));

            if(ZSTD_isError(res))
            {
                _protocol->internalError(this, std::string{"zstd decompression failed: "} + ZSTD_getErrorName(res));
                return output;
            }

            if(outBuffer.pos < outBuffer.size)
            {
                return output;
            }
        }

        dbgWarn("never here");
        return output;
    }
}
