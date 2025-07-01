#include "NXDockWidgetPrivate.h"

#include <QLinearGradient>

#include "NXDockWidget.h"
#include "DeveloperComponents/NXDockWidgetTitleBar.h"
NXDockWidgetPrivate::NXDockWidgetPrivate(QObject* parent)
    : QObject{parent}
{
}

NXDockWidgetPrivate::~NXDockWidgetPrivate()
{
}

void NXDockWidgetPrivate::onThemeModeChanged(NXThemeType::ThemeMode themeMode)
{
    Q_Q(NXDockWidget);
    q->update();
    _themeMode = themeMode;
}
