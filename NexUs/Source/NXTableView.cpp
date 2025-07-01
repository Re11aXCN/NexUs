#include "NXTableView.h"

#include <QHeaderView>
#include <QMouseEvent>

#include "DeveloperComponents/NXTableViewStyle.h"
#include "NXScrollBar.h"
#include "private/NXTableViewPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(NXTableView, bool, DrawSelectionBackground)
NXTableView::NXTableView(QWidget* parent)
    : QTableView(parent), d_ptr(new NXTableViewPrivate())
{
    Q_D(NXTableView);
    d->q_ptr = this;
    d->_pDrawSelectionBackground = true;

    setMouseTracking(true);
    setObjectName("NXTableView");
    setStyleSheet(
        "QTableView{background-color:transparent;}"
        "QHeaderView{background-color:transparent;border:0px;}");
    setShowGrid(false);
    setVerticalScrollBar(new NXScrollBar(this));
    setHorizontalScrollBar(new NXScrollBar(this));
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    d->_pTableViewStyle = new NXTableViewStyle(style());
    setStyle(d->_pTableViewStyle);
    this->installEventFilter(this);
}

NXTableView::~NXTableView()
{
}

void NXTableView::setBorderRadius(int radius) {
	Q_D(NXTableView);
    d->_pTableViewStyle->setBorderRadius(radius);
    update();
}

int NXTableView::getBorderRadius() const {
	Q_D(const NXTableView);
    return d->_pTableViewStyle->getBorderRadius();
}

void NXTableView::setIsDrawAlternateRowsEnabled(bool enabled) {
    Q_D(NXTableView);
    d->_pTableViewStyle->setIsDrawAlternateRowsEnabled(enabled);
    update();
}

bool NXTableView::getIsDrawAlternateRowsEnabled() const {
    Q_D(const NXTableView);
    return d->_pTableViewStyle->getIsDrawAlternateRowsEnabled();
}

void NXTableView::setIsSelectionEffectsEnabled(bool enabled) {
    Q_D(NXTableView);
    d->_pTableViewStyle->setIsSelectionEffectsEnabled(enabled);
    update();
}

bool NXTableView::getIsSelectionEffectsEnabled() const {
    Q_D(const NXTableView);
    return d->_pTableViewStyle->getIsSelectionEffectsEnabled();
}

void NXTableView::setIndexWidget(const QModelIndex& index, QWidget* widget) {
    QTableView::setIndexWidget(index, widget);
}

QWidget* NXTableView::indexWidget(const QModelIndex& index) const
{
    return QTableView::indexWidget(index);
}

QRect NXTableView::getAlignLeft(const QRect& cellRect, const QSize& iconSize) const
{
    return QRect(cellRect.x(),
        cellRect.y() + (cellRect.height() - iconSize.height()) / 2,
        iconSize.width(),
        iconSize.height());
}

QRect NXTableView::getAlignCenter(const QRect& cellRect, const QSize& iconSize) const
{
    return QRect(cellRect.x() + (cellRect.width() - iconSize.width()) / 2,
        cellRect.y() + (cellRect.height() - iconSize.height()) / 2,
        iconSize.width(),
        iconSize.height());
}

QRect NXTableView::getAlignRight(const QRect& cellRect, const QSize& iconSize) const
{
    return QRect(cellRect.x() + (cellRect.width() - iconSize.width()),
        cellRect.y() + (cellRect.height() - iconSize.height()) / 2,
        iconSize.width(),
        iconSize.height());
}

void NXTableView::setHeaderFontSize(int size)
{
    QFont tableHeaderFont = horizontalHeader()->font();
    tableHeaderFont.setPixelSize(size);
    horizontalHeader()->setFont(tableHeaderFont);
}

void NXTableView::setModelFontSize(int size)
{
    QFont tableModelFont = this->font();
    tableModelFont.setPixelSize(size);
    this->setFont(tableModelFont);
}

void NXTableView::setTableFontSize(int size)
{
    setHeaderFontSize(size);
    setModelFontSize(size);
}

void NXTableView::setHeaderAdjustParam(const QMap<int, NXAdjustParam>& adjustParamMap)
{
    Q_D(NXTableView);
    d->_pTableViewStyle->setHeaderAdjustParam(adjustParamMap);
}

void NXTableView::setAdjustTextRect(const QMap<int, NXAdjustParam>& adjustParamsMap)
{
    Q_D(NXTableView);
    d->_pTableViewStyle->setAdjustParams(adjustParamsMap);
}

void NXTableView::setCurrentHoverRow(int row)
{
    Q_D(NXTableView);
    d->_pTableViewStyle->setCurrentHoverRow(row);
    update();
}

void NXTableView::setHeaderMargin(int headerMargin)
{
    Q_D(NXTableView);
    if (headerMargin >= 0)
    {
        d->_pTableViewStyle->setHeaderMargin(headerMargin);
        doItemsLayout();
    }
}

int NXTableView::getHeaderMargin() const
{
    Q_D(const NXTableView);
    return d->_pTableViewStyle->getHeaderMargin();
}

void NXTableView::enterIndexWidgetUpdateHoverIndex(const QModelIndex& index)
{
    Q_D(NXTableView);
    if (selectionBehavior() == QAbstractItemView::SelectRows)
    {
        d->_pTableViewStyle->setCurrentHoverRow(index.row());
        update();
    }
}
void NXTableView::showEvent(QShowEvent* event)
{
    Q_EMIT tableViewShow();
    QTableView::showEvent(event);
}

void NXTableView::hideEvent(QHideEvent* event)
{
    Q_EMIT tableViewHide();
    QTableView::hideEvent(event);
}

void NXTableView::mouseMoveEvent(QMouseEvent* event)
{
    Q_D(NXTableView);
    if (selectionBehavior() == QAbstractItemView::SelectRows)
    {
        const QModelIndex& currentIndex = indexAt(event->pos());
        int currentHoverdRow = currentIndex.row();
        int currentHoverdColumn = currentIndex.column();
        d->_pTableViewStyle->setCurrentHoverRow(currentHoverdRow);
        Q_EMIT currentHoverRowChanged(currentHoverdRow);
        Q_EMIT currentHoverColumnChanged(currentHoverdColumn);
        update();
    }
    QTableView::mouseMoveEvent(event);
}

void NXTableView::mousePressEvent(QMouseEvent* event)
{
    Q_D(NXTableView);
    QTableView::mousePressEvent(event);
}

void NXTableView::mouseReleaseEvent(QMouseEvent* event)
{
    QTableView::mouseReleaseEvent(event);
}
void NXTableView::leaveEvent(QEvent* event)
{
    Q_D(NXTableView);
    if (selectionBehavior() == QAbstractItemView::SelectRows)
    {
        d->_pTableViewStyle->setCurrentHoverRow(-1);
        Q_EMIT currentHoverRowChanged(-1);
        update();
    }
    QTableView::leaveEvent(event);
}

NXModelIndexWidget::NXModelIndexWidget(const QModelIndex& index, QWidget* parent)
    : QWidget(parent), d_ptr(new NXModelIndexWidgetPrivate())
{
    Q_D(NXModelIndexWidget);
    d->q_ptr = this;
    d->_index = index;
    QObject::connect(this, &NXModelIndexWidget::updateIndex, this, [d, parent, this](const QModelIndex& newIndex) {
        d->_index = newIndex;
        qobject_cast<NXTableView*>(parent)->enterIndexWidgetUpdateHoverIndex(newIndex);
        });
}

NXModelIndexWidget::~NXModelIndexWidget()
{
}

const QModelIndex& NXModelIndexWidget::index() const
{
    Q_D(const NXModelIndexWidget);
    return d->_index;
}

void NXModelIndexWidget::setIndex(const QModelIndex& index)
{
    Q_D(NXModelIndexWidget);
    d->_index = index;
}

void NXModelIndexWidget::enterEvent(QEnterEvent* event)
{
    Q_D(NXModelIndexWidget);
    Q_EMIT entered();
    QWidget::enterEvent(event);
}

void NXModelIndexWidget::leaveEvent(QEvent* event)
{
    Q_D(NXModelIndexWidget);
    Q_EMIT leaved();
    QWidget::leaveEvent(event);
}

void NXModelIndexWidget::mousePressEvent(QMouseEvent* event)
{
    Q_D(NXModelIndexWidget);
    QWidget::mousePressEvent(event);
}
