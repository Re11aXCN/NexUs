#include "NXNavigationView.h"

#include <QDebug>
#include <QHeaderView>
#include <QModelIndex>
#include <QMouseEvent>
#include <QPainter>
#include <QScrollBar>
#include <QScroller>
#include <QMimeData>

#include "NXTheme.h"
#include "NXMenu.h"
#include "NXNavigationModel.h"
#include "NXNavigationNode.h"
#include "NXNavigationStyle.h"
#include "NXScrollBar.h"
#include "NXToolTip.h"

NXNavigationView::NXNavigationView(QWidget* parent)
    : QTreeView(parent)
    , _pIsLeftButtonPressedToggleNavigation(false)
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

void NXNavigationView::setNavigationNodeDragAndDropEnable(bool isEnable)
{
    setDragEnabled(isEnable);
    setAcceptDrops(isEnable);
    setDropIndicatorShown(isEnable);
    setDragDropMode(isEnable ? QAbstractItemView::InternalMove : QAbstractItemView::NoDragDrop);
}

QAbstractItemView::DropIndicatorPosition NXNavigationView::dropIndicatorPositionOverride() const
{
    if (!_hoveredIndex.isValid()) {
        return QAbstractItemView::OnViewport; // 当没有有效的索引时，返回 OnViewport
    }

    QRect itemRect = visualRect(_hoveredIndex);
    QPoint cursorPos = mapFromGlobal(QCursor::pos()); // 获取鼠标当前的位置

    if (cursorPos.y() - 9 < itemRect.top()) {
        return QAbstractItemView::AboveItem; // 鼠标在节点上方
    }
    else if (cursorPos.y() + 10 > itemRect.bottom()) {
        return QAbstractItemView::BelowItem; // 鼠标在节点下方
    }
    else {
        return QAbstractItemView::OnItem; // 鼠标在节点内部
    }
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

void NXNavigationView::mousePressEvent(QMouseEvent* event)
{
    QTreeView::mousePressEvent(event);
    if (event->button() == Qt::LeftButton && _pIsLeftButtonPressedToggleNavigation)
    {
        QModelIndex index = indexAt(event->pos());
        if (index.isValid())
        {
            Q_EMIT navigationClicked(index);
        }
        _navigationStyle->setPressIndex(QModelIndex());
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
    if(event->button() == Qt::LeftButton && !_pIsLeftButtonPressedToggleNavigation)
    {
        QModelIndex index = indexAt(event->pos());
        if (index.isValid())
        {
            Q_EMIT navigationClicked(index);
        }
        _navigationStyle->setPressIndex(QModelIndex());
    }
}
// dragEnterEvent 触发 canDropMimeData 函数，如果返回 true 则可以接收 dropMimeData，否则不接收。
void NXNavigationView::dragEnterEvent(QDragEnterEvent* event)
{
    event->setAccepted(event->mimeData()->hasFormat("application/x-nxnavigation-node"));
}

void NXNavigationView::dropEvent(QDropEvent* event)
{
    DropIndicatorPosition dropindicationPos = dropIndicatorPositionOverride();
    _hoveredIndex = QModelIndex();
    NXNavigationModel* model = qobject_cast<NXNavigationModel*>(this->model());
    NXNavigationNode* draggedNode = model->getSelectedNode();
    const QModelIndex draggedIndex = draggedNode->getModelIndex();
    const QModelIndex targetIndex = indexAt(event->position().toPoint());
    const QModelIndex draggedParentIndex = draggedIndex.parent();
    const QModelIndex targetParentIndex = targetIndex.parent();
    bool draggedParentIsRootNode = draggedNode->getParentNode()->getIsRootNode();
    // 禁止与自己的父节点交换
    if (targetIndex == draggedParentIndex && !draggedParentIsRootNode)
    {
        event->ignore();
        return;
    }
    int draggedRow = draggedIndex.row();
    int targetRow = targetIndex.row();
    int draggedParentRow = draggedParentIndex.row();
    int targetParentRow = targetParentIndex.row();

    // 检查目标是否为自身
    if (draggedIndex == targetIndex) {
        if (dropindicationPos == QAbstractItemView::AboveItem || dropindicationPos == QAbstractItemView::OnItem) {
            event->ignore();
            return;
        }
        else if (dropindicationPos == QAbstractItemView::BelowItem) {
            if ((targetRow += 1) == model->rowCount(targetParentIndex))// 移动到下一个位置，最后一个节点不处理
            {
                event->ignore();
                return;
            }
        }
    }

    QModelIndex previousIndex = model->index(draggedRow - 1, 0, draggedParentIndex);
    if (previousIndex.isValid() && previousIndex == targetIndex && dropindicationPos == QAbstractItemView::BelowItem) {
        event->ignore();
        return; 
    }
    //QModelIndex nextIndex = model->index(draggedRow + 1, 0, draggedParentIndex);
    //if (nextIndex.isValid() && nextIndex == targetIndex && dropindicationPos == QAbstractItemView::AboveItem) {
    //    event->ignore();
    //    return;
    //}
    if (draggedRow == 0 && dropindicationPos == QAbstractItemView::BelowItem) {
        // 首节点只能与下方节点交换
        event->ignore();
        return;
    }
    if ((draggedRow == model->rowCount(targetParentIndex) - 1) && !targetIndex.isValid()) {
        // 尾节点只能与上方节点交换
        event->ignore();
        return;
    }
    if (model->canDropMimeData(event->mimeData(), Qt::MoveAction, targetRow, 0, targetParentIndex)) {
        model->dropMimeData(event->mimeData(), Qt::MoveAction, targetRow, 0, targetParentIndex);
        event->acceptProposedAction();
    }
    else {
        event->ignore();
    }
    viewport()->update();
}

void NXNavigationView::dragMoveEvent(QDragMoveEvent* event)
{
    //QTreeView::dragMoveEvent(event);
    // 触发PE_IndicatorItemViewItemDrop，dropIndicatorPosition()
    // 但是对于PageNode来说，不会触发QStyle::PE_IndicatorItemViewItemDrop，
    // 且当前点击拖拽的PageNode，触发dropIndicatorPosition()Above和Below时效果不好
    // 所以这里需要重写dragMoveEvent，自定义dropIndicatorPosition方法和绘制指示器
    if (!event->mimeData()->hasFormat("application/x-nxnavigation-node")) {
        event->ignore();
        return;
    }

    _hoveredIndex = indexAt(event->position().toPoint());
    viewport()->update();
    event->accept();
}

void NXNavigationView::paintEvent(QPaintEvent* event)
{
    QTreeView::paintEvent(event);
    if (!_hoveredIndex.isValid()) return;
    QStyleOptionViewItem option;
    option.initFrom(this);
    option.rect = visualRect(_hoveredIndex);

    QPainter painter(viewport());
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(NXThemeColor(nxTheme->getThemeMode(), PrimaryNormal), 2));

    switch (dropIndicatorPositionOverride()) {
    case QAbstractItemView::AboveItem: {
        int startY = option.rect.top() + 1;
        int startX = option.rect.left() + 6;
        painter.drawEllipse(QPoint(startX, startY), 4, 4);
        painter.drawLine(QPoint(startX, startY), QPoint(option.rect.width() + 6, startY));
        break;
    }
    case QAbstractItemView::BelowItem: {
        int startY = option.rect.bottom();
        int startX = option.rect.left() + 6;
        painter.drawEllipse(QPoint(startX, startY), 4, 4);
        painter.drawLine(QPoint(startX, startY), QPoint(option.rect.width() + 6, startY));
        break;
    }
    case QAbstractItemView::OnItem: {
        painter.drawRect(option.rect.adjusted(1, 1, -1, -1));
        break;
    }
    default: break;
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
