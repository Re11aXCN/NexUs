#include "NXTabWidgetPrivate.h"

#include "DeveloperComponents/NXCustomTabWidget.h"
#include "NXTabBar.h"
#include "NXTabWidget.h"
#include <QApplication>
#include <QDebug>
#include <QMimeData>
#include <QMouseEvent>
NXTabWidgetPrivate::NXTabWidgetPrivate(QObject* parent)
    : QObject{ parent }
{
}

NXTabWidgetPrivate::~NXTabWidgetPrivate()
{
}

void NXTabWidgetPrivate::onTabDragCreate(QDrag* drag)
{
    Q_Q(NXTabWidget);
    if (!drag || !drag->mimeData()) return;
        
    QMimeData* mimeData = drag->mimeData();
    mimeData->setProperty("NXTabWidgetObject", QVariant::fromValue(q));
    int index = mimeData->property("TabIndex").toInt();
    
    // 验证索引有效性
    if (index < 0 || index >= q->count()) return;
        
    QString tabText = q->tabText(index);
    QIcon tabIcon = q->tabIcon(index);
    QWidget* dragWidget = q->widget(index);
    
    if (!dragWidget) return;
        
    QVariant originTabWidgetVariant = dragWidget->property("NXOriginTabWidget");
    NXTabBar* tabBarObject = mimeData->property("NXTabBarObject").value<NXTabBar*>();
    if (!originTabWidgetVariant.isValid() && q->objectName() != "NXCustomTabWidget")
    {
        dragWidget->setProperty("NXOriginTabWidget", QVariant::fromValue<NXTabWidget*>(q));
    }
    mimeData->setProperty("DragWidget", QVariant::fromValue(dragWidget));
    
    // 发送鼠标释放事件
    if (tabBarObject)
    {
        QMouseEvent releaseEvent(QEvent::MouseButtonRelease, QPoint(-1, -1), QPoint(-1, -1), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
        QApplication::sendEvent(tabBarObject, &releaseEvent);
    }
    if (drag->exec() == Qt::IgnoreAction)
    {
        // 创建新窗口
        NXTabBar* originCustomTabBar = tabBarObject;
        if (originCustomTabBar && originCustomTabBar->objectName() == "NXCustomTabBar")
        {
            originCustomTabBar->removeTab(index);
        }
        QWidget* floatParentWidget = originTabWidgetVariant.value<NXTabWidget*>();
        if (!floatParentWidget)
        {
            floatParentWidget = dragWidget->property("NXFloatParentWidget").value<QWidget*>();
        }
        NXCustomTabWidget* floatWidget = new NXCustomTabWidget(floatParentWidget);
        NXTabBar* customTabBar = floatWidget->getCustomTabBar();
        dragWidget->setProperty("CurrentCustomBar", QVariant::fromValue<NXTabBar*>(customTabBar));
        floatWidget->addTab(dragWidget, tabIcon, tabText);
        floatWidget->show();
        QPoint cursorPoint = QCursor::pos();
        floatWidget->move(cursorPoint.x() - floatWidget->width() / 2, cursorPoint.y() - 40);
    }
}

void NXTabWidgetPrivate::onTabDragDrop(const QMimeData* mimeData)
{
    Q_Q(NXTabWidget);
    QWidget* dragWidget = mimeData->property("DragWidget").value<QWidget*>();
    QString tabText = mimeData->property("TabText").toString();
    QIcon tabIcon = mimeData->property("TabIcon").value<QIcon>();
    NXTabBar* customTabBar = mimeData->property("NXTabBarObject").value<NXTabBar*>();
    int index = mimeData->property("TabIndex").toInt();
    int dropIndex = mimeData->property("TabDropIndex").toInt();
    if (customTabBar && customTabBar->objectName() == "NXCustomTabBar")
    {
        customTabBar->removeTab(index);
    }
    q->insertTab(dropIndex, dragWidget, tabIcon, tabText);
    
    // 如果目标是NXCustomTabWidget，设置相应属性
    if (_customTabBar)
    {
        dragWidget->setProperty("CurrentCustomBar", QVariant::fromValue<NXTabBar*>(_customTabBar));
        _customTabBar->insertTab(dropIndex, tabIcon, tabText);
    }
    else
    {
        // 如果移动到普通NXTabWidget，清除CustomBar属性
        dragWidget->setProperty("CurrentCustomBar", QVariant());
    }
}

void NXTabWidgetPrivate::onTabCloseRequested(int index)
{
    Q_Q(NXTabWidget);
    QWidget* closeWidget = q->widget(index);
    NXTabWidget* originTabWidget = closeWidget->property("NXOriginTabWidget").value<NXTabWidget*>();

    // 情况1：widget有原始TabWidget且不是当前TabWidget，需要还原回去
    if (originTabWidget && originTabWidget != q)
    {
        NXTabBar* customTabBar = closeWidget->property("CurrentCustomBar").value<NXTabBar*>();
        if (customTabBar)
        {
            customTabBar->removeTab(index);
            closeWidget->setProperty("CurrentCustomBar", QVariant::fromValue<NXTabBar*>(nullptr));
        }
        originTabWidget->addTab(closeWidget, q->tabIcon(index), q->tabText(index));
    }
    // 情况2：NXCustomTabWidget中没有原始TabWidget的widget，直接删除
    else if (!originTabWidget && q->objectName() == "NXCustomTabWidget")
    {
        if (_customTabBar)
        {
            _customTabBar->removeTab(index);
        }
        q->removeTab(index);
        _allTabWidgetList.removeAll(closeWidget);
        closeWidget->deleteLater();
    }
    // 情况3：普通TabWidget中的widget或originTabWidget就是当前TabWidget
    else
    {
        q->removeTab(index);
        _allTabWidgetList.removeAll(closeWidget);
        closeWidget->deleteLater();
    }
}
