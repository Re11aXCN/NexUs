﻿#include "NXSuggestDelegate.h"

#include <QPainter>
#include <QPainterPath>

#include "private/NXSuggestBoxPrivate.h"
#include "NXSuggestModel.h"
#include "NXTheme.h"
NXSuggestDelegate::NXSuggestDelegate(QObject* parent)
    : QStyledItemDelegate{parent}
{
    _themeMode = nxTheme->getThemeMode();
    QObject::connect(nxTheme, &NXTheme::themeModeChanged, this, [=](NXThemeType::ThemeMode themeMode) { _themeMode = themeMode; });
}

NXSuggestDelegate::~NXSuggestDelegate()
{
}

void NXSuggestDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    int margin = 2;
    QStyleOptionViewItem viewOption(option);
    initStyleOption(&viewOption, index);

    NXSuggestModel* model = dynamic_cast<NXSuggestModel*>(const_cast<QAbstractItemModel*>(index.model()));
    NXSuggestion* suggest = model->getSearchSuggestion(index.row());
    if (option.state.testFlag(QStyle::State_HasFocus))
    {
        viewOption.state &= ~QStyle::State_HasFocus;
    }
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    QPainterPath path;
    QRect optionRect = option.rect;
    optionRect.adjust(margin * 2, margin, -margin * 2, -margin);
    path.addRoundedRect(optionRect, 8, 8);
    if (option.state & QStyle::State_Selected)
    {
        if (option.state & QStyle::State_MouseOver)
        {
            //选中时覆盖
            painter->fillPath(path, NXThemeColor(_themeMode, BasicSelectedHoverAlpha));
        }
        else
        {
            //选中
            painter->fillPath(path, NXThemeColor(_themeMode, BasicSelectedAlpha));
        }
    }
    else
    {
        if (option.state & QStyle::State_MouseOver)
        {
            //覆盖时颜色
            painter->fillPath(path, NXThemeColor(_themeMode, BasicHoverAlpha));
        }
    }
    //文字绘制
    painter->setPen(NXThemeColor(_themeMode, BasicText));
    painter->drawText(option.rect.x() + 37, option.rect.y() + 25, suggest->getSuggestText());

    //图标绘制
    if (suggest->getNXIcon() != NXIconType::None)
    {
        QFont iconFont = QFont("NXAwesome");
        iconFont.setPixelSize(17);
        painter->setFont(iconFont);
        painter->drawText(option.rect.x() + 11, option.rect.y() + 26, QChar((unsigned short)suggest->getNXIcon()));
    }
    painter->restore();
}

QSize NXSuggestDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    size.setHeight(40);
    return size;
}
