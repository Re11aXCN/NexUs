﻿#include "NXFlowLayout.h"

#include <QPropertyAnimation>
#include <QWidget>

#include "private/NXFlowLayoutPrivate.h"

NXFlowLayout::NXFlowLayout(QWidget* parent, int margin, int hSpacing, int vSpacing)
    : QLayout(parent), d_ptr(new NXFlowLayoutPrivate())
{
    Q_D(NXFlowLayout);
    d->q_ptr = this;
    d->_hSpacing = hSpacing;
    d->_vSpacing = vSpacing;
    setContentsMargins(margin, margin, margin, margin);
    d->_lastHeightMap.insert(this, 0);
}

NXFlowLayout::NXFlowLayout(int margin, int hSpacing, int vSpacing)
    : d_ptr(new NXFlowLayoutPrivate())
{
    Q_D(NXFlowLayout);
    d->q_ptr = this;
    d->_hSpacing = hSpacing;
    d->_vSpacing = vSpacing;
    setContentsMargins(margin, margin, margin, margin);
    d->_lastHeightMap.insert(this, 0);
}

NXFlowLayout::~NXFlowLayout()
{
    QLayoutItem* item;
    while ((item = NXFlowLayout::takeAt(0)))
    {
        delete item;
    }
}

void NXFlowLayout::addItem(QLayoutItem* item)
{
    Q_D(NXFlowLayout);
    d->_itemList.append(item);
}
int NXFlowLayout::horizontalSpacing() const
{
    Q_D(const NXFlowLayout);
    if (d->_hSpacing >= 0)
    {
        return d->_hSpacing;
    }
    else
    {
        return d->_smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
    }
}

int NXFlowLayout::verticalSpacing() const
{
    Q_D(const NXFlowLayout);
    if (d->_vSpacing >= 0)
    {
        return d->_vSpacing;
    }
    else
    {
        return d->_smartSpacing(QStyle::PM_LayoutVerticalSpacing);
    }
}
int NXFlowLayout::count() const
{
    return d_ptr->_itemList.size();
}

QLayoutItem* NXFlowLayout::itemAt(int index) const
{
    return d_ptr->_itemList.value(index);
}

QLayoutItem* NXFlowLayout::takeAt(int index)
{
    Q_D(NXFlowLayout);
    if (index >= 0 && index < d->_itemList.size())
    {
        return d->_itemList.takeAt(index);
    }
    return nullptr;
}

void NXFlowLayout::setIsAnimation(bool isAnimation)
{
    Q_D(NXFlowLayout);
    d->_isAnimation = isAnimation;
}

Qt::Orientations NXFlowLayout::expandingDirections() const
{
    return {};
}

bool NXFlowLayout::hasHeightForWidth() const
{
    return true;
}

int NXFlowLayout::heightForWidth(int width) const
{
    int height = d_ptr->_doLayout(QRect(0, 0, width, 0), true);
    d_ptr->_lastHeightMap[const_cast<NXFlowLayout*>(this)] = height;
    return height;
}

void NXFlowLayout::setGeometry(const QRect& rect)
{
    Q_D(NXFlowLayout);
    QLayout::setGeometry(rect);
    d->_doLayout(rect, false);
}

QSize NXFlowLayout::sizeHint() const
{
    return minimumSize();
}

QSize NXFlowLayout::minimumSize() const
{
    QSize size;
    for (const QLayoutItem* item : std::as_const(d_ptr->_itemList))
    {
        size = size.expandedTo(item->minimumSize());
    }

    const QMargins margins = contentsMargins();
    size += QSize(margins.left() + margins.right(), margins.top() + margins.bottom());
    return size;
}
