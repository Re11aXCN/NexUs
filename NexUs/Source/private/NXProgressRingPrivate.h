﻿#ifndef NXFRAMEWORK_NEXUS_PRIVATE_NXPROGRESSRINGPRIVATE_H_
#define NXFRAMEWORK_NEXUS_PRIVATE_NXPROGRESSRINGPRIVATE_H_

#include <QObject>

#include "NXDef.h"
class NXProgressRing;
class QPropertyAnimation;
class NXProgressRingPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXProgressRing);
    Q_PROPERTY_CREATE_D(bool, IsBusying)
    Q_PROPERTY_CREATE_D(bool, IsTransparent)
    Q_PROPERTY_CREATE_D(bool, IsDisplayValue)
    Q_PROPERTY_CREATE_D(NXProgressRingType::ValueDisplayMode, ValueDisplayMode)
    Q_PROPERTY_CREATE_D(int, BusyingWidth)
    Q_PROPERTY_CREATE_D(int, BusyingDurationTime)
    Q_PROPERTY_CREATE_D(int, Minimum)
    Q_PROPERTY_CREATE_D(int, Maximum)
    Q_PROPERTY_CREATE_D(int, Value)
    Q_PROPERTY_CREATE_D(int, ValuePixelSize)
    Q_PROPERTY_CREATE(int, BusyIndex)
    Q_PROPERTY_CREATE(int, BusyStartDeg)
    Q_PROPERTY_CREATE(int, BusyContentDeg)
public:
    explicit NXProgressRingPrivate(QObject* parent = nullptr);
    ~NXProgressRingPrivate() override;

private:
    NXThemeType::ThemeMode _themeMode;
    QPropertyAnimation* _busyStartDegAnimation{nullptr};
    QPropertyAnimation* _busyContentDegAnimation{nullptr};
};

#endif //NXFRAMEWORK_NEXUS_PRIVATE_NXPROGRESSRINGPRIVATE_H_
