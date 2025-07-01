#ifndef NXSLIDER_H
#define NXSLIDER_H

#include <QSlider>

#include "stdafx.h"
class NXSliderPrivate;
class NX_EXPORT NXSlider : public QSlider
{
    Q_OBJECT
    Q_Q_CREATE(NXSlider)
public:
    explicit NXSlider(QWidget* parent = nullptr);
    explicit NXSlider(Qt::Orientation orientation, QWidget* parent = nullptr);
    ~NXSlider();
};

#endif // NXSLIDER_H
