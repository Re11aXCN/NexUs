#include "NXLineEditStyle.h"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include <QtMath>

#include "NXTheme.h"
NXLineEditStyle::NXLineEditStyle(QStyle* style)
{
    _pLineEditBorderRadius = 6;
    _themeMode = nxTheme->getThemeMode();
    QObject::connect(nxTheme, &NXTheme::themeModeChanged, this, [=](NXThemeType::ThemeMode themeMode) { _themeMode = themeMode; });
}

NXLineEditStyle::~NXLineEditStyle()
{
}

void NXLineEditStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case PE_PanelLineEdit:
    {
        if (const QStyleOptionFrame* fopt = qstyleoption_cast<const QStyleOptionFrame*>(option))
        {
            QRect lineEditRect = fopt->rect;
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing);
            painter->setPen(Qt::NoPen);
            
            painter->setBrush(NXThemeColor(_themeMode, BasicBorder));
            painter->drawRoundedRect(lineEditRect, _pLineEditBorderRadius, _pLineEditBorderRadius);
            
            if (fopt->state & QStyle::State_HasFocus)
            {
                painter->setBrush(NXThemeColor(_themeMode, DialogBase));
            }
            else if (fopt->state & QStyle::State_MouseOver)
            {
                painter->setBrush(NXThemeColor(_themeMode, BasicHover));
            }
            else
            {
                painter->setBrush(NXThemeColor(_themeMode, BasicBase));
            }
            painter->drawRoundedRect(QRectF(lineEditRect.x() + 1.5, lineEditRect.y() + 1.5, lineEditRect.width() - 3, lineEditRect.height() - 3), _pLineEditBorderRadius, _pLineEditBorderRadius);

            painter->setBrush(NXThemeColor(_themeMode, BasicHemline));
            QPainterPath path;
            path.moveTo(6, lineEditRect.height());
            path.lineTo(lineEditRect.width() - 6, lineEditRect.height());
            path.arcTo(QRectF(lineEditRect.width() - 12, lineEditRect.height() - 12, 12, 12), -90, 45);
            path.lineTo(6 - 3 * std::sqrt(2), lineEditRect.height() - (6 - 3 * std::sqrt(2)));
            path.arcTo(QRectF(0, lineEditRect.height() - 12, 12, 12), 225, 45);
            path.closeSubpath();
            painter->drawPath(path);
            painter->restore();
        }
        return;
    }
    default:
    {
        break;
    }
    }
    QProxyStyle::drawPrimitive(element, option, painter, widget);
}
