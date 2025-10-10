#include "NXTabWidget.h"

#include "NXTabBar.h"
#include "private/NXTabWidgetPrivate.h"
#include <QDebug>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>
#include <QPainter>
Q_PROPERTY_CREATE_Q_CPP(NXTabWidget, bool, IsTabTransparent);
Q_PROPERTY_CREATE_Q_CPP(NXTabWidget, bool, IsContainerAcceptDrops);
NXTabWidget::NXTabWidget(QWidget* parent)
    : QTabWidget(parent), d_ptr(new NXTabWidgetPrivate())
{
    Q_D(NXTabWidget);
    d->q_ptr = this;
    d->_pIsContainerAcceptDrops = false;
    d->_pIsTabTransparent = false;
    setObjectName("NXTabWidget");
    setAcceptDrops(true);
    d->_tabBar = new NXTabBar(this);
    setTabBar(d->_tabBar);
    QObject::connect(d->_tabBar, &NXTabBar::tabDragCreate, d, &NXTabWidgetPrivate::onTabDragCreate);
    QObject::connect(d->_tabBar, &NXTabBar::tabDragEnter, d, &NXTabWidgetPrivate::onTabDragEnter);
    QObject::connect(d->_tabBar, &NXTabBar::tabDragLeave, d, &NXTabWidgetPrivate::onTabDragLeave);
    QObject::connect(d->_tabBar, &NXTabBar::tabDragDrop, d, &NXTabWidgetPrivate::onTabDragDrop);
    QObject::connect(d->_tabBar, &NXTabBar::tabCloseRequested, d, &NXTabWidgetPrivate::onTabCloseRequested);
}

NXTabWidget::~NXTabWidget()
{
    Q_D(NXTabWidget);
    d->_clearAllTabWidgetList();
}

void NXTabWidget::setTabSize(QSize tabSize)
{
    Q_D(NXTabWidget);
    d->_tabBar->setTabSize(tabSize);
}

QSize NXTabWidget::getTabSize() const
{
    Q_D(const NXTabWidget);
    return d->_tabBar->getTabSize();
}

void NXTabWidget::setTabPosition(TabPosition position)
{
    if (position == QTabWidget::North || position == QTabWidget::South)
    {
        QTabWidget::setTabPosition(position);
    }
}

void NXTabWidget::paintEvent(QPaintEvent* event)
{
    Q_D(NXTabWidget);
    if (!d->_pIsTabTransparent)
    {
        QTabWidget::paintEvent(event);
    }
}

void NXTabWidget::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->property("DragType").toString() == "NXTabBarDrag")
    {
        event->acceptProposedAction();
    }
    QTabWidget::dragEnterEvent(event);
}

void NXTabWidget::dropEvent(QDropEvent* event)
{
    Q_D(NXTabWidget);
    if (d->_pIsContainerAcceptDrops && event->mimeData()->property("NXTabWidgetObject").value<NXTabWidget*>() != this)
    {
        QMimeData* data = const_cast<QMimeData*>(event->mimeData());
        data->setProperty("TabDropIndex", count());
        d->onTabDragDrop(data);
        event->accept();
    }
    QTabWidget::dropEvent(event);
}

void NXTabWidget::tabInserted(int index)
{
    Q_D(NXTabWidget);
    QWidget* tabWidget = widget(index);
    if (!tabWidget->property("IsMetaWidget").toBool() && !tabWidget->property("NXOriginTabWidget").isValid())
    {
        d->_allTabWidgetList.append(widget(index));
    }
    QTabWidget::tabInserted(index);
}