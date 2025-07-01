#include "NXSlider.h"

#include <QMouseEvent>
#include <QStyleOptionSlider>

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

void NXSlider::setOnlySlideChangeValue(bool onlySlideChangeValue)
{
    Q_D(NXSlider);
    d->_onlySlideChangeValue = onlySlideChangeValue;
}

void NXSlider::setAlignToNearestTick(bool alignToNearestTick, const QList<int>& tickPositions)
{
    Q_D(NXSlider);
    d->_tickPositions = tickPositions;
    d->_alignToNearestTick = alignToNearestTick;
}

void NXSlider::mousePressEvent(QMouseEvent* event)
{
    
    if (event->button() == Qt::LeftButton) {
        Q_D(NXSlider);
        if (d->_onlySlideChangeValue)
        {
            QStyleOptionSlider opt;
            initStyleOption(&opt);
            const QRect sliderRect = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
            if (!sliderRect.contains(event->pos())) {
                event->ignore();
                return;
            }
        }
    }
    QSlider::mousePressEvent(event);
}

void NXSlider::mouseMoveEvent(QMouseEvent* event)
{
    QSlider::mouseMoveEvent(event);
}

void NXSlider::mouseReleaseEvent(QMouseEvent* event)
{
    QSlider::mouseReleaseEvent(event);
    Q_D(NXSlider);
    if (d->_alignToNearestTick && d->_tickPositions.size() > 0)
    {
        int currentValue = value();
        int closestValue = d->_tickPositions.first();
        for (int val : d->_tickPositions)
        {
            if (qAbs(val - currentValue) < qAbs(closestValue - currentValue)) {
                closestValue = val;
            }
        }
        if (closestValue != currentValue) {
            setValue(closestValue);
        }
    }
}
