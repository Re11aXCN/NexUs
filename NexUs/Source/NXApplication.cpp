#include "NXApplication.h"

#include <QApplication>
#include <QCursor>
#include <QFontDatabase>
#include <QWidget>

#include "NXTheme.h"
#include "private/NXApplicationPrivate.h"
Q_SINGLETON_CREATE_CPP(NXApplication)
NXApplication::NXApplication(QObject* parent)
    : QObject{parent}, d_ptr(new NXApplicationPrivate())
{
    Q_D(NXApplication);
    d->q_ptr = this;
    d->_pIsEnableMica = false;
    d->_pMicaImagePath = ":/Resource/Image/MicaBase.png";
    d->_themeMode = nxTheme->getThemeMode();
    QObject::connect(nxTheme, &NXTheme::themeModeChanged, d, &NXApplicationPrivate::onxThemeModeChanged);
}

NXApplication::~NXApplication()
{
}

void NXApplication::setIsEnableMica(bool isEnable)
{
    Q_D(NXApplication);
    d->_pIsEnableMica = isEnable;
    if (isEnable)
    {
        d->_initMicaBaseImage(QImage(d->_pMicaImagePath));
    }
    else
    {
        d->onxThemeModeChanged(d->_themeMode);
        Q_EMIT pIsEnableMicaChanged();
    }
}
bool NXApplication::getIsEnableMica() const
{
    Q_D(const NXApplication);
    return d->_pIsEnableMica;
}
void NXApplication::setMicaImagePath(QString micaImagePath)
{
    Q_D(NXApplication);
    d->_pMicaImagePath = micaImagePath;
    d->_initMicaBaseImage(QImage(d->_pMicaImagePath));
    Q_EMIT pMicaImagePathChanged();
}
QString NXApplication::getMicaImagePath() const
{
    Q_D(const NXApplication);
    return d->_pMicaImagePath;
}

void NXApplication::init()
{
    Q_INIT_RESOURCE(NexUs);
    QApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    QFontDatabase::addApplicationFont(":/Resource/Font/ElaAwesome.ttf");
    QFontDatabase::addApplicationFont(":/Resource/Font/segoe_slboot_EX.ttf");
    //é»˜è®¤å­—ä½“
    QFont font = qApp->font();
    font.setPixelSize(13);
    font.setFamily("Microsoft YaHei");
    font.setHintingPreference(QFont::PreferNoHinting);
    qApp->setFont(font);
}

void NXApplication::syncMica(QWidget* widget, bool isSync)
{
    Q_D(NXApplication);
    if (!widget)
    {
        return;
    }
    if (isSync)
    {
        widget->installEventFilter(d);
        d->_micaWidgetList.append(widget);
        if (d->_pIsEnableMica)
        {
            d->_updateMica(widget, false);
        }
    }
    else
    {
        widget->removeEventFilter(d);
        d->_micaWidgetList.removeOne(widget);
    }
    d->_updateMica(widget, false);
}


bool NXApplication::containsCursorToItem(QWidget* item)
{
    if (!item || !item->isVisible())
    {
        return false;
    }
    auto point = item->window()->mapFromGlobal(QCursor::pos());
    QRectF rect = QRectF(item->mapTo(item->window(), QPoint(0, 0)), item->size());
    if (rect.contains(point))
    {
        return true;
    }
    return false;
}
