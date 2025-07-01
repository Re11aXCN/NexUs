#include "NXTextPrivate.h"

#include "NXText.h"
#include <QTimer>

NXTextPrivate::NXTextPrivate(QObject* parent)
    : QObject{parent}
{
}

NXTextPrivate::~NXTextPrivate()
{
}

void NXTextPrivate::onThemeChanged(NXThemeType::ThemeMode themeMode)
{
    Q_Q(NXText);
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

void NXTextPrivate::_changeTheme()
{
    Q_Q(NXText);
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
