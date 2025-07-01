﻿#include "XIO/NXXIO_Connection.h"

#include <iostream>

#include "XIO/NXXIO_Interface.h"
#include "XIO/NXXIO_PacketRegistry.h"
#include "PacketIO/PakTCP_IO.h"
#include "PacketIO/PakUDP_IO.h"

NXXIO_Connection::NXXIO_Connection(NXXIO_Interface* aInterfacePtr, PakSocketIO* aIOPtr)
    : mInterfacePtr(aInterfacePtr), mLinkedConnectionPtr(nullptr), mIOPtr(aIOPtr), mIsServer(false), mIsInitialized(false), mDisconnecting(false), mHasClockTranslation(false), mLastTimeStamp(0.0)
{
    static int sUniqueConnectionId = 1;
    mConnectionId = sUniqueConnectionId++;

    mTCP_IO_Ptr = dynamic_cast<PakTCP_IO*>(mIOPtr);
    mUDP_IO_Ptr = dynamic_cast<PakUDP_IO*>(mIOPtr);
}

NXXIO_Connection::~NXXIO_Connection()
{
    delete mIOPtr;
}

void NXXIO_Connection::send(NXXIO_Packet& aPkt)
{
    mInterfacePtr->send(aPkt, this);
}
