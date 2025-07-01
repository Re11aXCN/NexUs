﻿#include "NXCalendarPickerContainer.h"

#include <QPainter>

#include "NXTheme.h"
NXCalendarPickerContainer::NXCalendarPickerContainer(QWidget* parent)
    : QWidget{parent}
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Popup | Qt::NoDropShadowWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setContentsMargins(0, 0, 0, 0);
    setObjectName("NXCalendarPickerContainer");
    setStyleSheet("#NXCalendarPickerContainer{background-color:transparent}");
    _themeMode = nxTheme->getThemeMode();
    QObject::connect(nxTheme, &NXTheme::themeModeChanged, this, [=](NXThemeType::ThemeMode themeMode) { _themeMode = themeMode; });
}

NXCalendarPickerContainer::~NXCalendarPickerContainer()
{
}

void NXCalendarPickerContainer::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing);
    nxTheme->drawEffectShadow(&painter, rect(), 6, 5);
    painter.restore();
}
