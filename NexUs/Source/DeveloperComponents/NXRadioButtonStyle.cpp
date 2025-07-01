#include "NXRadioButtonStyle.h"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>

#include "NXTheme.h"
NXRadioButtonStyle::NXRadioButtonStyle(QStyle* style)
{
    _themeMode = nxTheme->getThemeMode();
    QObject::connect(nxTheme, &NXTheme::themeModeChanged, this, [=](NXThemeType::ThemeMode themeMode) { _themeMode = themeMode; });
}

NXRadioButtonStyle::~NXRadioButtonStyle()
{
}

void NXRadioButtonStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case PE_IndicatorRadioButton:
    {
        const QStyleOptionButton* bopt = qstyleoption_cast<const QStyleOptionButton*>(option);
        if (!bopt)
        {
            break;
        }
        QRect buttonRect = bopt->rect;
        buttonRect.adjust(1, 1, -1, -1);
        painter->save();
        painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

        if (bopt->state & QStyle::State_Off)
        {
            painter->setPen(QPen(NXThemeColor(_themeMode, BasicBorder), 1.5));
            if (bopt->state & QStyle::State_MouseOver)
            {
                painter->setBrush(NXThemeColor(_themeMode, BasicHover));
            }
            else
            {
                painter->setBrush(NXThemeColor(_themeMode, BasicBase));
            }
            painter->drawEllipse(QPointF(buttonRect.center().x() + 1, buttonRect.center().y() + 1), 7.5, 7.5);
        }
        else
        {
            painter->setPen(Qt::NoPen);
            painter->setBrush(NXThemeColor(_themeMode, PrimaryNormal));
            painter->drawEllipse(QPointF(buttonRect.center().x() + 1, buttonRect.center().y() + 1), buttonRect.width() / 2, buttonRect.width() / 2);
            
            painter->setBrush(NXThemeColor(_themeMode, BasicTextInvert));
            if (bopt->state & QStyle::State_Sunken)
            {
                if (bopt->state & QStyle::State_MouseOver)
                {
                    painter->drawEllipse(QPointF(buttonRect.center().x() + 1, buttonRect.center().y() + 1), buttonRect.width() / 4.5, buttonRect.width() / 4.5);
                }
            }
            else
            {
                if (bopt->state & QStyle::State_MouseOver)
                {
                    painter->drawEllipse(QPointF(buttonRect.center().x() + 1, buttonRect.center().y() + 1), buttonRect.width() / 3.5, buttonRect.width() / 3.5);
                }
                else
                {
                    painter->drawEllipse(QPointF(buttonRect.center().x() + 1, buttonRect.center().y() + 1), buttonRect.width() / 4, buttonRect.width() / 4);
                }
            }
        }
        painter->restore();
        return;
    }
    default:
    {
        break;
    }
    }

    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

int NXRadioButtonStyle::pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
{
    switch (metric)
    {
    case QStyle::PM_ExclusiveIndicatorWidth:
    {
        return 18;
    }
    case QStyle::PM_ExclusiveIndicatorHeight:
    {
        return 18;
    }
    default:
    {
        break;
    }
    }
    return QProxyStyle::pixelMetric(metric, option, widget);
}
