#include "NXNavigationStyle.h"

#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QStyleOption>

#include "DeveloperComponents/NXNavigationModel.h"
#include "DeveloperComponents/NXNavigationNode.h"
#include "DeveloperComponents/NXNavigationView.h"
#include "NXTheme.h"
NXNavigationStyle::NXNavigationStyle(QStyle* style)
{
    _pOpacity = 1;
    _pItemHeight = 40;
    _pLastSelectMarkTop = 10.0;
    _pLastSelectMarkBottom = 10.0;
    _pSelectMarkTop = 10.0;
    _pSelectMarkBottom = 10.0;

    _lastSelectMarkTopAnimation = new QPropertyAnimation(this, "pLastSelectMarkTop");
    QObject::connect(_lastSelectMarkTopAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) { _pNavigationView->viewport()->update(); });
    _lastSelectMarkTopAnimation->setDuration(300);
    _lastSelectMarkTopAnimation->setEasingCurve(QEasingCurve::InOutSine);

    _selectMarkBottomAnimation = new QPropertyAnimation(this, "pSelectMarkBottom");
    QObject::connect(_selectMarkBottomAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) { _pNavigationView->viewport()->update(); });
    _selectMarkBottomAnimation->setDuration(300);
    _selectMarkBottomAnimation->setEasingCurve(QEasingCurve::InOutSine);
    QObject::connect(_lastSelectMarkTopAnimation, &QPropertyAnimation::finished, this, [=]() {
        _isSelectMarkDisplay = true;
        _lastSelectedNode = nullptr;
        _selectMarkBottomAnimation->setStartValue(0);
        _selectMarkBottomAnimation->setEndValue(10);
        _selectMarkBottomAnimation->start(); });

    _lastSelectMarkBottomAnimation = new QPropertyAnimation(this, "pLastSelectMarkBottom");
    QObject::connect(_lastSelectMarkBottomAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) { _pNavigationView->viewport()->update(); });
    _lastSelectMarkBottomAnimation->setDuration(300);
    _lastSelectMarkBottomAnimation->setEasingCurve(QEasingCurve::InOutSine);

    _selectMarkTopAnimation = new QPropertyAnimation(this, "pSelectMarkTop");
    QObject::connect(_selectMarkTopAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) { _pNavigationView->viewport()->update(); });
    _selectMarkTopAnimation->setDuration(300);
    _selectMarkTopAnimation->setEasingCurve(QEasingCurve::InOutSine);
    QObject::connect(_lastSelectMarkBottomAnimation, &QPropertyAnimation::finished, this, [=]() {
        _isSelectMarkDisplay = true;
        _lastSelectedNode = nullptr;
        _selectMarkTopAnimation->setStartValue(0);
        _selectMarkTopAnimation->setEndValue(10);
        _selectMarkTopAnimation->start(); });
    _themeMode = nxTheme->getThemeMode();
    QObject::connect(nxTheme, &NXTheme::themeModeChanged, this, [=](NXThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
    });
}

NXNavigationStyle::~NXNavigationStyle()
{
}

void NXNavigationStyle::drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case QStyle::PE_PanelItemViewItem:
    {
        if (const QStyleOptionViewItem* vopt = qstyleoption_cast<const QStyleOptionViewItem*>(option))
        {
            painter->save();
            QModelIndex index = vopt->index;
            NXNavigationNode* node = static_cast<NXNavigationNode*>(index.internalPointer());
            if (this->_opacityAnimationTargetNode && node->getParentNode() == this->_opacityAnimationTargetNode)
            {
                painter->setOpacity(_pOpacity);
            }
            painter->setRenderHint(QPainter::Antialiasing);
            QRect itemRect = vopt->rect;
            itemRect.setTop(itemRect.top() + 2);
            itemRect.setBottom(itemRect.bottom() - 2);
            QPainterPath path;
            path.addRoundedRect(itemRect, 8, 8);
            if (vopt->state & QStyle::State_Selected)
            {
                if (index == _pPressIndex)
                {
                    painter->fillPath(path, NXThemeColor(_themeMode, BasicHoverAlpha));
                }
                else
                {
                    if (vopt->state & QStyle::State_MouseOver)
                    {
                        painter->fillPath(path, NXThemeColor(_themeMode, BasicSelectedHoverAlpha));
                    }
                    else
                    {
                        painter->fillPath(path, NXThemeColor(_themeMode, BasicSelectedAlpha));
                    }
                }
            }
            else
            {
                if (index == _pPressIndex)
                {
                    painter->fillPath(path, NXThemeColor(_themeMode, BasicSelectedHoverAlpha));
                }
                else
                {
                    if (vopt->state & QStyle::State_MouseOver)
                    {
                        painter->fillPath(path, NXThemeColor(_themeMode, BasicHoverAlpha));
                    }
                }
            }
            painter->restore();
        }
        return;
    }
    case QStyle::PE_PanelItemViewRow:
    {
        return;
    }
    case QStyle::PE_IndicatorBranch:
    {
        return;
    }
    default:
    {
        break;
    }
    }
    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void NXNavigationStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    switch (element)
    {
    case QStyle::CE_ShapedFrame:
    {
        return;
    }
    case QStyle::CE_ItemViewItem:
    {
        if (const QStyleOptionViewItem* vopt = qstyleoption_cast<const QStyleOptionViewItem*>(option))
        {
            this->drawPrimitive(QStyle::PE_PanelItemViewItem, option, painter, widget);

            QRect itemRect = option->rect;
            painter->save();
            painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
            NXNavigationNode* node = static_cast<NXNavigationNode*>(vopt->index.internalPointer());
            NXNavigationModel* model = dynamic_cast<NXNavigationModel*>(const_cast<QAbstractItemModel*>(vopt->index.model()));

            if (this->_opacityAnimationTargetNode && node->getParentNode() == this->_opacityAnimationTargetNode)
            {
                painter->setOpacity(_pOpacity);
            }

            if (_isSelectMarkDisplay && (node == model->getSelectedNode() || node == model->getSelectedExpandedNode()))
            {
                painter->setPen(Qt::NoPen);
                painter->setBrush(NXThemeColor(_themeMode, PrimaryNormal));
                painter->drawRoundedRect(QRectF(itemRect.x() + 3, itemRect.y() + _pSelectMarkTop, 3, itemRect.height() - _pSelectMarkTop - _pSelectMarkBottom), 3, 3);
            }
            if (node == _lastSelectedNode)
            {
                painter->setPen(Qt::NoPen);
                painter->setBrush(NXThemeColor(_themeMode, PrimaryNormal));
                painter->drawRoundedRect(QRectF(itemRect.x() + 3, itemRect.y() + _pLastSelectMarkTop, 3, itemRect.height() - _pLastSelectMarkTop - _pLastSelectMarkBottom), 3, 3);
            }

            painter->setPen(vopt->index == _pPressIndex ? NXThemeColor(_themeMode, BasicTextPress) : NXThemeColor(_themeMode, BasicText));
            if (node->getAwesome() != NXIconType::None)
            {
                painter->save();
                QFont iconFont = QFont(QStringLiteral("ElaAwesome"));
                iconFont.setPixelSize(17);
                painter->setFont(iconFont);
                painter->drawText(QRect(itemRect.x(), itemRect.y(), _iconAreaWidth, itemRect.height()), Qt::AlignCenter, QChar((unsigned short)node->getAwesome()));
                painter->restore();
            }

            int viewWidth = widget->width();
            painter->setPen(vopt->index == _pPressIndex ? NXThemeColor(_themeMode, BasicTextPress) : NXThemeColor(_themeMode, BasicText));
            QRect textRect;
            if (node->getAwesome() != NXIconType::None)
            {
                textRect = QRect(itemRect.x() + _iconAreaWidth, itemRect.y(), itemRect.width() - _textRightSpacing - _indicatorIconAreaWidth - _iconAreaWidth, itemRect.height());
            }
            else
            {
                textRect = QRect(itemRect.x() + _leftPadding, itemRect.y(), itemRect.width() - _textRightSpacing - _indicatorIconAreaWidth - _leftPadding, itemRect.height());
            }
            QString text = painter->fontMetrics().elidedText(node->getNodeTitle(), Qt::ElideRight, textRect.width());
            painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, text);

            if (viewWidth > 260)
            {
                if (node->getIsExpanderNode())
                {
                    if (node->getIsHasChild())
                    {
                        QRectF expandIconRect(itemRect.right() - _indicatorIconAreaWidth, itemRect.y(), 17, itemRect.height());

                        painter->save();
                        QFont iconFont = QFont(QStringLiteral("ElaAwesome"));
                        iconFont.setPixelSize(17);
                        painter->setFont(iconFont);
                        painter->translate(expandIconRect.x() + (qreal)expandIconRect.width() / 2, expandIconRect.y() + (qreal)expandIconRect.height() / 2);
                        if (node == _expandAnimationTargetNode)
                        {
                            painter->rotate(_pRotate);
                        }
                        else
                        {
                            if (node->getIsExpanded())
                            {
                                painter->rotate(-180);
                            }
                            else
                            {
                                painter->rotate(0);
                            }
                        }
                        painter->translate(-expandIconRect.x() - (qreal)expandIconRect.width() / 2 + 1, -expandIconRect.y() - (qreal)expandIconRect.height() / 2);
                        painter->drawText(expandIconRect, Qt::AlignVCenter, QChar((unsigned short)NXIconType::AngleDown));
                        painter->restore();
                    }
                    if (node->getIsChildHasKeyPoints())
                    {
                        painter->save();
                        painter->setPen(Qt::NoPen);
                        painter->setBrush(NXThemeColor(_themeMode, PrimaryNormal));
                        painter->drawEllipse(QPoint(itemRect.right() - 17, itemRect.y() + 12), 3, 3);
                        painter->restore();
                    }
                }
                else
                {
                    int keyPoints = node->getKeyPoints();
                    if (keyPoints)
                    {
                        // KeyPoints
                        painter->save();
                        painter->setPen(Qt::NoPen);
                        painter->setBrush(Qt::white);
                        painter->drawEllipse(QPoint(itemRect.right() - 26, itemRect.y() + itemRect.height() / 2), 10, 10);
                        painter->setBrush(NXThemeColor(_themeMode, StatusDanger));
                        painter->drawEllipse(QPoint(itemRect.right() - 26, itemRect.y() + itemRect.height() / 2), 9, 9);
                        painter->setPen(QPen(Qt::white, 2));
                        QFont font = painter->font();
                        font.setBold(true);
                        if (keyPoints > 99)
                        {
                            keyPoints = 99;
                        }
                        if (keyPoints > 9)
                        {
                            font.setPixelSize(11);
                        }
                        else
                        {
                            font.setPixelSize(12);
                        }
                        painter->setFont(font);
                        painter->drawText(keyPoints > 9 ? itemRect.right() - 33 : itemRect.right() - 30, itemRect.y() + itemRect.height() / 2 + 4, QString::number(keyPoints));
                        painter->restore();
                    }
                }
            }
            painter->restore();
        }

        return;
    }
    default:
    {
        break;
    }
    }
    QProxyStyle::drawControl(element, option, painter, widget);
}

QSize NXNavigationStyle::sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const
{
    switch (type)
    {
    case QStyle::CT_ItemViewItem:
    {
        QSize itemSize = QProxyStyle::sizeFromContents(type, option, size, widget);
        itemSize.setHeight(_pItemHeight);
        return itemSize;
    }
    default:
    {
        break;
    }
    }
    return QProxyStyle::sizeFromContents(type, option, size, widget);
}

void NXNavigationStyle::navigationNodeStateChange(QVariantMap data)
{
    if (data.contains("Expand"))
    {
        NXNavigationNode* lastExpandNode = _expandAnimationTargetNode;
        _opacityAnimationTargetNode = data.value("Expand").value<NXNavigationNode*>();
        _expandAnimationTargetNode = _opacityAnimationTargetNode;
        QPropertyAnimation* nodeOpacityAnimation = new QPropertyAnimation(this, "pOpacity");
        QObject::connect(nodeOpacityAnimation, &QPropertyAnimation::finished, this, [=]() { _opacityAnimationTargetNode = nullptr; });
        QObject::connect(nodeOpacityAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            _pNavigationView->viewport()->update();
        });
        nodeOpacityAnimation->setDuration(600);
        nodeOpacityAnimation->setEasingCurve(QEasingCurve::InOutSine);
        nodeOpacityAnimation->setKeyValueAt(0.4, 0);
        nodeOpacityAnimation->setStartValue(0);
        nodeOpacityAnimation->setEndValue(1);
        nodeOpacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);

        QPropertyAnimation* rotateAnimation = new QPropertyAnimation(this, "pRotate");
        QObject::connect(rotateAnimation, &QPropertyAnimation::finished, this, [=]() { _expandAnimationTargetNode = nullptr; });
        QObject::connect(rotateAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            _pNavigationView->viewport()->update();
        });
        rotateAnimation->setDuration(300);
        rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
        if (lastExpandNode == _expandAnimationTargetNode)
        {
            rotateAnimation->setStartValue(_pRotate);
        }
        else
        {
            rotateAnimation->setStartValue(0);
        }
        rotateAnimation->setEndValue(-180);
        rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else if (data.contains("Collapse"))
    {
        NXNavigationNode* lastExpandNode = _expandAnimationTargetNode;
        _opacityAnimationTargetNode = data.value("Collapse").value<NXNavigationNode*>();
        _expandAnimationTargetNode = _opacityAnimationTargetNode;
        _pOpacity = 0;

        QPropertyAnimation* rotateAnimation = new QPropertyAnimation(this, "pRotate");
        QObject::connect(rotateAnimation, &QPropertyAnimation::finished, this, [=]() {
            _pOpacity = 1;
            _expandAnimationTargetNode = nullptr; });
        QObject::connect(rotateAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            _pNavigationView->viewport()->update();
        });
        rotateAnimation->setDuration(300);
        rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
        if (lastExpandNode == _expandAnimationTargetNode)
        {
            rotateAnimation->setStartValue(_pRotate);
        }
        else
        {
            rotateAnimation->setStartValue(-180);
        }
        rotateAnimation->setEndValue(0);
        rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else if (data.contains("SelectMarkChanged"))
    {
        _lastSelectedNode = data.value("LastSelectedNode").value<NXNavigationNode*>();
        NXNavigationNode* selectedNode = data.value("SelectedNode").value<NXNavigationNode*>();
        bool direction = _compareItemY(selectedNode, _lastSelectedNode);
        _pLastSelectMarkTop = 10;
        _pLastSelectMarkBottom = 10;
        _pSelectMarkTop = 10;
        _pSelectMarkBottom = 10;
        if (direction)
        {
            _lastSelectMarkTopAnimation->setStartValue(10);
            _lastSelectMarkTopAnimation->setEndValue(0);
            _lastSelectMarkTopAnimation->start();
            _lastSelectMarkBottomAnimation->stop();
            _selectMarkTopAnimation->stop();
            _isSelectMarkDisplay = false;
        }
        else
        {
            _lastSelectMarkBottomAnimation->setStartValue(10);
            _lastSelectMarkBottomAnimation->setEndValue(0);
            _lastSelectMarkBottomAnimation->start();
            _lastSelectMarkTopAnimation->stop();
            _selectMarkBottomAnimation->stop();
            _isSelectMarkDisplay = false;
        }
    }
}

bool NXNavigationStyle::_compareItemY(NXNavigationNode* node1, NXNavigationNode* node2)
{
    if (!node1)
    {
        return false;
    }
    if (!node2)
    {
        return true;
    }
    if (node1->getParentNode() == node2->getParentNode())
    {
        if (node1->getModelIndex().row() < node2->getModelIndex().row())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        NXNavigationNode* node1OriginalNode = node1->getOriginalNode();
        NXNavigationNode* node2OriginalNode = node2->getOriginalNode();

        if (node1OriginalNode == node2OriginalNode)
        {
            int node1Depth = node1->getDepth();
            int node2Depth = node2->getDepth();
            if (node1Depth == node2Depth)
            {
                NXNavigationNode* node1ParentNode = node1->getParentNode();
                NXNavigationNode* node2ParentNode = node2->getParentNode();
                if (node1ParentNode->getModelIndex().row() < node2ParentNode->getModelIndex().row())
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                if (node1Depth < node2Depth)
                {
                    NXNavigationNode* node2ParentNode = node2->getParentNode();
                    while (node2ParentNode->getDepth() != node1Depth)
                    {
                        node2ParentNode = node2ParentNode->getParentNode();
                    }
                    if (node1 == node2ParentNode)
                    {
                        return true;
                    }
                    if (node1->getModelIndex().row() < node2ParentNode->getModelIndex().row())
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    NXNavigationNode* node1ParentNode = node1->getParentNode();
                    while (node1ParentNode->getDepth() != node2Depth)
                    {
                        node1ParentNode = node1ParentNode->getParentNode();
                    }
                    if (node2 == node1ParentNode)
                    {
                        return false;
                    }
                    if (node1ParentNode->getModelIndex().row() < node2->getModelIndex().row())
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
        }
        else
        {
            if (node1OriginalNode->getModelIndex().row() < node2OriginalNode->getModelIndex().row())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}
