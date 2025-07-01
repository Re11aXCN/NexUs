#include "NXSlider.h"

#include <QEvent>

#include "DeveloperComponents/NXSliderStyle.h"
NXSlider::NXSlider(QWidget* parent)
    : QSlider(parent)
{
    setOrientation(Qt::Horizontal);
    setStyle(new NXSliderStyle(style()));
}

NXSlider::NXSlider(Qt::Orientation orientation, QWidget* parent)
    : QSlider(orientation, parent)
{
    setStyle(new NXSliderStyle(style()));
}

NXSlider::~NXSlider()
{
}
