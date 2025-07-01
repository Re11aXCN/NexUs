#include "NXBreadcrumbBarDelegate.h"

#include <QPainter>

#include "NXTheme.h"
NXBreadcrumbBarDelegate::NXBreadcrumbBarDelegate(QObject* parent)
    : QStyledItemDelegate{parent}
{
    _themeMode = nxTheme->getThemeMode();
    QObject::connect(nxTheme, &NXTheme::themeModeChanged, this, [=](NXThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
}

NXBreadcrumbBarDelegate::~NXBreadcrumbBarDelegate()
{
}

void NXBreadcrumbBarDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter->setPen(NXThemeColor(_themeMode, BasicText));
    QRect itemRect = option.rect;
    QString breadcrumbDisplayData = index.data(Qt::DisplayRole).toString();
    QString breadcrumbUserData = index.data(Qt::UserRole).toString();
    if (breadcrumbUserData != "LastBreadcrumb")
    {
        if (_pPressIndex == index)
        {
            painter->setPen(NXThemeColor(_themeMode, BasicTextPress));
        }
        else
        {
            if (!(option.state & QStyle::State_MouseOver))
            {
                painter->setPen(NXThemeColor(_themeMode, BasicTextNoFocus));
            }
        }
    }
    if (breadcrumbDisplayData != ">")
    {
        painter->drawText(itemRect, Qt::AlignVCenter | Qt::AlignLeft, breadcrumbDisplayData);
    }
    else
    {
        QFont iconFont = QFont(QStringLiteral("NXAwesome"));
        iconFont.setPixelSize(painter->font().pixelSize() * 0.785);
        painter->setFont(iconFont);
        itemRect.setX(itemRect.x() - itemRect.width() * 0.36);
        painter->drawText(itemRect, Qt::AlignCenter, QChar((unsigned short)NXIconType::AngleRight));
    }
    painter->restore();
}
