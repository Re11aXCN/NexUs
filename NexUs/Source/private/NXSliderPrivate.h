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
    std::shared_ptr<NXSliderStyle> _sliderStyle{ nullptr };
};

#endif // NXSLIDERPRIVATE_H
