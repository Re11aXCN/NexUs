#include "NXLCDNumberPrivate.h"
#include "NXLCDNumber.h"
#include "NXTheme.h"
#include <QTimer>
NXLCDNumberPrivate::NXLCDNumberPrivate(QObject* parent)
    : QObject(parent)
{
}

NXLCDNumberPrivate::~NXLCDNumberPrivate()
{
}

void NXLCDNumberPrivate::onThemeModeChanged(NXThemeType::ThemeMode themeMode)
{
    Q_Q(NXLCDNumber);
    if (q->isVisible())
    {
        QPalette palette = q->palette();
        palette.setColor(QPalette::WindowText, NXThemeColor(themeMode, BasicText));
        q->setPalette(palette);
    }
    else
    {
        QTimer::singleShot(1, this, [=] {
            QPalette palette = q->palette();
            palette.setColor(QPalette::WindowText, NXThemeColor(themeMode, BasicText));
            q->setPalette(palette);
        });
    }
}
