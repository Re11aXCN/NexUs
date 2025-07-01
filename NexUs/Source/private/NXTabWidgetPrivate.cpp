#include "NXTabWidgetPrivate.h"

#include <QApplication>
#include <QMimeData>
#include <QMouseEvent>

#include "DeveloperComponents/NXCustomTabWidget.h"
#include "NXTabBar.h"
#include "NXTabWidget.h"
#include "NXToolButton.h"
NXTabWidgetPrivate::NXTabWidgetPrivate(QObject* parent)
    : QObject{parent}
{
}

NXTabWidgetPrivate::~NXTabWidgetPrivate()
{
}

void NXTabWidgetPrivate::onTabBarPress(int index)
{
}

void NXTabWidgetPrivate::onTabDragCreate(QDrag* drag)
{
    Q_Q(NXTabWidget);
    QMimeData* mimeData = drag->mimeData();
    mimeData->setProperty("NXTabWidgetObject", QVariant::fromValue(q));
    int index = mimeData->property("TabIndex").toInt();
    QString tabText = q->tabText(index);
    QIcon tabIcon = q->tabIcon(index);
    QWidget* dragWidget = q->widget(index);
    QVariant originTabWidget = dragWidget->property("NXOriginTabWidget");
    if (!originTabWidget.isValid())
    {
        dragWidget->setProperty("NXOriginTabWidget", QVariant::fromValue<NXTabWidget*>(q));
    }
    mimeData->setProperty("DragWidget", QVariant::fromValue(dragWidget));
    drag->setMimeData(mimeData);
    QMouseEvent releaseEvent(QEvent::MouseButtonRelease, QPoint(-1, -1), QPoint(-1, -1), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(mimeData->property("NXTabBarObject").value<NXTabBar*>(), &releaseEvent);
    if (drag->exec() == Qt::IgnoreAction)
    {
        NXTabBar* originCustomTabBar = mimeData->property("NXTabBarObject").value<NXTabBar*>();
        if (originCustomTabBar && originCustomTabBar->objectName() == "NXCustomTabBar")
        {
            originCustomTabBar->removeTab(index);
        }
        NXTabWidget* originTabWidget = dragWidget->property("NXOriginTabWidget").value<NXTabWidget*>();
        NXCustomTabWidget* floatWidget = new NXCustomTabWidget(originTabWidget);
        NXTabBar* customTabBar = floatWidget->getCustomTabBar();
        dragWidget->setProperty("CurrentCustomBar", QVariant::fromValue<NXTabBar*>(customTabBar));
        NXTabWidget* customTabWidget = floatWidget->getCustomTabWidget();
        customTabWidget->d_ptr->_customTabBar = customTabBar;
        QObject::connect(customTabBar, &NXTabBar::tabBarPress, customTabWidget->d_func(), &NXTabWidgetPrivate::onTabBarPress);
        QObject::connect(customTabBar, &NXTabBar::tabDragCreate, customTabWidget->d_func(), &NXTabWidgetPrivate::onTabDragCreate);
        QObject::connect(customTabBar, &NXTabBar::tabDragDrop, customTabWidget->d_func(), &NXTabWidgetPrivate::onTabDragDrop);
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
    if (_customTabBar)
    {
        dragWidget->setProperty("CurrentCustomBar", QVariant::fromValue<NXTabBar*>(_customTabBar));
        _customTabBar->insertTab(dropIndex, tabIcon, tabText);
    }
}

void NXTabWidgetPrivate::onTabCloseRequested(int index)
{
    Q_Q(NXTabWidget);
    QWidget* closeWidget = q->widget(index);
    NXTabWidget* originTabWidget = closeWidget->property("NXOriginTabWidget").value<NXTabWidget*>();
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
    else
    {
        q->removeTab(index);
        closeWidget->deleteLater();
    }
}
