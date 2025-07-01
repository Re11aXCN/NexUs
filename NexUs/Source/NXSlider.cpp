#include "NXSlider.h"

#include <QEvent>

#include "DeveloperComponents/NXSliderStyle.h"
#include "private/NXSliderPrivate.h"
NXSlider::NXSlider(QWidget* parent)
    : QSlider(parent), d_ptr(new NXSliderPrivate())
{
    Q_D(NXSlider);
    d->q_ptr = this;
    setOrientation(Qt::Horizontal);
    d->_sliderStyle = std::make_shared<NXSliderStyle>(style());
    setStyle(d->_sliderStyle.get());
}

NXSlider::NXSlider(Qt::Orientation orientation, QWidget* parent)
    : QSlider(orientation, parent), d_ptr(new NXSliderPrivate())
{
    Q_D(NXSlider);
    d->q_ptr = this;
    d->_sliderStyle = std::make_shared<NXSliderStyle>(style());
    setStyle(d->_sliderStyle.get());
}

NXSlider::~NXSlider()
{
}
