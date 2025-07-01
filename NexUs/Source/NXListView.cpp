#include "NXListView.h"

#include <QMouseEvent>

#include "private/NXListViewPrivate.h"
#include "DeveloperComponents/NXListViewStyle.h"
#include "NXScrollBar.h"
NXListView::NXListView(QWidget* parent)
    : QListView(parent), d_ptr(new NXListViewPrivate())
{
    Q_D(NXListView);
    d->q_ptr = this;
    setObjectName("NXListView");
    setStyleSheet("#NXListView{background-color:transparent;}");
    d->_listViewStyle = new NXListViewStyle(style());
    setStyle(d->_listViewStyle);
    setMouseTracking(true);
    setVerticalScrollBar(new NXScrollBar(this));
    setHorizontalScrollBar(new NXScrollBar(this));
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
}

NXListView::~NXListView()
{
}

void NXListView::setItemHeight(int itemHeight)
{
    Q_D(NXListView);
    if (itemHeight > 0)
    {
        d->_listViewStyle->setItemHeight(itemHeight);
        doItemsLayout();
    }
}

int NXListView::getItemHeight() const
{
    Q_D(const NXListView);
    return d->_listViewStyle->getItemHeight();
}

void NXListView::setIsTransparent(bool isTransparent)
{
    Q_D(NXListView);
    d->_listViewStyle->setIsTransparent(isTransparent);
    update();
}

bool NXListView::getIsTransparent() const
{
    Q_D(const NXListView);
    return d->_listViewStyle->getIsTransparent();
}
