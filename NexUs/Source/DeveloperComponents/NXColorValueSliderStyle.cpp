﻿#include "NXColorValueSliderStyle.h"

#include <QPainter>
#include <QPropertyAnimation>
#include <QStyleOptionSlider>

#include "NXTheme.h"
NXColorValueSliderStyle::NXColorValueSliderStyle(QStyle* style)
    : _pIsUseAlpha(false)
{
    _baseGradient = new QLinearGradient(0, 0, 100, 100);
    setProperty("circleRadius", 0.01);
    _themeMode = nxTheme->getThemeMode();
    QObject::connect(nxTheme, &NXTheme::themeModeChanged, this, [=](NXThemeType::ThemeMode themeMode) { _themeMode = themeMode; });
}

NXColorValueSliderStyle::~NXColorValueSliderStyle()
{
    delete _baseGradient;
}

void NXColorValueSliderStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const
{
    switch (control)
    {
    case QStyle::CC_Slider:
    {
        const QStyleOptionSlider* sopt = qstyleoption_cast<const QStyleOptionSlider*>(option);
        if (!sopt)
        {
            break;
        }
        _baseGradient->setFinalStop(widget->rect().bottomRight());
        QColor highValueColor = _pBaseColor.toHsv();
 		if (_pIsUseAlpha)
        {
            highValueColor.setHsv(highValueColor.hue(), highValueColor.saturation(), highValueColor.value(), 255);
        }
        else
        {
            highValueColor.setHsv(highValueColor.hue(), highValueColor.saturation(), 255);
        }
        QColor lowValueColor = _pBaseColor.toHsv();
        if (_pIsUseAlpha)
        {
            lowValueColor.setHsv(lowValueColor.hue(), lowValueColor.saturation(), lowValueColor.value(), 0);
        }
        else
        {
            lowValueColor.setHsv(lowValueColor.hue(), lowValueColor.saturation(), 0);
        }
        _baseGradient->setColorAt(0, highValueColor);
        _baseGradient->setColorAt(1, lowValueColor);
        painter->save();
        painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        QRectF sliderRect = sopt->rect;
        sliderRect.adjust(1.5, 0, -1.5, 0);
        QRect sliderHandleRect = subControlRect(control, sopt, SC_SliderHandle, widget);
        sliderHandleRect.adjust(1, 1, -1, -1);
        // 滑槽
        painter->setPen(Qt::NoPen);
        painter->setBrush(*_baseGradient);
        // 渐变背景
        painter->drawRoundedRect(QRectF(sliderRect.x(), sliderRect.y() + sliderRect.width() / 8, sliderRect.width(), sliderRect.height() - sliderRect.width() / 4), sliderRect.width() / 2, sliderRect.width() / 2);

        // 滑块
        // 外圆形
        painter->setPen(NXThemeColor(_themeMode, BasicBorder));
        painter->setBrush(NXThemeColor(_themeMode, BasicBase));
        painter->drawEllipse(QPointF(sliderHandleRect.center().x() + 1, sliderHandleRect.center().y() + 1), sliderHandleRect.width() / 2, sliderHandleRect.width() / 2);
        // 内圆形
        painter->setPen(Qt::NoPen);
        painter->setBrush(NXThemeColor(_themeMode, PrimaryNormal));
        if (_lastState == 0)
        {
            _lastState = sopt->state;
        }
        if (_circleRadius == 0)
        {
            _circleRadius = sliderHandleRect.width() / 3.8;
        }
        if (sopt->activeSubControls == SC_SliderHandle)
        {
            if (sopt->state & QStyle::State_Sunken)
            {
                if (sopt->state & QStyle::State_MouseOver)
                {
                    if (!_lastState.testFlag(QStyle::State_Sunken))
                    {
                        _startRadiusAnimation(sliderHandleRect.width() / 2.8, sliderHandleRect.width() / 4.5, const_cast<QWidget*>(widget));
                        _lastState = sopt->state;
                    }
                    painter->drawEllipse(QPointF(sliderHandleRect.center().x() + 1, sliderHandleRect.center().y() + 1), _circleRadius, _circleRadius);
                }
            }
            else
            {
                if (sopt->state & QStyle::State_MouseOver)
                {
                    if (!_lastState.testFlag(QStyle::State_MouseOver))
                    {
                        _startRadiusAnimation(_circleRadius, sliderHandleRect.width() / 2.8, const_cast<QWidget*>(widget));
                        _lastState = sopt->state;
                    }
                    if (_lastState.testFlag(QStyle::State_Sunken))
                    {
                        _startRadiusAnimation(_circleRadius, sliderHandleRect.width() / 2.8, const_cast<QWidget*>(widget));
                        _lastState = sopt->state;
                    }
                    painter->drawEllipse(QPointF(sliderHandleRect.center().x() + 1, sliderHandleRect.center().y() + 1), _circleRadius, _circleRadius);
                }
            }
        }
        else
        {
            if (_lastState.testFlag(QStyle::State_MouseOver) || _lastState.testFlag(QStyle::State_Sunken))
            {
                _startRadiusAnimation(_circleRadius, sliderHandleRect.width() / 3.8, const_cast<QWidget*>(widget));
                _lastState &= ~QStyle::State_MouseOver;
                _lastState &= ~QStyle::State_Sunken;
            }
            painter->drawEllipse(QPointF(sliderHandleRect.center().x() + 1, sliderHandleRect.center().y() + 1), _circleRadius, _circleRadius);
        }
        painter->restore();
        return;
    }
    default:
    {
        break;
    }
    }
    QProxyStyle::drawComplexControl(control, option, painter, widget);
}

int NXColorValueSliderStyle::pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
{
    switch (metric)
    {
    case QStyle::PM_SliderLength:
    {
        return 20;
    }
    case QStyle::PM_SliderThickness:
    {
        return 16;
    }
    default:
    {
        break;
    }
    }
    return QProxyStyle::pixelMetric(metric, option, widget);
}

int NXColorValueSliderStyle::styleHint(StyleHint hint, const QStyleOption* option, const QWidget* widget, QStyleHintReturn* returnData) const
{
    if (hint == QStyle::SH_Slider_AbsoluteSetButtons)
    {
        return Qt::LeftButton;
    }
    return QProxyStyle::styleHint(hint, option, widget, returnData);
}

void NXColorValueSliderStyle::_startRadiusAnimation(qreal startRadius, qreal endRadius, QWidget* widget) const
{
    NXColorValueSliderStyle* style = const_cast<NXColorValueSliderStyle*>(this);
    QPropertyAnimation* circleRadiusAnimation = new QPropertyAnimation(style, "circleRadius");
    QObject::connect(circleRadiusAnimation, &QPropertyAnimation::valueChanged, style, [=](const QVariant& value) {
        this->_circleRadius = value.toReal();
        widget->update(); });
    circleRadiusAnimation->setEasingCurve(QEasingCurve::InOutSine);
    circleRadiusAnimation->setStartValue(startRadius);
    circleRadiusAnimation->setEndValue(endRadius);
    circleRadiusAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}
