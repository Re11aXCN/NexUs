#include "NXDxgiManager.h"
#ifdef Q_OS_WIN
#include <d3d11.h>
#include <dxgi1_6.h>
#include <windows.h>
#endif

#include <QApplication>
#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QThread>
#include <QUrl>

#include "DeveloperComponents/NXDxgi.h"
#include "private/NXDxgiManagerPrivate.h"
Q_SINGLETON_CREATE_CPP(NXDxgiManager);
NXDxgiManager::NXDxgiManager(QObject* parent)
    : QObject{parent}, d_ptr(new NXDxgiManagerPrivate())
{
#ifdef Q_OS_WIN
    Q_D(NXDxgiManager);
    d->q_ptr = this;
    d->_dxgiThread = new QThread(this);
    d->_dxgi = new NXDxgi();
    setGrabArea(0, 0, GetSystemMetrics(SM_CXVIRTUALSCREEN), GetSystemMetrics(SM_CYVIRTUALSCREEN));
    bool ret = d->_dxgi->initialize(0, 0);
    if (!ret)
    {
        for (int i = 1; i < d->_dxgi->getDxDeviceList().count(); i++)
        {
            bool ret = d->_dxgi->initialize(i, 0);
            if (ret)
            {
                break;
            }
        }
    }
    if (!ret)
    {
        d->_dxgi->initialize(0, 0);
        qCritical() << "No available screenshot devices";
    }
    d->_dxgi->moveToThread(d->_dxgiThread);
    d->_dxgiThread->start();
    connect(d, &NXDxgiManagerPrivate::grabScreen, d->_dxgi, &NXDxgi::onGrabScreen);
    connect(d->_dxgi, &NXDxgi::grabScreenOver, this, &NXDxgiManager::grabImageUpdate);
#else
    qCritical() << "Critical Error: NXDxgiManager is not supported on non-Windows platforms";
#endif
}

NXDxgiManager::~NXDxgiManager()
{
#ifdef Q_OS_WIN
    Q_D(NXDxgiManager);
    if (d->_dxgi)
    {
        d->_dxgi->setIsGrabActive(false);
    }
    if (d->_dxgiThread->isRunning())
    {
        d->_dxgiThread->quit();
        d->_dxgiThread->wait();
    }
    delete d->_dxgi;
#endif
}

QStringList NXDxgiManager::getDxDeviceList() const
{
#ifdef Q_OS_WIN
    Q_D(const NXDxgiManager);
    return d->_dxgi->getDxDeviceList();
#else
    qCritical() << "Critical Error: NXDxgiManager is not supported on non-Windows platforms";
    return QStringList();
#endif
}

QStringList NXDxgiManager::getOutputDeviceList() const
{
#ifdef Q_OS_WIN
    Q_D(const NXDxgiManager);
    return d->_dxgi->getOutputDeviceList();
#else
    qCritical() << "Critical Error: NXDxgiManager is not supported on non-Windows platforms";
    return QStringList();
#endif
}

QImage NXDxgiManager::grabScreenToImage() const
{
#ifdef Q_OS_WIN
    Q_D(const NXDxgiManager);
    if (!d->_dxgi->getIsInitSuccess())
    {
        return QImage();
    }
    return d->_dxgi->getGrabImage();
#else
    qCritical() << "Critical Error: NXDxgiManager is not supported on non-Windows platforms";
    return QImage();
#endif
}

void NXDxgiManager::startGrabScreen()
{
#ifdef Q_OS_WIN
    Q_D(NXDxgiManager);
    d->_isAllowedGrabScreen = true;
    if (!d->_dxgi->getIsGrabActive())
    {
        d->_dxgi->setIsGrabActive(true);
        Q_EMIT d->grabScreen();
    }
#else
    qCritical() << "Critical Error: NXDxgiManager is not supported on non-Windows platforms";
#endif
}

void NXDxgiManager::stopGrabScreen()
{
#ifdef Q_OS_WIN
    Q_D(NXDxgiManager);
    d->_isAllowedGrabScreen = false;
    d->_dxgi->setIsGrabActive(false);
#else
    qCritical() << "Critical Error: NXDxgiManager is not supported on non-Windows platforms";
#endif
}

bool NXDxgiManager::getIsGrabScreen() const
{
#ifdef Q_OS_WIN
    Q_D(const NXDxgiManager);
    return d->_dxgi->getIsGrabActive();
#else
    qCritical() << "Critical Error: NXDxgiManager is not supported on non-Windows platforms";
    return false;
#endif
}

bool NXDxgiManager::setDxDeviceID(int dxID)
{
#ifdef Q_OS_WIN
    Q_D(NXDxgiManager);
    if (dxID < 0 || d->_dxgi->getDxDeviceList().count() <= dxID)
    {
        return false;
    }
    d->_dxgi->setIsGrabActive(false);
    while (!d->_dxgi->getIsGrabStoped())
    {
        //等待任务结束
        QApplication::processEvents();
    }
    if (d->_dxgi->initialize(dxID, d->_dxgi->getOutputDeviceID()))
    {
        if (d->_isAllowedGrabScreen)
        {
            d->_dxgi->setIsGrabActive(true);
            Q_EMIT d->grabScreen();
        }
        return true;
    }
    return false;
#else
    qCritical() << "Critical Error: NXDxgiManager is not supported on non-Windows platforms";
    return false;
#endif
}

int NXDxgiManager::getDxDeviceID() const
{
#ifdef Q_OS_WIN
    Q_D(const NXDxgiManager);
    return d->_dxgi->getDxDeviceID();
#else
    qCritical() << "Critical Error: NXDxgiManager is not supported on non-Windows platforms";
    return -1;
#endif
}

bool NXDxgiManager::setOutputDeviceID(int deviceID)
{
#ifdef Q_OS_WIN
    Q_D(NXDxgiManager);
    if (deviceID < 0 || d->_dxgi->getOutputDeviceList().count() <= deviceID)
    {
        return false;
    }

    d->_dxgi->setIsGrabActive(false);
    while (!d->_dxgi->getIsGrabStoped())
    {
        //等待任务结束
        QApplication::processEvents();
    }
    if (d->_dxgi->initialize(d->_dxgi->getDxDeviceID(), deviceID))
    {
        if (d->_isAllowedGrabScreen)
        {
            d->_dxgi->setIsGrabActive(true);
            Q_EMIT d->grabScreen();
        }
        return true;
    }
    return false;
#else
    qCritical() << "Critical Error: NXDxgiManager is not supported on non-Windows platforms";
    return false;
#endif
}

int NXDxgiManager::getOutputDeviceID() const
{
#ifdef Q_OS_WIN
    Q_D(const NXDxgiManager);
    return d->_dxgi->getOutputDeviceID();
#else
    qCritical() << "Critical Error: NXDxgiManager is not supported on non-Windows platforms";
    return -1;
#endif
}

void NXDxgiManager::setGrabArea(int width, int height)
{
#ifdef Q_OS_WIN
    Q_D(NXDxgiManager);
    int maxWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    int maxHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);
    if (width <= 0 || width > maxWidth)
    {
        width = maxWidth;
    }
    if (height <= 0 || height > maxHeight)
    {
        height = maxHeight;
    }
    d->_dxgi->setIsGrabCenter(true);
    d->_dxgi->setGrabArea(QRect(0, 0, width, height));
#else
    qCritical() << "Critical Error: NXDxgiManager is not supported on non-Windows platforms";
#endif
}

void NXDxgiManager::setGrabArea(int x, int y, int width, int height)
{
#ifdef Q_OS_WIN
    Q_D(NXDxgiManager);
    int maxWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    int maxHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);
    if (width <= 0 || width > maxWidth)
    {
        width = maxWidth;
    }
    if (height <= 0 || height > maxHeight)
    {
        height = maxHeight;
    }
    d->_dxgi->setIsGrabCenter(false);
    d->_dxgi->setGrabArea(QRect(x, y, width, height));
#else
    qCritical() << "Critical Error: NXDxgiManager is not supported on non-Windows platforms";
#endif
}

QRect NXDxgiManager::getGrabArea() const
{
#ifdef Q_OS_WIN
    Q_D(const NXDxgiManager);
    return d->_dxgi->getGrabArea();
#else
    qCritical() << "Critical Error: NXDxgiManager is not supported on non-Windows platforms";
    return QRect();
#endif
}

void NXDxgiManager::setGrabFrameRate(int frameRateValue)
{
#ifdef Q_OS_WIN
    Q_D(NXDxgiManager);
    if (frameRateValue > 0)
    {
        d->_dxgi->setGrabFrameRate(frameRateValue);
    }
#else
    qCritical() << "Critical Error: NXDxgiManager is not supported on non-Windows platforms";
#endif
}

int NXDxgiManager::getGrabFrameRate() const
{
#ifdef Q_OS_WIN
    Q_D(const NXDxgiManager);
    return d->_dxgi->getGrabFrameRate();
#else
    qCritical() << "Critical Error: NXDxgiManager is not supported on non-Windows platforms";
    return -1;
#endif
}

void NXDxgiManager::setTimeoutMsValue(int timeoutValue)
{
#ifdef Q_OS_WIN
    Q_D(NXDxgiManager);
    if (timeoutValue > 0)
    {
        d->_dxgi->setTimeoutMsValue(timeoutValue);
    }
#else
    qCritical() << "Critical Error: NXDxgiManager is not supported on non-Windows platforms";
#endif
}

int NXDxgiManager::getTimeoutMsValue() const
{
#ifdef Q_OS_WIN
    Q_D(const NXDxgiManager);
    return d->_dxgi->getTimeoutMsValue();
#else
    qCritical() << "Critical Error: NXDxgiManager is not supported on non-Windows platforms";
    return -1;
#endif
}

Q_PROPERTY_CREATE_Q_CPP(NXDxgiScreen, int, BorderRadius)
NXDxgiScreen::NXDxgiScreen(QWidget* parent)
    : QWidget(parent), d_ptr(new NXDxgiScreenPrivate())
{
#ifdef Q_OS_WIN
    Q_D(NXDxgiScreen);
    d->q_ptr = this;
    d->_pBorderRadius = 5;
    d->_dxgiManager = NXDxgiManager::getInstance();
    setFixedSize(700, 500);
    connect(d->_dxgiManager, &NXDxgiManager::grabImageUpdate, this, [=](QImage img) {
        if (isVisible())
        {
            d->_img = std::move(img);
            update();
        }
    });
#else
    qCritical() << "Critical Error: NXDxgiScreen is not supported on non-Windows platforms";
#endif
}

NXDxgiScreen::~NXDxgiScreen()
{
}

void NXDxgiScreen::paintEvent(QPaintEvent* event)
{
#ifdef Q_OS_WIN
    Q_D(NXDxgiScreen);
    if (d->_dxgiManager->getIsGrabScreen())
    {
        QPainter painter(this);
        painter.save();
        painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
        QPainterPath path;
        path.addRoundedRect(rect(), d->_pBorderRadius, d->_pBorderRadius);
        painter.drawImage(rect(), d->_img);
        painter.restore();
    }
#endif
}

void NXDxgiScreen::setIsSyncGrabSize(bool isSyncGrabSize)
{
#ifdef Q_OS_WIN
    Q_D(NXDxgiScreen);
    if (isSyncGrabSize)
    {
        setFixedSize(d->_dxgiManager->getGrabArea().size());
    }
#else
    qCritical() << "Critical Error: NXDxgiScreen is not supported on non-Windows platforms";
#endif
}

bool NXDxgiScreen::getIsSyncGrabSize() const
{
#ifdef Q_OS_WIN
    Q_D(const NXDxgiScreen);
    return d->_isSyncGrabSize;
#else
    qCritical() << "Critical Error: NXDxgiScreen is not supported on non-Windows platforms";
    return false;
#endif
}
