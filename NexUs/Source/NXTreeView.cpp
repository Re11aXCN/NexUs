#include "NXTreeView.h"

#include "NXScrollBar.h"
#include "private/NXTreeViewPrivate.h"
#include "DeveloperComponents/NXTreeViewStyle.h"
NXTreeView::NXTreeView(QWidget* parent)
    : QTreeView(parent), d_ptr(new NXTreeViewPrivate())
{
    Q_D(NXTreeView);
    d->q_ptr = this;
    setObjectName("NXTreeView");
    setStyleSheet(
        "#NXTreeView{background-color:transparent;}"
        "QHeaderView{background-color:transparent;border:0px;}");

    setAnimated(true);
    setMouseTracking(true);

    NXScrollBar* hScrollBar = new NXScrollBar(this);
    hScrollBar->setIsAnimation(true);
    connect(hScrollBar, &NXScrollBar::rangeAnimationFinished, this, [=]() {
        doItemsLayout();
    });
    setHorizontalScrollBar(hScrollBar);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    NXScrollBar* vScrollBar = new NXScrollBar(this);
    vScrollBar->setIsAnimation(true);
    connect(vScrollBar, &NXScrollBar::rangeAnimationFinished, this, [=]() {
        doItemsLayout();
    });
    setVerticalScrollBar(vScrollBar);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    d->_treeViewStyle = new NXTreeViewStyle(style());
    setStyle(d->_treeViewStyle);
}

NXTreeView::~NXTreeView()
{
}

void NXTreeView::setItemHeight(int itemHeight)
{
    Q_D(NXTreeView);
    if (itemHeight > 0)
    {
        d->_treeViewStyle->setItemHeight(itemHeight);
        doItemsLayout();
    }
}

int NXTreeView::getItemHeight() const
{
    Q_D(const NXTreeView);
    return d->_treeViewStyle->getItemHeight();
}

void NXTreeView::setHeaderMargin(int headerMargin)
{
    Q_D(NXTreeView);
    if (headerMargin >= 0)
    {
        d->_treeViewStyle->setHeaderMargin(headerMargin);
        doItemsLayout();
    }
}

int NXTreeView::getHeaderMargin() const
{
    Q_D(const NXTreeView);
    return d->_treeViewStyle->getHeaderMargin();
}
