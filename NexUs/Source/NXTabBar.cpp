﻿#include "NXTabBar.h"

#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>
#include <private/qtabbar_p.h>

#include "private/NXTabBarPrivate.h"
#include "DeveloperComponents/NXTabBarStyle.h"

#include <QTimer>
NXTabBar::NXTabBar(QWidget* parent)
    : QTabBar(parent), d_ptr(new NXTabBarPrivate())
{
    Q_D(NXTabBar);
    d->q_ptr = this;
    setObjectName("NXTabBar");
    setStyleSheet("#NXTabBar{background-color:transparent;}");
    setTabsClosable(true);
    setMovable(true);
    setAcceptDrops(true);
    d->_style = new NXTabBarStyle(style());
    setStyle(d->_style);
    d->_tabBarPrivate = dynamic_cast<QTabBarPrivate*>(this->QTabBar::d_ptr.data());
}

NXTabBar::~NXTabBar()
{
    Q_D(NXTabBar);
    delete d->_style;
}

void NXTabBar::setTabSize(QSize tabSize)
{
    Q_D(NXTabBar);
    d->_style->setTabSize(tabSize);
}

QSize NXTabBar::getTabSize() const
{
    Q_D(const NXTabBar);
    return d->_style->getTabSize();
}

QSize NXTabBar::sizeHint() const
{
    QSize oldSize = QTabBar::sizeHint();
    QSize newSize = oldSize;
    newSize.setWidth(parentWidget()->width());
    return oldSize.expandedTo(newSize);
}

void NXTabBar::mouseMoveEvent(QMouseEvent* event)
{
    Q_D(NXTabBar);
    QPoint currentPos = event->pos();
    if (objectName() == "NXCustomTabBar" && count() == 1)
    {
        if (!d->_mimeData)
        {
            d->_mimeData = new QMimeData();
            d->_mimeData->setProperty("DragType", "NXTabBarDrag");
            d->_mimeData->setProperty("NXTabBarObject", QVariant::fromValue(this));
            d->_mimeData->setProperty("TabSize", d->_style->getTabSize());
            d->_mimeData->setProperty("IsFloatWidget", true);
            QRect currentTabRect = tabRect(currentIndex());
            d->_mimeData->setProperty("DragPos", QPoint(currentPos.x() - currentTabRect.x(), currentPos.y() - currentTabRect.y()));
            Q_EMIT tabDragCreate(d->_mimeData);
            d->_mimeData = nullptr;
        }
    }
    else
    {
        QRect moveRect = rect();
        moveRect.adjust(0, -height(), 0, height());
        if (currentPos.x() < 0 || currentPos.x() > width() || currentPos.y() > moveRect.bottom() || currentPos.y() < moveRect.y())
        {
            if (!d->_mimeData)
            {
                d->_mimeData = new QMimeData();
                d->_mimeData->setProperty("DragType", "NXTabBarDrag");
                d->_mimeData->setProperty("NXTabBarObject", QVariant::fromValue(this));
                d->_mimeData->setProperty("TabSize", d->_style->getTabSize());
                Q_EMIT tabDragCreate(d->_mimeData);
                d->_mimeData = nullptr;
            }
        }
    }
    QTabBar::mouseMoveEvent(event);
}

void NXTabBar::dragEnterEvent(QDragEnterEvent* event)
{
    Q_D(NXTabBar);
    if (event->mimeData()->property("DragType").toString() == "NXTabBarDrag")
    {
        event->acceptProposedAction();
        auto mimeData = const_cast<QMimeData*>(event->mimeData());
        d->_mimeData = mimeData;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        mimeData->setProperty("TabDropIndex", tabAt(event->position().toPoint()));
#else
        mimeData->setProperty("TabDropIndex", tabAt(event->pos()));
#endif
        Q_EMIT tabDragEnter(mimeData);
        QTimer::singleShot(10, this, [=]() {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            QPoint pressPos(tabRect(currentIndex()).x() + 110, 0);
            QMouseEvent pressEvent(QEvent::MouseButtonPress, pressPos, mapToGlobal(pressPos), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
#else
            QMouseEvent pressEvent(QEvent::MouseButtonPress, QPoint(tabRect(currentIndex()).x() + 110, 0), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
#endif
            QApplication::sendEvent(this, &pressEvent);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            QPoint movePos(event->position().toPoint().x(), 0);
            QMouseEvent moveEvent(QEvent::MouseMove, movePos, mapToGlobal(movePos), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
#else
            QMouseEvent moveEvent(QEvent::MouseMove, QPoint(event->pos().x(), 0), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
#endif
            QApplication::sendEvent(this, &moveEvent);
            });
    }
    QTabBar::dragEnterEvent(event);
}

void NXTabBar::dragMoveEvent(QDragMoveEvent* event)
{
    Q_D(NXTabBar);
    if (event->mimeData()->property("DragType").toString() == "NXTabBarDrag")
    {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        QPoint movePos(event->position().toPoint().x(), 0);
        QMouseEvent moveEvent(QEvent::MouseMove, movePos, mapToGlobal(movePos), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
#else
        QMouseEvent moveEvent(QEvent::MouseMove, QPoint(event->pos().x(), 0), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
#endif
        QApplication::sendEvent(this, &moveEvent);
    }
    QWidget::dragMoveEvent(event);
}

void NXTabBar::dragLeaveEvent(QDragLeaveEvent* event)
{
    Q_D(NXTabBar);
    if (d->_mimeData)
    {
        Q_EMIT tabDragLeave(d->_mimeData);
        d->_mimeData = nullptr;
    }
    QMouseEvent releaseEvent(QEvent::MouseButtonRelease, QPoint(-1, -1), QPoint(-1, -1), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(this, &releaseEvent);
    QTabBar::dragLeaveEvent(event);
}

void NXTabBar::dropEvent(QDropEvent* event)
{
    Q_D(NXTabBar);
    d->_mimeData = nullptr;
    QMouseEvent releaseEvent(QEvent::MouseButtonRelease, QPoint(-1, -1), QPoint(-1, -1), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(this, &releaseEvent);
    if (objectName() != "NXCustomTabBar")
    {
        QMimeData* data = const_cast<QMimeData*>(event->mimeData());
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        data->setProperty("TabDropIndex", tabAt(event->position().toPoint()));
#else
        data->setProperty("TabDropIndex", tabAt(event->pos()));
#endif
        Q_EMIT tabDragDrop(data);
    }
    event->accept();
    QTabBar::dropEvent(event);
}

void NXTabBar::wheelEvent(QWheelEvent* event)
{
    QTabBar::wheelEvent(event);
    event->accept();
}

void NXTabBar::paintEvent(QPaintEvent* event)
{
    Q_D(NXTabBar);
    QSize tabSize = d->_style->getTabSize();
    for (int i = 0; i < d->_tabBarPrivate->tabList.size(); i++)
    {
#if (QT_VERSION > QT_VERSION_CHECK(6, 0, 0))
        d->_tabBarPrivate->tabList[i]->rect = QRect(tabSize.width() * i, d->_tabBarPrivate->tabList[i]->rect.y(), tabSize.width(), tabSize.height());
#else
        d->_tabBarPrivate->tabList[i].rect = QRect(tabSize.width() * i, d->_tabBarPrivate->tabList[i].rect.y(), tabSize.width(), tabSize.height());
#endif
    }
    d->_tabBarPrivate->layoutWidgets();
    QTabBar::paintEvent(event);
}