#ifndef NXDXGIMANAGERPRIVATE_H
#define NXDXGIMANAGERPRIVATE_H
#include <QObject>

#include "stdafx.h"
#include <QImage>
class NXDxgi;
class NXDxgiManager;
class NXDxgiManagerPrivate : public QObject
{
    Q_OBJECT
        Q_D_CREATE(NXDxgiManager)
public:
    explicit NXDxgiManagerPrivate(QObject* parent = nullptr);
    ~NXDxgiManagerPrivate();

private:
    Q_SIGNAL void grabScreen();
    bool _isAllowedGrabScreen{ false };
    NXDxgi* _dxgi{ nullptr };
    QThread* _dxgiThread{ nullptr };
};

class NXDxgiScreen;
class NXDxgiScreenPrivate : public QObject
{
    Q_OBJECT
        Q_D_CREATE(NXDxgiScreen)
        Q_PROPERTY_CREATE_D(int, BorderRadius)
public:
    explicit NXDxgiScreenPrivate(QObject* parent = nullptr);
    ~NXDxgiScreenPrivate();

private:
    NXDxgiManager* _dxgiManager{ nullptr };
    bool _isSyncGrabSize{ false };
    QImage _img;
};

#endif // NXDXGIMANAGERPRIVATE_H
