#ifndef NXSLIDERPRIVATE_H
#define NXSLIDERPRIVATE_H

#include <QObject>

#include "stdafx.h"

class NXSlider;
class NXSliderStyle;
class NXSliderPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXSlider)
public:
    explicit NXSliderPrivate(QObject* parent = nullptr);
    ~NXSliderPrivate();

private:
    bool _alignToNearestTick{ false };
    bool _onlySlideChangeValue{ false };
    QList<int> _tickPositions;
    std::shared_ptr<NXSliderStyle> _sliderStyle{ nullptr };
};

#endif // NXSLIDERPRIVATE_H
