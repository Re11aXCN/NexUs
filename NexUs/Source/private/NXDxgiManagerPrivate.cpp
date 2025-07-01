#include "NXDxgiManagerPrivate.h"

NXDxgiManagerPrivate::NXDxgiManagerPrivate(QObject* parent)
    : QObject{parent}
{
#ifdef Q_OS_WIN
#else
    qCritical("Critical Error: NXDxgiManagerPrivate not implemented on non-Windows platforms");
#endif
}

NXDxgiManagerPrivate::~NXDxgiManagerPrivate()
{
}

NXDxgiScreenPrivate::NXDxgiScreenPrivate(QObject* parent)
    : QObject{parent}
{
#ifdef Q_OS_WIN
#else
    qCritical("Critical Error: NXDxgiScreenPrivate not implemented on non-Windows platforms");
#endif
}

NXDxgiScreenPrivate::~NXDxgiScreenPrivate()
{
}
