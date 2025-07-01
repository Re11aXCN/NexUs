﻿#ifndef GENSOCKETCONNECTION_H
#define GENSOCKETCONNECTION_H

#include "NXPacketIO_Export.h"

#include <memory>
#include <string>

#include "GenIO/GenSocket.h"

//! Base class for a socket connection.  Not much more than a GenSocket.
class NX_PACKETIO_EXPORT GenSocketConnection
{
public:
    GenSocketConnection(GenSockets::GenSocket* aSocket);

    virtual ~GenSocketConnection() = default;

    virtual int GetLocalPort() const;

    bool IsConnected() const;

    virtual bool IsNonBlocking() const;

    virtual void SetIsNonBlocking(bool aTF);

    std::unique_ptr<GenSockets::GenSocket> mSocket;
};

#endif
