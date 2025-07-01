﻿#ifndef NEXUS_NXLCDNUMBERPRIVATE_H
#define NEXUS_NXLCDNUMBERPRIVATE_H

#include "NXDef.h"

#include <QObject>
class QTimer;
class NXLCDNumber;
class NXLCDNumberStyle;
class NXLCDNumberPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXLCDNumber)
    Q_PROPERTY_CREATE_D(bool, IsUseAutoClock)
    Q_PROPERTY_CREATE_D(QString, AutoClockFormat)
public:
    explicit NXLCDNumberPrivate(QObject* parent = nullptr);
    ~NXLCDNumberPrivate() override;

    Q_SLOT void onThemeModeChanged(NXThemeType::ThemeMode themeMode);

private:
    QTimer* _clockTimer{nullptr};
    NXLCDNumberStyle* _lcdNumberStyle{nullptr};
};

#endif //NEXUS_NXLCDNUMBERPRIVATE_H
