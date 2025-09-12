#include "NXTabWidget.h"

#include "NXTabBar.h"
#include "private/NXTabWidgetPrivate.h"
#include <QDebug>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>
#include <QPainter>
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
    QObject::connect(tabBar, &NXTabBar::tabDragCreate, d, &NXTabWidgetPrivate::onTabDragCreate);
    QObject::connect(tabBar, &NXTabBar::tabDragDrop, d, &NXTabWidgetPrivate::onTabDragDrop);
    QObject::connect(tabBar, &NXTabBar::tabCloseRequested, d, &NXTabWidgetPrivate::onTabCloseRequested);
}

NXTabWidget::~NXTabWidget()
{
    Q_D(NXTabWidget);
    /*for (auto widget : d->_allTabWidgetList)
    {
        auto originTabWidgetVariant = widget->property("NXOriginTabWidget");
        if (originTabWidgetVariant.isValid() && originTabWidgetVariant.value<NXTabWidget*>() == this)
        {
            widget->setProperty("NXOriginTabWidget", QVariant());
        }
    }*/
    d->_allTabWidgetList.clear();
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

/*
 * qt 源码: https://github.com/qt/qtbase/blob/dev/src/widgets/kernel
 * NXCustomTabWidget(部持有NXTabWidget) 之间的Tab拖拽交互, 
 * 被拖拽的 标签页窗口 指针所有权转移到目标NXCustomTabWidget中, _allTabWidgetList存储
 * 
 * NXCustomTabWidget的 标签页关闭, call onTabCloseRequested():
 * 1. 关闭的标签页如果不是NXOriginTabWidget原有标签页, 移除 并 可以deleteLater
 * 2. 关闭的标签页如果是NXOriginTabWidget原有标签页, 移除, 归还标签页指针所有权指针给NXOriginTabWidget, 由Qt内部进行管理, 不能deleteLater
 * NXCustomTabWidget的 窗口关闭
 * 1. 将NXOriginTabWidget原有标签页移除归还, 由Qt内部进行管理, 无需关心内存泄漏
 * 2. 由本身创建的标签页, 因为内部维护 NXTabWidget* _customTabWidget, 创建期间call addTab() qt内置方法,
 *    其内部void QLayout::addChildWidget(QWidget *w)已经设置了setParent父类, 因此无需deleteLater, 由Qt内部进行管理, 无需关心内存泄漏
 *    具体为QTabWidget由QStackedWidget实现QStackedWidget持有QStackedLayout, QStackedLayout继承QLayout,
 *    QStackedLayout的父对象parentWidget是QStackedWidget, 标签页将会被setParent(QStackedWidget)
 * 
 * 因此 NXCustomTabWidget 的析构不需要 将标签页窗口deleteLater
 */
void NXTabWidget::tabInserted(int index)
{
    Q_D(NXTabWidget);
    QWidget* tabWidget = widget(index);
    if (!tabWidget->property("NXOriginTabWidget").isValid())
    {
        d->_allTabWidgetList.append(widget(index));
    }
    QTabWidget::tabInserted(index);
}
