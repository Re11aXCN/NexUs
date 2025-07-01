﻿#include "NXToolButtonStyle.h"

#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>
#include <QtMath>

#include "NXTheme.h"
NXToolButtonStyle::NXToolButtonStyle(QStyle* style)
{
    _pIsSelected = false;
    _pIsTransparent = true;
    _pExpandIconRotate = 0;
    _pBorderRadius = 4;
    _themeMode = nxTheme->getThemeMode();
    QObject::connect(nxTheme, &NXTheme::themeModeChanged, this, [=](NXThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
}

NXToolButtonStyle::~NXToolButtonStyle()
{
}

void NXToolButtonStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const
{
    switch (control)
    {
    case QStyle::CC_ToolButton:
    {
        // 内容绘制
        if (const QStyleOptionToolButton* bopt = qstyleoption_cast<const QStyleOptionToolButton*>(option))
        {
            if (bopt->arrowType != Qt::NoArrow)
            {
                break;
            }
            QRect toolButtonRect = bopt->rect;
            if (!_pIsTransparent)
            {
                // 边框距离
                toolButtonRect.adjust(1, 1, -1, -1);
            }
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
            painter->setPen(_pIsTransparent ? Qt::transparent : NXThemeColor(_themeMode, BasicBorder));
            // 背景绘制
            if (bopt->state.testFlag(QStyle::State_Enabled))
            {
                if (bopt->state.testFlag(QStyle::State_Sunken))
                {
                    painter->setBrush(_pIsTransparent ? NXThemeColor(_themeMode, BasicPressAlpha) : NXThemeColor(_themeMode, BasicPress));
                    painter->drawRoundedRect(toolButtonRect, _pBorderRadius, _pBorderRadius);
                }
                else
                {
                    if (_pIsSelected)
                    {
                        painter->setBrush(_pIsTransparent ? NXThemeColor(_themeMode, BasicSelectedAlpha) : NXThemeColor(_themeMode, BasicHover));
                        painter->drawRoundedRect(toolButtonRect, _pBorderRadius, _pBorderRadius);
                    }
                    else
                    {
                        if (bopt->state.testFlag(QStyle::State_MouseOver) || bopt->state.testFlag(QStyle::State_On))
                        {
                            painter->setBrush(_pIsTransparent ? NXThemeColor(_themeMode, BasicHoverAlpha) : NXThemeColor(_themeMode, BasicHover));
                            painter->drawRoundedRect(toolButtonRect, _pBorderRadius, _pBorderRadius);
                        }
                        else
                        {
                            if (!_pIsTransparent)
                            {
                                painter->setBrush(NXThemeColor(_themeMode, BasicBase));
                                painter->drawRoundedRect(toolButtonRect, _pBorderRadius, _pBorderRadius);
                                // 底边线绘制
                                painter->setPen(NXThemeColor(_themeMode, BasicBaseLine));
                                painter->drawLine(toolButtonRect.x() + _pBorderRadius, toolButtonRect.y() + toolButtonRect.height(), toolButtonRect.x() + toolButtonRect.width() - _pBorderRadius, toolButtonRect.y() + toolButtonRect.height());
                            }
                        }
                    }
                }
            }
            // 指示器绘制
            _drawIndicator(painter, bopt, widget);

            // 图标绘制
            QRect contentRect = subControlRect(control, bopt, QStyle::SC_ScrollBarAddLine, widget);
            int heightOffset = contentRect.height() * 0.05;
            contentRect.adjust(0, heightOffset, 0, -heightOffset);
            _drawIcon(painter, contentRect, bopt, widget);

            // 文字绘制
            _drawText(painter, contentRect, bopt);
            painter->restore();
        }
        return;
    }
    default:
    {
        break;
    }
    }
    QProxyStyle::drawComplexControl(control, option, painter, widget);
}

QSize NXToolButtonStyle::sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const
{
    if (type == QStyle::CT_ToolButton)
    {
        if (const QStyleOptionToolButton* bopt = qstyleoption_cast<const QStyleOptionToolButton*>(option))
        {
            QSize toolButtonSize = QProxyStyle::sizeFromContents(type, option, size, widget);
            if (bopt->features.testFlag(QStyleOptionToolButton::HasMenu) && !bopt->features.testFlag(QStyleOptionToolButton::MenuButtonPopup))
            {
                toolButtonSize.setWidth(toolButtonSize.width() + _contentMargin + 0.65 * std::min(bopt->iconSize.width(), bopt->iconSize.height()));
            }
            return toolButtonSize;
        }
    }
    return QProxyStyle::sizeFromContents(type, option, size, widget);
}

void NXToolButtonStyle::_drawIndicator(QPainter* painter, const QStyleOptionToolButton* bopt, const QWidget* widget) const
{
    if (bopt->features.testFlag(QStyleOptionToolButton::MenuButtonPopup))
    {
        QRect indicatorRect = subControlRect(QStyle::CC_ToolButton, bopt, QStyle::SC_ScrollBarSubLine, widget);
        // 指示器区域
        if (bopt->state.testFlag(QStyle::State_Enabled) && bopt->activeSubControls.testFlag(QStyle::SC_ScrollBarSubLine))
        {
            painter->setBrush(NXThemeColor(_themeMode, BasicIndicator));
            QPainterPath path;
            path.moveTo(indicatorRect.topLeft());
            path.lineTo(indicatorRect.right() - 4, indicatorRect.y());
            path.arcTo(QRect(indicatorRect.right() - 8, indicatorRect.y(), 8, 8), 90, -90);
            path.lineTo(indicatorRect.right(), indicatorRect.bottom() - 4);
            path.arcTo(QRect(indicatorRect.right() - 8, indicatorRect.bottom() - 8, 8, 8), 0, -90);
            path.lineTo(indicatorRect.bottomLeft());
            path.closeSubpath();
            painter->drawPath(path);
        }
        // 指示器
        painter->setBrush(bopt->state.testFlag(QStyle::State_Enabled) ? NXThemeColor(_themeMode, BasicText) : NXThemeColor(_themeMode, BasicTextDisable));
        QPainterPath indicatorPath;
        qreal indicatorHeight = qCos(30 * M_PI / 180.0) * indicatorRect.width() * 0.85;
        indicatorPath.moveTo(indicatorRect.x() + indicatorRect.width() * 0.15, indicatorRect.center().y());
        indicatorPath.lineTo(indicatorRect.right() - indicatorRect.width() * 0.15, indicatorRect.center().y());
        indicatorPath.lineTo(indicatorRect.center().x(), indicatorRect.center().y() + indicatorHeight / 2);
        indicatorPath.closeSubpath();
        painter->drawPath(indicatorPath);
    }
    else if (bopt->features.testFlag(QStyleOptionToolButton::HasMenu))
    {
        // 展开指示器
        QSize iconSize = bopt->iconSize;
        painter->save();
        QRect toolButtonRect = bopt->rect;
        QFont iconFont = QFont("NXAwesome");
        iconFont.setPixelSize(0.75 * std::min(iconSize.width(), iconSize.height()));
        painter->setFont(iconFont);
        int indicatorWidth = painter->fontMetrics().horizontalAdvance(QChar((unsigned short)NXIconType::AngleDown));
        QRect expandIconRect(toolButtonRect.right() - _contentMargin - indicatorWidth, toolButtonRect.y() + 1, indicatorWidth, toolButtonRect.height());
        painter->setPen(NXThemeColor(_themeMode, BasicText));
        painter->translate(expandIconRect.center().x(), expandIconRect.y() + (qreal)expandIconRect.height() / 2);
        painter->rotate(_pExpandIconRotate);
        painter->translate(-expandIconRect.center().x() - 1, -expandIconRect.y() - (qreal)expandIconRect.height() / 2);
        painter->drawText(expandIconRect, Qt::AlignCenter, QChar((unsigned short)NXIconType::AngleDown));
        painter->restore();
    }
}

void NXToolButtonStyle::_drawIcon(QPainter* painter, QRectF iconRect, const QStyleOptionToolButton* bopt, const QWidget* widget) const
{
    if (bopt->toolButtonStyle != Qt::ToolButtonTextOnly)
    {
        QSize iconSize = bopt->iconSize;
        if (widget->property("NXIconType").toString().isEmpty())
        {
            // 绘制QIcon
            QIcon icon = bopt->icon;
            if (!icon.isNull())
            {
                QPixmap iconPix = icon.pixmap(iconSize,
                                              (bopt->state & State_Enabled) ? QIcon::Normal
                                                                            : QIcon::Disabled,
                                              (bopt->state & State_Selected) ? QIcon::On
                                                                             : QIcon::Off);
                switch (bopt->toolButtonStyle)
                {
                case Qt::ToolButtonIconOnly:
                {
                    painter->drawPixmap(QRect(QPoint(iconRect.x(), iconRect.center().y() - iconSize.height() / 2), iconSize), iconPix);
                    break;
                }
                case Qt::ToolButtonFollowStyle:
                case Qt::ToolButtonTextBesideIcon:
                {
                    painter->drawPixmap(QRect(QPoint(iconRect.x() + _contentMargin, iconRect.center().y() - iconSize.height() / 2), iconSize), iconPix);
                    break;
                }
                 case Qt::ToolButtonTextUnderIcon:
                {
                    // 图片高度、字体高度、上下边距共10
                    QRect buttonRect = bopt->rect;
                    // 图标区域计算
                    const qreal iconHeightRatio = 0.4;  // 图标占按钮高度的40%
                    const qreal iconTopMarginRatio = 0.1; // 图标顶部间距10%

                    // 图标区域
                    QRectF adjustIconRect(
                        buttonRect.center().x() - iconSize.width() / 2,  // 水平居中
                        buttonRect.top() + buttonRect.height() * iconTopMarginRatio, // 顶部留白
                        iconSize.width(),
                        iconSize.height()
                        //buttonRect.height() * iconHeightRatio // 图标高度
                    );
                    if (bopt->features.testFlag(QStyleOptionToolButton::HasMenu) && !bopt->features.testFlag(QStyleOptionToolButton::MenuButtonPopup))
                    {
                        adjustIconRect.setX(buttonRect.x() + _contentMargin
                            + painter->fontMetrics().horizontalAdvance(bopt->text) / 2
                            - painter->fontMetrics().height() / 2);
                    }
                    painter->drawPixmap(adjustIconRect.toRect(), iconPix);
                    break;
                }
                default:
                {
                    break;
                }
                }
            }
        }
        else
        {
            // 绘制NXIcon
            painter->save();
			bool isSelected = bopt->state.testFlag(QStyle::State_Enabled) && _pIsSelected;
            if (bopt->state.testFlag(QStyle::State_Enabled))
            {
                painter->setPen(NXThemeColor(_themeMode, BasicText));
            }
            else
            {
                painter->setPen(NXThemeColor(_themeMode, BasicTextDisable));
            }
            if (isSelected)
                painter->setPen(NXThemeColor(_themeMode, PrimaryPress));
            QFont iconFont = QFont("NXAwesome");
            switch (bopt->toolButtonStyle)
            {
            case Qt::ToolButtonIconOnly:
            {
                iconFont.setPixelSize(0.75 * std::min(iconSize.width(), iconSize.height()));
                painter->setFont(iconFont);
                painter->drawText(iconRect, Qt::AlignCenter, widget->property("NXIconType").toString());
                break;
            }
            case Qt::ToolButtonFollowStyle:
            case Qt::ToolButtonTextBesideIcon:
            {
                QRect adjustIconRect(iconRect.x() + _contentMargin, iconRect.y(), iconSize.width(), iconRect.height());
                iconFont.setPixelSize(0.75 * std::min(iconSize.width(), iconSize.height()));
                painter->setFont(iconFont);
                painter->drawText(adjustIconRect, Qt::AlignCenter, widget->property("NXIconType").toString());
                break;
            }
 			case Qt::ToolButtonTextUnderIcon:
            {
                QRect buttonRect = bopt->rect;
                const qreal iconHeightRatio = 0.4;
                const qreal iconTopMarginRatio = 0.12;

                QRectF adjustIconRect(
                    buttonRect.center().x() - iconSize.width() / 2,
                    buttonRect.top() + buttonRect.height() * iconTopMarginRatio, // 顶部留白
                    iconSize.width(),
                    iconSize.height()
                );
                
                int textWidth = painter->fontMetrics().horizontalAdvance(bopt->text);
                qreal newPixelSize = 0.86 * std::min(iconSize.width(), iconSize.height());
                //qreal ratio = newPixelSize / painter->font().pixelSize();
                iconFont.setPixelSize(newPixelSize);
                painter->setFont(iconFont);
                if (bopt->features.testFlag(QStyleOptionToolButton::HasMenu) && !bopt->features.testFlag(QStyleOptionToolButton::MenuButtonPopup))
                {
                    adjustIconRect.setLeft(buttonRect.left() + _contentMargin + textWidth / 2/*ratio*/ - painter->fontMetrics().height() / 2);/*setRight(adjustIconRect.right()  +10 - _calculateExpandIndicatorWidth(bopt, painter));*/
                    painter->drawText(adjustIconRect, Qt::AlignLeft, widget->property("NXIconType").toString());
                }
                else
                    painter->drawText(adjustIconRect, Qt::AlignHCenter, widget->property("NXIconType").toString());
                break;
            }
            default:
            {
                break;
            }
            }
            painter->restore();
        }
    }
}

void NXToolButtonStyle::_drawText(QPainter* painter, QRect contentRect, const QStyleOptionToolButton* bopt) const
{
    if (!bopt->text.isEmpty())
    {
        bool isSelected = bopt->state.testFlag(QStyle::State_Enabled) && _pIsSelected;
        if (bopt->state.testFlag(QStyle::State_Enabled))
        {
            painter->setPen(NXThemeColor(_themeMode, BasicText));
        }
        else
        {
            painter->setPen(NXThemeColor(_themeMode, BasicTextDisable));
        }
 		if (isSelected)
            painter->setPen(NXThemeColor(_themeMode, PrimaryPress));
        switch (bopt->toolButtonStyle)
        {
        case Qt::ToolButtonTextOnly:
        {
            contentRect.setLeft(contentRect.left() + _contentMargin + 2);
            painter->drawText(contentRect, Qt::AlignLeft | Qt::AlignVCenter, bopt->text);
  			if (isSelected)
            {
                painter->setPen(Qt::NoPen);
                painter->setBrush(NXThemeColor(_themeMode, PrimaryHover));
                QFontMetricsF metrics(painter->font());
                qreal textWidth = metrics.horizontalAdvance(bopt->text);
                painter->drawRoundedRect(QRectF(contentRect.left(), contentRect.bottom() - 1, textWidth, 2), 2, 2);
            }            
			break;
        }
        case Qt::ToolButtonTextBesideIcon:
        {
            painter->drawText(QRect(contentRect.x() + _contentMargin * 2 + bopt->iconSize.width(), contentRect.y(), contentRect.width() - bopt->iconSize.width(), contentRect.height()), Qt::AlignLeft | Qt::AlignVCenter, bopt->text);
            break;
        }
        case Qt::ToolButtonTextUnderIcon:
        {
            QFontMetricsF metrics(painter->font());
            qreal textHeight = metrics.height();
            QRectF textRect(
                bopt->rect.left(),
                bopt->rect.top() + bopt->rect.height() * 0.65,
                bopt->rect.width(),
                textHeight
            );
            if (bopt->features.testFlag(QStyleOptionToolButton::HasMenu) && !bopt->features.testFlag(QStyleOptionToolButton::MenuButtonPopup))
            {
                textRect.setLeft(textRect.left() + _contentMargin);
                painter->drawText(textRect, Qt::AlignLeft, bopt->text);
                //painter->drawText(QPointF(contentRect.left() + 1, contentRect.bottom() - metrics.height() * 0.3), bopt->text);
            }
            else
            {
                painter->drawText(textRect, Qt::AlignHCenter, bopt->text);
                //painter->drawText(QPointF(contentRect.left() + 1 + (contentRect.width() - textWidth) / 2, contentRect.bottom() - metrics.height() * 0.3 - bopt->rect.height() * 0.1), bopt->text);
            }
            break;
        }
        case Qt::ToolButtonFollowStyle:
        {
            break;
        }
        default:
        {
            break;
        }
        }
    }
}

qreal NXToolButtonStyle::_calculateExpandIndicatorWidth(const QStyleOptionToolButton* bopt, QPainter* painter) const
{
    // 展开指示器
    QSize iconSize = bopt->iconSize;
    painter->save();
    QFont iconFont = QFont("NXAwesome");
    iconFont.setPixelSize(0.75 * std::min(iconSize.width(), iconSize.height()));
    painter->setFont(iconFont);
    int indicatorWidth = painter->fontMetrics().horizontalAdvance(QChar((unsigned short)NXIconType::AngleDown));
    painter->restore();
    return indicatorWidth;
}
