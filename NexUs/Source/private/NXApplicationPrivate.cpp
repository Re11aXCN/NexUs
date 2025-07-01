#include "NXApplicationPrivate.h"
#include <QApplication>
#include <QEvent>
#include <QImage>
#include <QPalette>
#include <QScreen>
#include <QThread>
#include <QWidget>
#include <QtMath>
#include "NXApplication.h"
#include "DeveloperComponents/NXMicaBaseInitObject.h"
NXApplicationPrivate::NXApplicationPrivate(QObject* parent)
    : QObject{parent}
{
}

NXApplicationPrivate::~NXApplicationPrivate()
{
}

void NXApplicationPrivate::onxThemeModeChanged(NXThemeType::ThemeMode themeMode)
{
    _themeMode = themeMode;
    _updateAllMicaWidget();
}
bool NXApplicationPrivate::eventFilter(QObject* watched, QEvent* event)
{
    switch (event->type())
    {
    case QEvent::Show:
    case QEvent::Move:
    case QEvent::Resize:
    {
        if (_pIsEnableMica)
        {
            QWidget* widget = qobject_cast<QWidget*>(watched);
            if (widget)
            {
                _updateMica(widget);
            }
        }
        break;
    }
    case QEvent::Destroy:
    {
        QWidget* widget = qobject_cast<QWidget*>(watched);
        if (widget)
        {
            _micaWidgetList.removeOne(widget);
        }
        break;
    }
    default:
    {
        break;
    }
    }
    return QObject::eventFilter(watched, event);
}
void NXApplicationPrivate::_initMicaBaseImage(QImage img)
{
    Q_Q(NXApplication);
    if (img.isNull())
    {
        return;
    }
    QThread* initThread = new QThread();
    NXMicaBaseInitObject* initObject = new NXMicaBaseInitObject(this);
    QObject::connect(initThread, &QThread::finished, initObject, &NXMicaBaseInitObject::deleteLater);
    QObject::connect(initObject, &NXMicaBaseInitObject::initFinished, initThread, [=]() {
        Q_EMIT q->pIsEnableMicaChanged();
        _updateAllMicaWidget();
        initThread->quit();
        initThread->wait();
        initThread->deleteLater();
        });
    initObject->moveToThread(initThread);
    initThread->start();
    QObject::connect(this, &NXApplicationPrivate::initMicaBase, initObject, &NXMicaBaseInitObject::onInitMicaBase);
    Q_EMIT initMicaBase(img);
}
QRect NXApplicationPrivate::_calculateWindowVirtualGeometry(QWidget* widget)
{
    QRect geometry = widget->geometry();
    qreal xImageRatio = 1, yImageRatio = 1;
    QRect rNXtiveGeometry;
    if (qApp->screens().count() > 1)
    {
        QScreen* currentScreen = qApp->screenAt(geometry.topLeft());
        if (currentScreen)
        {
            QRect screenGeometry = currentScreen->availableGeometry();
            xImageRatio = (qreal)_lightBaseImage.width() / screenGeometry.width();
            yImageRatio = (qreal)_lightBaseImage.height() / screenGeometry.height();
            rNXtiveGeometry = QRect((geometry.x() - screenGeometry.x()) * xImageRatio, (geometry.y() - screenGeometry.y()) * yImageRatio, geometry.width() * xImageRatio, geometry.height() * yImageRatio);
            return rNXtiveGeometry;
        }
    }
    QRect primaryScreenGeometry = qApp->primaryScreen()->availableGeometry();
    xImageRatio = (qreal)_lightBaseImage.width() / primaryScreenGeometry.width();
    yImageRatio = (qreal)_lightBaseImage.height() / primaryScreenGeometry.height();
    rNXtiveGeometry = QRect((geometry.x() - primaryScreenGeometry.x()) * xImageRatio, (geometry.y() - primaryScreenGeometry.y()) * yImageRatio, geometry.width() * xImageRatio, geometry.height() * yImageRatio);
    return rNXtiveGeometry;
}
void NXApplicationPrivate::_updateMica(QWidget* widget, bool isProcessEvent)
{
    if (widget->isVisible())
    {
        QPalette palette = widget->palette();
        if (_themeMode == NXThemeType::Light)
        {
            palette.setBrush(QPalette::Window, _lightBaseImage.copy(_calculateWindowVirtualGeometry(widget)).scaled(widget->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        }
        else
        {
            palette.setBrush(QPalette::Window, _darkBaseImage.copy(_calculateWindowVirtualGeometry(widget)).scaled(widget->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        }
        widget->setPalette(palette);
        if (isProcessEvent)
        {
            QApplication::processEvents();
        }
    }
}
void NXApplicationPrivate::_updateAllMicaWidget()
{
    if (_pIsEnableMica)
    {
        for (auto widget : _micaWidgetList)
        {
            _updateMica(widget, false);
        }
    }
}