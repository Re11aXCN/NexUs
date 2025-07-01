#include "NXSpinBoxStyle.h"

#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionSpinBox>

#include "NXTheme.h"
NXSpinBoxStyle::NXSpinBoxStyle(QStyle* style)
{
    _themeMode = nxTheme->getThemeMode();
    connect(nxTheme, &NXTheme::themeModeChanged, this, [=](NXThemeType::ThemeMode themeMode) { _themeMode = themeMode; });
}

NXSpinBoxStyle::~NXSpinBoxStyle()
{
}

void NXSpinBoxStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const
{
    switch (control)
    {
    case QStyle::CC_SpinBox:
    {
        const QStyleOptionSpinBox* sopt = qstyleoption_cast<const QStyleOptionSpinBox*>(option);
        if (!sopt)
        {
            break;
        }
        painter->save();
        painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        //閼冲本娅?
        painter->setPen(NXThemeColor(_themeMode, BasicBorder));
        painter->setBrush(NXThemeColor(_themeMode, BasicBase));
        painter->drawRoundedRect(sopt->rect, 4, 4);
        //濞ｈ濮為幐澶愭尦
        QRect addLineRect = subControlRect(control, sopt, SC_ScrollBarAddLine, widget);
        if (sopt->activeSubControls == SC_ScrollBarAddLine)
        {
            if (sopt->state & QStyle::State_Sunken && sopt->state & QStyle::State_MouseOver)
            {
                painter->setBrush(NXThemeColor(_themeMode, BasicPressAlpha));
            }
            else
            {
                if (sopt->state & QStyle::State_MouseOver)
                {
                    painter->setBrush(NXThemeColor(_themeMode, BasicHoverAlpha));
                }
                else
                {
                    painter->setBrush(NXThemeColor(_themeMode, BasicBaseDeep));
                }
            }
        }
        else
        {
            painter->setBrush(NXThemeColor(_themeMode, BasicBaseDeep));
        }
        QPainterPath addLinePath;
        addLinePath.moveTo(addLineRect.topLeft());
        addLinePath.lineTo(addLineRect.bottomLeft());
        addLinePath.lineTo(addLineRect.right() - 4, addLineRect.bottom());
        addLinePath.arcTo(QRectF(addLineRect.right() - 8, addLineRect.bottom() - 8, 8, 8), -90, 90);
        addLinePath.lineTo(addLineRect.right(), addLineRect.y() + 4);
        addLinePath.arcTo(QRectF(addLineRect.right() - 8, addLineRect.y(), 8, 8), 0, 90);
        addLinePath.closeSubpath();
        painter->drawPath(addLinePath);

        //閸戝繐鐨幐澶愭尦
        QRect subLineRect = subControlRect(control, sopt, SC_ScrollBarSubLine, widget);
        if (sopt->activeSubControls == SC_ScrollBarSubLine)
        {
            if (sopt->state & QStyle::State_Sunken && sopt->state & QStyle::State_MouseOver)
            {
                painter->setBrush(NXThemeColor(_themeMode, BasicPressAlpha));
            }
            else
            {
                if (sopt->state & QStyle::State_MouseOver)
                {
                    painter->setBrush(NXThemeColor(_themeMode, BasicHoverAlpha));
                }
                else
                {
                    painter->setBrush(NXThemeColor(_themeMode, BasicBaseDeep));
                }
            }
        }
        else
        {
            painter->setBrush(NXThemeColor(_themeMode, BasicBaseDeep));
        }
        QPainterPath subLinePath;
        subLinePath.moveTo(subLineRect.topRight());
        subLinePath.lineTo(subLineRect.x() + 4, subLineRect.y());
        subLinePath.arcTo(QRectF(subLineRect.x(), subLineRect.y(), 8, 8), 90, 90);
        subLinePath.lineTo(subLineRect.x(), subLineRect.bottom() - 4);
        subLinePath.arcTo(QRectF(subLineRect.x(), subLineRect.bottom() - 8, 8, 8), 180, 90);
        subLinePath.lineTo(subLineRect.bottomRight());
        subLinePath.closeSubpath();
        painter->drawPath(subLinePath);
        //鎼存洝绔熺痪?
        if (sopt->state & QStyle::State_HasFocus)
        {
            painter->setPen(QPen(NXThemeColor(_themeMode, PrimaryNormal), 2));
            painter->drawLine(subLineRect.right() + 1, subLineRect.y() + subLineRect.height() - 2, addLineRect.left() - 1, subLineRect.y() + subLineRect.height() - 2);
        }
        else
        {
            painter->setPen(NXThemeColor(_themeMode, BasicHemline));
            painter->drawLine(subLineRect.right() + 1, subLineRect.y() + subLineRect.height() - 1, addLineRect.left() - 1, subLineRect.y() + subLineRect.height() - 1);
        }

        //濞ｈ濮為崶鐐垼
        QFont iconFont = QFont("NXAwesome");
        iconFont.setPixelSize(17);
        painter->setFont(iconFont);
        painter->setPen(NXThemeColor(_themeMode, BasicText));
        painter->drawText(addLineRect, Qt::AlignCenter, QChar((unsigned short)NXIconType::Plus));
        //閸戝繐鐨崶鐐垼
        painter->drawText(subLineRect, Qt::AlignCenter, QChar((unsigned short)NXIconType::Minus));
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

QRect NXSpinBoxStyle::subControlRect(ComplexControl cc, const QStyleOptionComplex* opt, SubControl sc, const QWidget* widget) const
{
    QRect rect = QProxyStyle::subControlRect(cc, opt, sc, widget);
    switch (cc)
    {
    case CC_SpinBox:
    {
        switch (sc)
        {
        case SC_ScrollBarAddLine:
        {
            //婢х偛濮為幐澶愭尦
            QRect spinBoxRect = QProxyStyle::subControlRect(cc, opt, SC_SpinBoxFrame, widget);
            return QRect(spinBoxRect.width() - spinBoxRect.height(), 0, spinBoxRect.height(), spinBoxRect.height());
        }
        case SC_ScrollBarSubLine:
        {
            //閸戝繐鐨幐澶愭尦
            QRect spinBoxRect = QProxyStyle::subControlRect(cc, opt, SC_SpinBoxFrame, widget);
            return QRect(0, 0, spinBoxRect.height(), spinBoxRect.height());
        }
        case SC_SpinBoxEditField:
        {
            QRect spinBoxRect = QProxyStyle::subControlRect(cc, opt, SC_SpinBoxFrame, widget);
            return QRect(spinBoxRect.height(), 0, spinBoxRect.width() - 2 * spinBoxRect.height(), spinBoxRect.height());
        }
        default:
        {
            break;
        }
        }
        break;
    }
    default:
    {
        break;
    }
    }
    return rect;
}
