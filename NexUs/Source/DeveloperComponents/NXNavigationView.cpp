#include "NXNavigationView.h"

#include <QDebug>
#include <QHeaderView>
#include <QModelIndex>
#include <QMouseEvent>
#include <QPainter>
#include <QScrollBar>
#include <QScroller>

#include "NXMenu.h"
#include "NXNavigationModel.h"
#include "NXNavigationNode.h"
#include "NXNavigationStyle.h"
#include "NXScrollBar.h"
#include "NXToolTip.h"

NXNavigationView::NXNavigationView(QWidget* parent)
    : QTreeView(parent)
{
    setObjectName("NXNavigationView");
    setStyleSheet("#NXNavigationView{background-color:transparent;}");
    setAnimated(true);
    setHeaderHidden(true);
    setRootIsDecorated(false);
    setExpandsOnDoubleClick(false);
    setAutoScroll(false);
    setMouseTracking(true);
    setSelectionMode(QAbstractItemView::NoSelection);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    NXScrollBar* vScrollBar = new NXScrollBar(this);
    QObject::connect(vScrollBar, &NXScrollBar::rangeAnimationFinished, this, [=]() {
        doItemsLayout();
    });
    setVerticalScrollBar(vScrollBar);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    NXScrollBar* floatVScrollBar = new NXScrollBar(vScrollBar, this);
    floatVScrollBar->setIsAnimation(true);
    floatVScrollBar->installEventFilter(this);

    _navigationStyle = new NXNavigationStyle(this->style());
    _navigationStyle->setNavigationView(this);
    setStyle(_navigationStyle);

    QScroller::grabGesture(this->viewport(), QScroller::LeftMouseButtonGesture);
    QScroller* scroller = QScroller::scroller(this->viewport());
    QScrollerProperties properties = scroller->scrollerProperties();
    properties.setScrollMetric(QScrollerProperties::MousePressEventDelay, 0);
    properties.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);
    properties.setScrollMetric(QScrollerProperties::VerticalOvershootPolicy, QScrollerProperties::OvershootAlwaysOn);
    properties.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor, 0.35);
    properties.setScrollMetric(QScrollerProperties::OvershootScrollTime, 0.5);
    properties.setScrollMetric(QScrollerProperties::FrameRate, QScrollerProperties::Fps60);
    scroller->setScrollerProperties(properties);

    QObject::connect(scroller, &QScroller::stateChanged, this, [=](QScroller::State newstate) {
        if (newstate == QScroller::Pressed)
        {
            _navigationStyle->setPressIndex(indexAt(mapFromGlobal(QCursor::pos())));
            viewport()->update();
        }
        else if (newstate == QScroller::Scrolling || newstate == QScroller::Inactive)
        {
            _navigationStyle->setPressIndex(QModelIndex());
        }
    });

    setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(this, &NXNavigationView::customContextMenuRequested, this, &NXNavigationView::onCustomContextMenuRequested);
    _compactToolTip = new NXToolTip(this);

}

NXNavigationView::~NXNavigationView()
{
}

void NXNavigationView::navigationNodeStateChange(QVariantMap data)
{
    this->_navigationStyle->navigationNodeStateChange(data);
}

void NXNavigationView::onCustomContextMenuRequested(const QPoint& pos)
{
    QModelIndex posIndex = indexAt(pos);
    if (!posIndex.isValid())
    {
        return;
    }
    NXNavigationNode* posNode = static_cast<NXNavigationNode*>(posIndex.internalPointer());
    if (!posNode->getIsExpanderNode())
    {
        NXMenu menu;
        menu.setMenuItemHeight(27);

        QAction* openAction = menu.addNXIconAction(NXIconType::ObjectGroup, "在新窗口中打开");
        QObject::connect(openAction, &QAction::triggered, this, [=]() {
            Q_EMIT navigationOpenNewWindow(posNode->getNodeKey());
        });
        QAction* closeAction = menu.addNXIconAction(NXIconType::FilmSlash, "关闭当前导航窗口");
        QObject::connect(closeAction, &QAction::triggered, this, [=]() {
            Q_EMIT navigationCloseCurrentWindow(posNode->getNodeKey());
            });
        menu.exec(mapToGlobal(pos));
    }
}

void NXNavigationView::mouseMoveEvent(QMouseEvent* event)
{
    if (width() <= 60)
    {
        QModelIndex posIndex = indexAt(event->pos());
        if (!posIndex.isValid())
        {
            _compactToolTip->hide();
            return;
        }
        NXNavigationNode* posNode = static_cast<NXNavigationNode*>(posIndex.internalPointer());
        _compactToolTip->setToolTip(posNode->getNodeTitle());
        _compactToolTip->updatePos();
        _compactToolTip->show();
    }
    else
    {
        _compactToolTip->hide();
    }
    QTreeView::mouseMoveEvent(event);
}


void NXNavigationView::mouseDoubleClickEvent(QMouseEvent* event)
{
    _navigationStyle->setPressIndex(indexAt(event->pos()));
    viewport()->update();
    QTreeView::mouseDoubleClickEvent(event);
}

void NXNavigationView::mouseReleaseEvent(QMouseEvent* event)
{
    QTreeView::mouseReleaseEvent(event);
    if (event->button() == Qt::LeftButton)
    {
        QModelIndex index = indexAt(event->pos());
        if (index.isValid())
        {
            Q_EMIT navigationClicked(index);
        }
        _navigationStyle->setPressIndex(QModelIndex());
    }
}

bool NXNavigationView::eventFilter(QObject* watched, QEvent* event)
{
    switch (event->type())
    {
    case QEvent::MouseMove:
    case QEvent::HoverMove:
    {
        if (width() <= 60)
        {
            QModelIndex posIndex = indexAt(mapFromGlobal(QCursor::pos()));
            if (!posIndex.isValid())
            {
                _compactToolTip->hide();
                break;
            }
            NXNavigationNode* posNode = static_cast<NXNavigationNode*>(posIndex.internalPointer());
            _compactToolTip->setToolTip(posNode->getNodeTitle());
            _compactToolTip->updatePos();
            _compactToolTip->show();
        }
        else
        {
            _compactToolTip->hide();
        }
        break;
    }
    default:
    {
        break;
    }
    }
    return QAbstractItemView::eventFilter(watched, event);
}
