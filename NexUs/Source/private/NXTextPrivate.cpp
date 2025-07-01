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
    palette.setColor(QPalette::WindowText, _themeMode == NXThemeType::Light ? Qt::black : Qt::white);
    q->setPalette(palette);
}
