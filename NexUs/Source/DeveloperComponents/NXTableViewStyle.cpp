#include "NXTableViewStyle.h"

#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionViewItem>

#include "NXTableView.h"
#include "NXTheme.h"
NXTableViewStyle::NXTableViewStyle(QStyle* style)
{
    _pHeaderMargin = 6;
    _pCurrentHoverRow = -1;
    _pIsDrawAlternateRowsEnabled = true;
    _pIsSelectionEffectsEnabled = true;
    _pBorderRadius = 3;
    _themeMode = nxTheme->getThemeMode();
    QObject::connect(nxTheme, &NXTheme::themeModeChanged, this, [=](NXThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
}

NXTableViewStyle::~NXTableViewStyle()
{
}

void NXTableViewStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    //qDebug() << element << option->rect << widget->objectName();
    switch (element)
    {
    case QStyle::PE_PanelItemViewItem:
    {
        // 行覆盖绘制
        if (const QStyleOptionViewItem* vopt = qstyleoption_cast<const QStyleOptionViewItem*>(option))
        {
            const NXTableView* tabView = dynamic_cast<const NXTableView*>(widget);
            if (!tabView)
            {
				tabView = dynamic_cast<const NXTableView*>(widget->focusProxy());
				if(!tabView) return;
            }
            painter->save();
            painter->setRenderHint(QPainter::Antialiasing);
            QAbstractItemView::SelectionBehavior selectionBehavior = tabView->selectionBehavior();
            if (selectionBehavior == QAbstractItemView::SelectRows)
            {
                if (vopt->index.row() == _pCurrentHoverRow)
                {
                    painter->setPen(Qt::NoPen);
                    painter->setBrush(NXThemeColor(_themeMode, BasicHoverAlpha));
                    painter->drawRect(vopt->rect);
                }
            }
            else
            {
                if (vopt->state.testFlag(QStyle::State_Selected) || vopt->state.testFlag(QStyle::State_MouseOver))
                {
                    painter->setPen(Qt::NoPen);
                    painter->setBrush(NXThemeColor(_themeMode, BasicHoverAlpha));
                    painter->drawRect(vopt->rect);
                }
            }
            painter->restore();
        }
        return;
    }
    case QStyle::PE_PanelItemViewRow:
    {
        if (const QStyleOptionViewItem* vopt = qstyleoption_cast<const QStyleOptionViewItem*>(option))
        {
            painter->save();
            painter->setRenderHint(QPainter::Antialiasing);
            QRect itemRect = vopt->rect;
            painter->setPen(Qt::NoPen);
			const NXTableView* tabView = qobject_cast<const NXTableView*>(widget);
            if (vopt->state & QStyle::State_Selected)
            {
                if (tabView && !tabView->getDrawSelectionBackground())
                {
                    painter->setBrush(QColor(0, 0, 0, 0)); 
                    painter->drawRect(itemRect);
                }
                else
                {
                    painter->setBrush(NXThemeColor(_themeMode, BasicSelectedAlpha));
                    painter->drawRect(itemRect);
                }
            }
            else
            {
                if (_pIsDrawAlternateRowsEnabled && vopt->features.testFlag(QStyleOptionViewItem::Alternate))
                {
                    // Item背景隔行变色
                    painter->setPen(Qt::NoPen);
                    painter->setBrush(NXThemeColor(_themeMode, BasicAlternating));
                    painter->drawRect(vopt->rect);
                }
            }
            painter->restore();
        }
        return;
    }
    case QStyle::PE_Widget:
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

void NXTableViewStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    //qDebug() << element << option->rect;
    switch (element)
    {
    case QStyle::CE_ShapedFrame:
    {
        // viewport视口外的其他区域背景
        QRect frameRect = option->rect;
        frameRect.adjust(1, 1, -1, -1);
        painter->save();
        painter->setRenderHints(QPainter::Antialiasing);
        painter->setPen(NXThemeColor(_themeMode, PopupBorder));
        painter->setBrush(NXThemeColor(_themeMode, BasicBaseAlpha));
        painter->drawRoundedRect(frameRect, _pBorderRadius, _pBorderRadius);
        painter->restore();
        return;
    }
    case QStyle::CE_HeaderLabel:
    {
        // 表头文字绘制
        if (const QStyleOptionHeader* hopt = qstyleoption_cast<const QStyleOptionHeader*>(option))
        {
            QRect headerRect = option->rect;
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
            if (!hopt->text.isEmpty())
            {
                painter->setPen(NXThemeColor(_themeMode, BasicText));
                painter->drawText(headerRect, hopt->textAlignment, hopt->text);
            }
 			if (!hopt->icon.isNull())
            {
                QIcon::Mode mode = QIcon::Normal;
                QIcon::State state = hopt->state & QStyle::State_Open ? QIcon::On : QIcon::Off;
                QRect iconRect = /*option->rect.adjusted(-3, 0, 0, 0)*/proxy()->subElementRect(QStyle::SE_HeaderLabel, hopt, widget);
                //iconRect.adjust(_horizontalPadding , 0, _horizontalPadding, 0);
                //iconRect.setSize(_pIconSize);
                //iconRect.moveCenter(option->rect.center() - QPoint(option->rect.width() / 2 - iconRect.width() + 3, 0));
                if(_headerAdjustParamMap.contains(hopt->section))
                    iconRect.adjust(_headerAdjustParamMap[hopt->section].x1, _headerAdjustParamMap[hopt->section].y1, _headerAdjustParamMap[hopt->section].x2, _headerAdjustParamMap[hopt->section].y2);
                hopt->icon.paint(painter, iconRect, hopt->textAlignment, mode, state);
            }           
			painter->restore();
        }
        return;
    }
    case QStyle::CE_HeaderSection:
    {
        // 表头背景绘制
        painter->save();
        painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
        painter->setPen(Qt::NoPen);
        painter->setBrush(NXThemeColor(_themeMode, BasicBaseDeepAlpha));
        painter->drawRect(option->rect);
        if (option->state.testFlag(QStyle::State_Sunken))
        {
            painter->setBrush(NXThemeColor(_themeMode, BasicPressAlpha));
        }
        else
        {
            if (option->state.testFlag(QStyle::State_MouseOver))
            {
                painter->setBrush(NXThemeColor(_themeMode, BasicHoverAlpha));
            }
        }
        painter->drawRect(option->rect);
        painter->restore();
        return;
    }
    case QStyle::CE_HeaderEmptyArea:
    {
        // 表头未使用区域背景绘制
        QRect frameRect = option->rect;
        painter->save();
        painter->setRenderHints(QPainter::Antialiasing);
        painter->setPen(Qt::NoPen);
        painter->setBrush(NXThemeColor(_themeMode, BasicBaseDeep));
        painter->drawRect(frameRect);
        painter->restore();
        return;
    }
    case QStyle::CE_ItemViewItem:
    {
        if (const QStyleOptionViewItem* vopt = qstyleoption_cast<const QStyleOptionViewItem*>(option))
        {
            // 背景绘制
            const NXTableView* tabView = dynamic_cast<const NXTableView*>(widget);
            if (!tabView)
            {
                return;
            }
            this->drawPrimitive(QStyle::PE_PanelItemViewItem, option, painter, widget);
			QAbstractItemView::SelectionBehavior selectionBehavior = tabView->selectionBehavior();
            QRect itemRect = option->rect;
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
            QRect checkRect = proxy()->subElementRect(SE_ItemViewItemCheckIndicator, vopt, widget);
            QRect iconRect = proxy()->subElementRect(SE_ItemViewItemDecoration, vopt, widget);
            QRect textRect = proxy()->subElementRect(SE_ItemViewItemText, vopt, widget);
            int column = vopt->index.column();
            if (_adjustParamsMap.contains(column)) {
                NXAdjustParam params = _adjustParamsMap[column];
                textRect.adjust(params.x1, params.y1, params.x2, params.y2);
            }
            if (column == 0)
            {
                iconRect.adjust(_horizontalPadding, 0, _horizontalPadding, 0);
                textRect.adjust(_horizontalPadding, 0, 0, 0);
                if (checkRect.isValid())
                {
                    painter->save();
                    if (vopt->checkState == Qt::Checked)
                    {
                        painter->setPen(Qt::NoPen);
                        painter->setBrush(NXThemeColor(_themeMode, PrimaryNormal));
                        painter->drawRoundedRect(checkRect, 2, 2);
                        QFont iconFont = QFont(QStringLiteral("NXAwesome"));
                        iconFont.setPixelSize(checkRect.width() * 0.85);
                        painter->setFont(iconFont);
                        painter->setPen(NXThemeColor(NXThemeType::Dark, BasicText));
                        painter->drawText(checkRect, Qt::AlignCenter, QChar((unsigned short)NXIconType::Check));
                    }
                    else if (vopt->checkState == Qt::PartiallyChecked)
                    {
                        painter->setPen(Qt::NoPen);
                        painter->setBrush(NXThemeColor(_themeMode, PrimaryNormal));
                        painter->drawRoundedRect(checkRect, 2, 2);
                        painter->setPen(NXThemeColor(NXThemeType::Dark, BasicText));
                        QLine checkLine(checkRect.x() + 3, checkRect.center().y(), checkRect.right() - 3, checkRect.center().y());
                        painter->drawLine(checkLine);
                    }
                    else
                    {
                        painter->setPen(QPen(NXThemeColor(_themeMode, BasicBorderDeep), 1.2));
                        painter->setBrush(Qt::transparent);
                        painter->drawRoundedRect(checkRect, 2, 2);
                    }
                    painter->restore();
                }
            }
            // 图标绘制
            if (!vopt->icon.isNull())
            {
                QIcon::Mode mode = QIcon::Normal;
                // if (!(vopt->state.testFlag(QStyle::State_Enabled)))
                // {
                //     mode = QIcon::Disabled;
                // }
                // else if (vopt->state.testFlag(QStyle::State_Selected))
                // {
                //     mode = QIcon::Selected;
                // }
                QIcon::State state = vopt->state & QStyle::State_Open ? QIcon::On : QIcon::Off;
                vopt->icon.paint(painter, iconRect, vopt->decorationAlignment, mode, state);
            }
            // 文字绘制
            if (!vopt->text.isEmpty())
            {
                painter->setPen(NXThemeColor(_themeMode, BasicText));
                painter->drawText(textRect, vopt->displayAlignment, vopt->text);
            }
            if (_pIsSelectionEffectsEnabled) {
				// 选中特效
				int heightOffset = itemRect.height() / 4;
				painter->setPen(Qt::NoPen);
				painter->setBrush(NXThemeColor(_themeMode, PrimaryNormal));
				if (vopt->state.testFlag(QStyle::State_Selected))
				{
					if (selectionBehavior == QAbstractItemView::SelectRows && vopt->index.column() == 0)
					{
						painter->drawRoundedRect(QRectF(itemRect.x() + 3, itemRect.y() + heightOffset, 3, itemRect.height() - 2 * heightOffset), 3, 3);
					}
				}
            }
            painter->restore();
        }
        return;
    }
    default:
    {
        break;
    }
    }
    QProxyStyle::drawControl(element, option, painter, widget);
}

int NXTableViewStyle::pixelMetric(PixelMetric metric, const QStyleOption* option, const QWidget* widget) const
{
    //qDebug() << metric << QProxyStyle::pixelMetric(metric, option, widget);
    switch (metric)
    {
    case QStyle::PM_HeaderMargin:
    {
        return _pHeaderMargin;
    }
    default:
    {
        break;
    }
    }
    return QProxyStyle::pixelMetric(metric, option, widget);
}

void NXTableViewStyle::setAdjustParams(const QMap<int, NXAdjustParam>& adjustParamMap)
{
    _adjustParamsMap = adjustParamMap;
}

void NXTableViewStyle::setHeaderAdjustParam(const QMap<int, NXAdjustParam>& adjustParamMap)
{
    _headerAdjustParamMap = adjustParamMap;
}

QMap<int, NXAdjustParam> NXTableViewStyle::getAdjustParams() const
{
    return _adjustParamsMap;
}
