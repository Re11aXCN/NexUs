#include "NXTabWidget.h"

#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>
#include <QPainter>

#include "NXTabBar.h"
#include "private/NXTabWidgetPrivate.h"
NXTabWidget::NXTabWidget(QWidget* parent)
    : QTabWidget(parent), d_ptr(new NXTabWidgetPrivate())
{
    Q_D(NXTabWidget);
    d->q_ptr = this;
    _pIsTabTransparent = false;
    setObjectName("NXTabWidget");
    setAcceptDrops(true);
    NXTabBar* tabBar = new NXTabBar(this);
    setTabBar(tabBar);
    QObject::connect(tabBar, &NXTabBar::tabBarPress, d, &NXTabWidgetPrivate::onTabBarPress);
    QObject::connect(tabBar, &NXTabBar::tabDragCreate, d, &NXTabWidgetPrivate::onTabDragCreate);
    QObject::connect(tabBar, &NXTabBar::tabDragDrop, d, &NXTabWidgetPrivate::onTabDragDrop);
    QObject::connect(tabBar, &NXTabBar::tabCloseRequested, d, &NXTabWidgetPrivate::onTabCloseRequested);
}

NXTabWidget::~NXTabWidget()
{
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
    if (!_pIsTabTransparent)
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
    if (event->mimeData()->property("NXTabWidgetObject").value<NXTabWidget*>() != this)
    {
        QMimeData* data = const_cast<QMimeData*>(event->mimeData());
        data->setProperty("TabDropIndex", count());
        d->onTabDragDrop(event->mimeData());
        event->accept();
    }
    QTabWidget::dropEvent(event);
}
