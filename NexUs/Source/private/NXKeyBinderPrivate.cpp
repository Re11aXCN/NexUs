#include "NXKeyBinderPrivate.h"
#include "NXKeyBinder.h"
#include <QTimer>
NXKeyBinderPrivate::NXKeyBinderPrivate(QObject* parent)
    : QObject(parent)
{
}

NXKeyBinderPrivate::~NXKeyBinderPrivate()
{
}

void NXKeyBinderPrivate::onThemeChanged(NXThemeType::ThemeMode themeMode)
{
    Q_Q(NXKeyBinder);
    _themeMode = themeMode;
    if (q->isVisible())
    {
        _changeTheme();
    }
    else
    {
        QTimer::singleShot(1, this, [=] {
            _changeTheme();
        });
    }
}

void NXKeyBinderPrivate::_changeTheme()
{
    Q_Q(NXKeyBinder);
    QPalette palette = q->palette();
    if (_themeMode == NXThemeType::Light)
    {
        palette.setColor(QPalette::WindowText, Qt::black);
        q->setPalette(palette);
    }
    else
    {
        palette.setColor(QPalette::WindowText, Qt::white);
        q->setPalette(palette);
    }
}
