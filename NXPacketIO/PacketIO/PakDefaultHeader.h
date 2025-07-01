﻿#ifndef PakDefaultHeader_H
#define PakDefaultHeader_H

#include "NXPacketIO_Export.h"

#include "GenIO/GenBuffer.h"
#include "PacketIO/PakHeader.h"
#include "PacketIO/PakIntTypes.h"
class GenO;
class GenI;

class NX_PACKETIO_EXPORT PakDefaultHeader : public PakHeader
{
public:
    ~PakDefaultHeader() override;

    PakHeader* Clone() const override;

    void WriteHeader(GenBuffer& aIO, int aPacketID, int aPacketLength) override;
    bool ReadHeader(GenBuffer& aIO, int& aPacketID, int& aPacketLength) override;

    int GetHeaderSize() override;

protected:
    struct header {
        int32_t length;
        int32_t id;
    };
};
#endif
