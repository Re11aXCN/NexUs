#include "NXMenuStyle.h"

#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOption>

#include "NXMenu.h"
#include "NXTheme.h"
NXMenuStyle::NXMenuStyle(QStyle* style)
{
    _pMenuItemHeight = 32;
    _pBorderRadius = 6;
    _themeMode = nxTheme->getThemeMode();
    QObject::connect(nxTheme, &NXTheme::themeModeChanged, this, [=](NXThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
}

NXMenuStyle::~NXMenuStyle()
{
}

void NXMenuStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case QStyle::PE_PanelMenu:
    {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        nxTheme->drawEffectShadow(painter, option->rect, _shadowBorderWidth, 6);
        
        QRect foregroundRect(_shadowBorderWidth, _shadowBorderWidth, option->rect.width() - 2 * _shadowBorderWidth, option->rect.height() - 2 * _shadowBorderWidth);
        painter->setPen(NXThemeColor(_themeMode, PopupBorder));
        painter->setBrush(NXThemeColor(_themeMode, PopupBase));
        painter->drawRoundedRect(foregroundRect, _pBorderRadius, _pBorderRadius);
        painter->restore();
        return;
    }
    case QStyle::PE_FrameMenu:
    {
        return;
    }
    default:
    {
        break;
    }
    }
    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void NXMenuStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case QStyle::CE_MenuItem:
    {
        if (const QStyleOptionMenuItem* mopt = qstyleoption_cast<const QStyleOptionMenuItem*>(option))
        {
            if (mopt->menuItemType == QStyleOptionMenuItem::Separator)
            {
                QRect separatorRect = mopt->rect;
                painter->save();
                painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
                painter->setPen(Qt::NoPen);
                painter->setBrush(NXThemeColor(_themeMode, BasicBaseLine));
                painter->drawRoundedRect(QRectF(separatorRect.x() + separatorRect.width() * 0.055, separatorRect.center().y(), separatorRect.width() - separatorRect.width() * 0.11, 1.5), 1, 1);
                painter->restore();
                return;
            }
            else
            {
                QRect menuRect = mopt->rect;
                qreal contentPadding = menuRect.width() * 0.055;
                qreal textLeftSpacing = menuRect.width() * 0.082;
                painter->save();
                painter->setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);

                bool isHover = mopt->state.testFlag(QStyle::State_Enabled) && (mopt->state.testFlag(QStyle::State_MouseOver) || mopt->state.testFlag(QStyle::State_Selected));
                if (isHover)
                {
                    QRect hoverRect = menuRect;
                    hoverRect.adjust(0, 2, 0, -2);
                    painter->setPen(Qt::NoPen);
                    painter->setBrush(NXThemeColor(_themeMode, PopupHover));
                    painter->drawRoundedRect(hoverRect, 5, 5);

                    qreal heightOffset = hoverRect.height() / 5;
                    painter->setBrush(NXThemeColor(_themeMode, PrimaryNormal));
                    painter->drawRoundedRect(QRectF(hoverRect.x() + 3, hoverRect.y() + heightOffset, 3, hoverRect.height() - 2 * heightOffset), 3, 3);
                }
                
                QIcon menuIcon = mopt->icon;
                qreal ditherOffset = isHover ? _pMenuItemHeight * 0.021 : 0;
                if (mopt->menuHasCheckableItems)
                {
                    painter->save();
                    painter->setPen(!mopt->state.testFlag(QStyle::State_Enabled) ? Qt::gray : _themeMode == NXThemeType::Light ? Qt::black
                                                                                                                                : Qt::white);
                    QFont iconFont = QFont(QStringLiteral("NXAwesome"));
                    iconFont.setPixelSize(_pMenuItemHeight * 0.57);
                    painter->setFont(iconFont);
                    painter->drawText(QRectF(menuRect.x() + contentPadding, menuRect.y(), _iconWidth, menuRect.height()), Qt::AlignCenter, mopt->checked ? QChar((unsigned short)NXIconType::Check) : QChar((unsigned short)NXIconType::None));
                    painter->restore();
                }
                else
                {
                    QString iconText;
                    const NXMenu* menu = dynamic_cast<const NXMenu*>(widget);
                    if (menu)
                    {
                        QAction* action = menu->actionAt(menuRect.center());
                        if (action)
                        {
                            iconText = action->property("NXIconType").toString();
                        }
                    }
                    if (!iconText.isEmpty())
                    {
                        painter->save();
                        isHover ? painter->setPen(!mopt->state.testFlag(QStyle::State_Enabled) ? Qt::gray : NXThemeColor(_themeMode, PrimaryPress))
                            : painter->setPen(!mopt->state.testFlag(QStyle::State_Enabled) ? Qt::gray : _themeMode == NXThemeType::Light ? Qt::black : Qt::white);
                        QFont iconFont = QFont(QStringLiteral("NXAwesome"));
                        iconFont.setPixelSize(_pMenuItemHeight * 0.57);
                        painter->setFont(iconFont);
                        
                        painter->drawText(QRectF(menuRect.x() + contentPadding + ditherOffset, menuRect.y() - ditherOffset * 1.5, _iconWidth, menuRect.height()), Qt::AlignCenter, iconText);
                        painter->restore();
                    }
                    else
                    {
                        if (!menuIcon.isNull())
                        {
                            painter->drawPixmap(QRect(menuRect.x() + contentPadding, menuRect.center().y() - _iconWidth / 2, _iconWidth, _iconWidth), menuIcon.pixmap(_iconWidth, _iconWidth));
                        }
                    }
                }
                
                if (!mopt->text.isEmpty())
                {
                    QStringList textList = mopt->text.split("\t");
                    isHover ? painter->setPen(!mopt->state.testFlag(QStyle::State_Enabled) ? Qt::gray : NXThemeColor(_themeMode, PrimaryHover))
                        : painter->setPen(!mopt->state.testFlag(QStyle::State_Enabled) ? Qt::gray : _themeMode == NXThemeType::Light ? Qt::black : Qt::white);
                    painter->drawText(QRectF(menuRect.x() + (_isAnyoneItemHasIcon ? contentPadding + textLeftSpacing : 0) + _iconWidth + ditherOffset, menuRect.y() - ditherOffset, menuRect.width(), menuRect.height()), Qt::AlignLeft | Qt::AlignVCenter | Qt::TextSingleLine, textList[0]);
                    if (textList.count() > 1)
                    {
                        painter->drawText(QRectF(menuRect.x() + contentPadding + _iconWidth + textLeftSpacing + ditherOffset, menuRect.y() - ditherOffset, menuRect.width() - (contentPadding * 2 + _iconWidth + textLeftSpacing), menuRect.height()), Qt::AlignRight | Qt::AlignVCenter | Qt::TextSingleLine, textList[1]);
                    }
                }
                
                if (mopt->menuItemType == QStyleOptionMenuItem::SubMenu)
                {
                    painter->save();
                    painter->setPen(!mopt->state.testFlag(QStyle::State_Enabled) ? Qt::gray : _themeMode == NXThemeType::Light ? Qt::black
                                                                                                                                : Qt::white);
                    QFont iconFont = QFont(QStringLiteral("NXAwesome"));
                    iconFont.setPixelSize(18);
                    painter->setFont(iconFont);
                    painter->drawText(QRect(menuRect.right() - 25, menuRect.y(), 25, menuRect.height()), Qt::AlignVCenter, QChar((unsigned short)NXIconType::AngleRight));
                    painter->restore();
                }
                painter->restore();
            }
        }

        return;
    }
    case QStyle::CE_MenuEmptyArea:
    {
        return;
    }
    default:
    {
        break;
    }
    }
    QProxyStyle::drawControl(element, option, painter, widget);
}

int NXMenuStyle::pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
{
    switch (metric)
    {
    case QStyle::PM_SmallIconSize:
    {
        return _iconWidth;
    }
    case QStyle::PM_MenuPanelWidth:
    {
        return _shadowBorderWidth * 1.5;
    }
    default:
    {
        break;
    }
    }
    return QProxyStyle::pixelMetric(metric, option, widget);
}

QSize NXMenuStyle::sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const
{
    switch (type)
    {
    case QStyle::CT_MenuItem:
    {
        if (const QStyleOptionMenuItem* mopt = qstyleoption_cast<const QStyleOptionMenuItem*>(option))
        {
            if (mopt->menuItemType == QStyleOptionMenuItem::Separator)
            {
                break;
            }
            QSize menuItemSize = QProxyStyle::sizeFromContents(type, option, size, widget);
            const NXMenu* menu = dynamic_cast<const NXMenu*>(widget);
            if (menu->isHasIcon() || mopt->menuHasCheckableItems)
            {
                _isAnyoneItemHasIcon = true;
            }
            if (menu->isHasChildMenu())
            {
                return QSize(menuItemSize.width() + 20, _pMenuItemHeight);
            }
            else
            {
                return QSize(menuItemSize.width(), _pMenuItemHeight);
            }
        }
    }
    default:
    {
        break;
    }
    }
    return QProxyStyle::sizeFromContents(type, option, size, widget);
}
