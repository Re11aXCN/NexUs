﻿#include "NXLCDNumber.h"
#include "private/NXLCDNumberPrivate.h"
#include "DeveloperComponents/NXLCDNumberStyle.h"
#include "NXTheme.h"
#include <QDateTime>
#include <QDebug>
#include <QTimer>
NXLCDNumber::NXLCDNumber(QWidget* parent)
    : QLCDNumber(parent), d_ptr(new NXLCDNumberPrivate)
{
    Q_D(NXLCDNumber);
    d->q_ptr = this;
    d->_pIsUseAutoClock = false;
    d->_pAutoClockFormat = "yyyy-MM-dd hh:mm:ss";
    setDigitCount(d->_pAutoClockFormat.length());
    setSegmentStyle(QLCDNumber::Flat);
    setObjectName("NXLCDNumber");
    setStyleSheet("#NXLCDNumber{background-color:transparent;}");
    d->_lcdNumberStyle = new NXLCDNumberStyle();
    setStyle(d->_lcdNumberStyle);
    d->_clockTimer = new QTimer(this);
    QObject::connect(d->_clockTimer, &QTimer::timeout, this, [=]() {
        display(QDateTime::currentDateTime().toString(d->_pAutoClockFormat));
    });

    d->onThemeModeChanged(nxTheme->getThemeMode());
    QObject::connect(nxTheme, &NXTheme::themeModeChanged, d, &NXLCDNumberPrivate::onThemeModeChanged);
}

NXLCDNumber::NXLCDNumber(uint numDigits, QWidget* parent)
    : NXLCDNumber(parent)
{
    setDigitCount(numDigits);
}

NXLCDNumber::~NXLCDNumber()
{
}

void NXLCDNumber::setIsUseAutoClock(bool isUseAutoClock)
{
    Q_D(NXLCDNumber);
    d->_pIsUseAutoClock = isUseAutoClock;
    if (d->_pIsUseAutoClock)
    {
        setDigitCount(d->_pAutoClockFormat.length());
        display(QDateTime::currentDateTime().toString(d->_pAutoClockFormat));
        d->_clockTimer->start(200);
    }
    else
    {
        d->_clockTimer->stop();
        display("");
    }
    Q_EMIT pIsUseAutoClockChanged();
}

bool NXLCDNumber::getIsUseAutoClock() const
{
    Q_D(const NXLCDNumber);
    return d->_pIsUseAutoClock;
}

void NXLCDNumber::setAutoClockFormat(const QString& autoClockFormat)
{
    Q_D(NXLCDNumber);
    d->_pAutoClockFormat = autoClockFormat;
    setDigitCount(d->_pAutoClockFormat.length());
    Q_EMIT pAutoClockFormatChanged();
}

QString NXLCDNumber::getAutoClockFormat() const
{
    Q_D(const NXLCDNumber);
    return d->_pAutoClockFormat;
}

void NXLCDNumber::setIsTransparent(bool isTransparent)
{
    Q_D(NXLCDNumber);
    d->_lcdNumberStyle->setIsTransparent(isTransparent);
    update();
    Q_EMIT pIsTransparentChanged();
}

bool NXLCDNumber::getIsTransparent() const
{
    Q_D(const NXLCDNumber);
    return d->_lcdNumberStyle->getIsTransparent();
}
