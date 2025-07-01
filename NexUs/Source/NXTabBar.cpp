#include "NXTabBar.h"

#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>

#include "private/NXTabBarPrivate.h"
#include "DeveloperComponents/NXTabBarStyle.h"
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
    setStyle(new NXTabBarStyle(style()));
}

NXTabBar::~NXTabBar()
{
}

void NXTabBar::mousePressEvent(QMouseEvent* event)
{
    Q_D(NXTabBar);
    QTabBar::mousePressEvent(event);
    d->_lastDragPix = grab(tabRect(currentIndex()));
    Q_EMIT tabBarPress(currentIndex());
}

void NXTabBar::mouseMoveEvent(QMouseEvent* event)
{
    Q_D(NXTabBar);
    QRect moveRect = rect();
    moveRect.adjust(0, -height(), 0, height());
    QPoint currentPos = event->pos();
    if (currentPos.y() > moveRect.bottom() || currentPos.y() < moveRect.y())
    {
        QDrag* drag = new QDrag(this);
        drag->setPixmap(d->_lastDragPix);
        qreal pixRatio = d->_lastDragPix.devicePixelRatioF();
        QPoint pixCenter = d->_lastDragPix.rect().center();
        drag->setHotSpot(pixCenter / pixRatio);
        QMimeData* data = new QMimeData;
        data->setProperty("DragType", "NXTabBarDrag");
        data->setProperty("TabIndex", currentIndex());
        data->setProperty("TabIcon", tabIcon(currentIndex()));
        data->setProperty("TabText", tabText(currentIndex()));
        data->setProperty("NXTabBarObject", QVariant::fromValue(this));
        data->setProperty("QDragObject", QVariant::fromValue(drag));
        drag->setMimeData(data);
        Q_EMIT tabDragCreate(drag);
    }
    QTabBar::mouseMoveEvent(event);
}

void NXTabBar::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->property("DragType").toString() == "NXTabBarDrag")
    {
        event->acceptProposedAction();
    }
    QTabBar::dragEnterEvent(event);
}

void NXTabBar::dropEvent(QDropEvent* event)
{
    if (event->mimeData()->property("NXTabBarObject").value<NXTabBar*>() != this)
    {
        event->accept();
        QMimeData* data = const_cast<QMimeData*>(event->mimeData());
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        data->setProperty("TabDropIndex", tabAt(event->position().toPoint()));
#else
        data->setProperty("TabDropIndex", tabAt(event->pos()));
#endif
        Q_EMIT tabDragDrop(data);
    }
    QTabBar::dropEvent(event);
}
