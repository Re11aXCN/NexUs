﻿#include "NXNavigationBar.h"

#include "DeveloperComponents/NXBaseListView.h"
#include "DeveloperComponents/NXFooterDelegate.h"
#include "DeveloperComponents/NXFooterModel.h"
#include "NXIconButton.h"
#include "NXInteractiveCard.h"
#include "NXMenu.h"
#include "DeveloperComponents/NXNavigationModel.h"
#include "DeveloperComponents/NXNavigationNode.h"
#include "DeveloperComponents/NXNavigationView.h"
#include "NXSuggestBox.h"
#include "NXTheme.h"
#include "NXToolButton.h"
#include "NXToolTip.h"
#include "private/NXNavigationBarPrivate.h"
#include "private/NXSuggestBoxPrivate.h"
#include <QEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QResizeEvent>
#include <QScroller>
#include <QVBoxLayout>
Q_PROPERTY_CREATE_Q_CPP(NXNavigationBar, bool, IsTransparent)
Q_PROPERTY_CREATE_Q_CPP(NXNavigationBar, bool, IsAllowPageOpenInNewWindow)
NXNavigationBar::NXNavigationBar(QWidget* parent)
    : QWidget{parent}, d_ptr(new NXNavigationBarPrivate())
{
    Q_D(NXNavigationBar);
    d->q_ptr = this;
    d->_pIsAllowPageOpenInNewWindow = true;
    d->_pNavigationBarWidth = 300;
    setFixedWidth(300);
    d->_pIsTransparent = true;

    //用户卡片
    d->_userCard = new NXInteractiveCard(this);
    d->_userCard->setMinimumWidth(0);
    d->_userCard->setCardPixmap(QPixmap(":/Resource/Image/Cirno.jpg"));
    d->_userCard->setTitle("NX Tool");
    d->_userCard->setSubTitle("Liniyous@gmail.com");
    QObject::connect(d->_userCard, &NXInteractiveCard::clicked, this, &NXNavigationBar::userInfoCardClicked);
    d->_userButton = new NXIconButton(QPixmap(":/Resource/Image/Cirno.jpg"), this);
    d->_userButton->setFixedSize(36, 36);
    d->_userButton->setVisible(false);
    d->_userButton->setBorderRadius(8);
    d->_userButtonLayout = new QVBoxLayout();
    d->_userButtonLayout->setAlignment(Qt::AlignLeft);
    d->_userButtonLayout->setContentsMargins(0, 0, 0, 6);
    d->_userButtonLayout->setSpacing(0);
    d->_userButtonLayout->addWidget(d->_userButton);

    QObject::connect(d->_userButton, &NXIconButton::clicked, this, &NXNavigationBar::userInfoCardClicked);
    QHBoxLayout* userCardLayout = new QHBoxLayout();
    userCardLayout->setContentsMargins(0, 0, 0, 0);
    userCardLayout->addSpacing(3);
    userCardLayout->addLayout(d->_userButtonLayout);
    userCardLayout->addWidget(d->_userCard);

    // 搜索栏和按钮组
    d->_navigationButton = new NXToolButton(this);
    d->_navigationButton->setFixedSize(40, 38);
    d->_navigationButton->setNXIcon(NXIconType::Bars);
    d->_navigationButton->setBorderRadius(8);
    QObject::connect(d->_navigationButton, &NXToolButton::clicked, d, &NXNavigationBarPrivate::onNavigationButtonClicked);

    d->_searchButton = new NXToolButton(this);
    d->_searchButton->setFixedSize(40, 38);
    d->_searchButton->setNXIcon(NXIconType::MagnifyingGlass);
    d->_searchButton->setBorderRadius(8);
    QObject::connect(d->_searchButton, &NXToolButton::clicked, d, &NXNavigationBarPrivate::onNavigationButtonClicked);
    d->_searchButton->setVisible(false);

    d->_navigationSuggestBox = new NXSuggestBox(this);
    d->_navigationSuggestBox->setMinimumWidth(0);
    d->_navigationButtonLayout = new QVBoxLayout();
    d->_navigationButtonLayout->setContentsMargins(0, 0, 0, 0);
    d->_navigationButtonLayout->setSpacing(0);
    d->_navigationButtonLayout->addWidget(d->_navigationButton);
    d->_navigationButtonLayout->addWidget(d->_searchButton);

    d->_navigationSuggestLayout = new QHBoxLayout();
    d->_navigationSuggestLayout->setContentsMargins(0, 0, 10, 0);
    d->_navigationSuggestLayout->setSpacing(6);
    d->_navigationSuggestLayout->addLayout(d->_navigationButtonLayout);
    d->_navigationSuggestLayout->addWidget(d->_navigationSuggestBox);

    // 搜索跳转
    QObject::connect(d->_navigationSuggestBox, &NXSuggestBox::suggestionClicked, this, [=](const QString& suggestText, QVariantMap suggestData) {
        navigation(suggestData.value("NXPageKey").toString());
    });

    // 导航模型
    d->_navigationModel = new NXNavigationModel(this);
    d->_navigationView = new NXNavigationView(this);
    d->_navigationView->setNavigationBarPrivate(d);
    d->_navigationView->setModel(d->_navigationModel);
    QObject::connect(d->_navigationView, &NXNavigationView::navigationClicked, this, [=](const QModelIndex& index) {
        d->onTreeViewClicked(index);
    });
    QObject::connect(d->_navigationView, &NXNavigationView::navigationPositionSwapped, this, [=](const QModelIndex& from, const QModelIndex& to) {
        d->onTreeViewClicked(from, false);
        });
    QObject::connect(d->_navigationView, &NXNavigationView::navigationOpenNewWindow, d, &NXNavigationBarPrivate::onNavigationOpenNewWindow);
    QObject::connect(d->_navigationView, &NXNavigationView::navigationCloseCurrentWindow, d, &NXNavigationBarPrivate::onNavigationCloseCurrentWindow);
    QObject::connect(d->_navigationModel, &NXNavigationModel::rowsMoved, this, [=]() {
        d->_initNodeModelIndex(QModelIndex());
        d->_resetNodeSelected();
        });
    // 页脚
    d->_footerView = new NXBaseListView(this);
    d->_footerView->setFixedHeight(0);
    d->_footerModel = new NXFooterModel(this);
    d->_footerView->setModel(d->_footerModel);
    d->_footerDelegate = new NXFooterDelegate(this);
    d->_footerDelegate->setNXListView(d->_footerView);
    d->_footerView->setItemDelegate(d->_footerDelegate);
    QObject::connect(d->_footerView, &NXBaseListView::mousePress, this, [=](const QModelIndex& index) {
        d->_footerDelegate->setPressIndex(index);
        d->_footerView->viewport()->update();
    });
    QObject::connect(d->_footerView, &NXBaseListView::mouseDoubleClick, this, [=](const QModelIndex& index) {
        d->_footerDelegate->setPressIndex(index);
        d->_footerView->viewport()->update();
    });
    QObject::connect(d->_footerView, &NXBaseListView::mouseRelease, this, [=](const QModelIndex& index) {
        d->_footerDelegate->setPressIndex(QModelIndex());
        d->_footerView->viewport()->update();
    });
    QObject::connect(d->_footerView, &NXBaseListView::clicked, this, [=](const QModelIndex& index) {
        d->onFooterViewClicked(index);
    });

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignLeft);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 5, 5);
    mainLayout->addLayout(userCardLayout);
    //mainLayout->addSpacing(4);
    mainLayout->addLayout(d->_navigationSuggestLayout);
    mainLayout->addSpacing(4);
    mainLayout->addWidget(d->_navigationView);
    mainLayout->addWidget(d->_footerView);

    // 主题设置
    d->_themeMode = nxTheme->getThemeMode();
    QObject::connect(nxTheme, &NXTheme::themeModeChanged, this, [=](NXThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
    });
}

NXNavigationBar::~NXNavigationBar()
{
}

void NXNavigationBar::setNavigationBarWidth(int navigationBarWidth)
{
    Q_D(NXNavigationBar);
    if (navigationBarWidth < 180)
    {
        navigationBarWidth = 180;
    }
    if (d->_currentDisplayMode == NXNavigationType::NavigationDisplayMode::Maximal)
    {
        setFixedWidth(navigationBarWidth);
    }
    d->_pNavigationBarWidth = navigationBarWidth;
    Q_EMIT pNavigationBarWidthChanged();
}

int NXNavigationBar::getNavigationBarWidth() const
{
    Q_D(const NXNavigationBar);
    return d->_pNavigationBarWidth;
}

void NXNavigationBar::setUserInfoCardVisible(bool isVisible)
{
    Q_D(NXNavigationBar);
    d->_isShowUserCard = isVisible;
    d->_userCard->setVisible(isVisible);
    if (!isVisible)
    {
        d->_userButton->setVisible(false);
    }
}

void NXNavigationBar::setUserInfoCardPixmap(QPixmap pix)
{
    Q_D(NXNavigationBar);
    d->_userCard->setCardPixmap(pix);
    d->_userButton->setPixmap(pix);
}

void NXNavigationBar::setUserInfoCardTitle(const QString& title)
{
    Q_D(NXNavigationBar);
    d->_userCard->setTitle(title);
}

void NXNavigationBar::setUserInfoCardSubTitle(const QString& subTitle)
{
    Q_D(NXNavigationBar);
    d->_userCard->setSubTitle(subTitle);
}

void NXNavigationBar::setNavigationPageOpenPolicy(std::function<void(const QString&/*nodeKey*/)>&& openNavigationPageFunc)
{
    Q_D(NXNavigationBar);
    d->_openPageFunc = std::move(openNavigationPageFunc);
}

void NXNavigationBar::setIsLeftButtonPressedToggleNavigation(bool isPressed)
{
    Q_D(NXNavigationBar);
    d->_navigationView->setIsLeftButtonPressedToggleNavigation(isPressed);
}

void NXNavigationBar::setNavigationNodeDragAndDropEnable(bool isEnable)
{
    Q_D(NXNavigationBar);
    d->_navigationView->setNavigationNodeDragAndDropEnable(isEnable);
}

void NXNavigationBar::setToolTipOffset(int offsetX, int offsetY)
{
    Q_D(NXNavigationBar);
    d->_navigationView->getCompactToolTip()->setOffSetX(offsetX);
    d->_navigationView->getCompactToolTip()->setOffSetY(offsetY);
}

NodeOperateReturnTypeWithKey NXNavigationBar::addExpanderNode(const QString& expanderTitle, NXIconType::IconName awesome)
{
    Q_D(NXNavigationBar);
    NodeOperateReturnTypeWithKey returnType = d_ptr->_navigationModel->addExpanderNode(expanderTitle, awesome);
    if (returnType.nodeOperateReturnType == NXNavigationType::Success)
    {
        d->_initNodeModelIndex(QModelIndex());
        d->_resetNodeSelected();
    }
    return returnType;
}

NodeOperateReturnTypeWithKey NXNavigationBar::addExpanderNode(const QString& expanderTitle, const QString& targetExpanderKey, NXIconType::IconName awesome)
{
    Q_D(NXNavigationBar);
    NodeOperateReturnTypeWithKey returnType = d->_navigationModel->addExpanderNode(expanderTitle, targetExpanderKey, awesome);
    if (returnType.nodeOperateReturnType == NXNavigationType::Success)
    {
        d->_initNodeModelIndex(QModelIndex());
        d->_resetNodeSelected();
    }
    return returnType;
}

NodeOperateReturnTypeWithKey NXNavigationBar::addPageNode(const QString& pageTitle, QWidget* page, NXIconType::IconName awesome)
{
    Q_D(NXNavigationBar);
    if (!page)
    {
        return { NXNavigationType::PageInvalid, QString{} };
    }
    NodeOperateReturnTypeWithKey returnType = d_ptr->_navigationModel->addPageNode(pageTitle, awesome);
    if (returnType.nodeOperateReturnType == NXNavigationType::Success)
    {
        d->_pageMetaMap.insert(returnType.nodeKey, page->metaObject());
        d->_pageNewWindowCountMap.insert(returnType.nodeKey, 0);
        d->_addStackedPage(page, returnType.nodeKey);
        d->_initNodeModelIndex(QModelIndex());
        d->_resetNodeSelected();
    }
    return returnType;
}

NodeOperateReturnTypeWithKey NXNavigationBar::addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, NXIconType::IconName awesome)
{
    Q_D(NXNavigationBar);
    if (!page)
    {
		return { NXNavigationType::PageInvalid, QString{} };
    }
    if (targetExpanderKey.isEmpty())
    {
        return { NXNavigationType::TargetNodeInvalid, QString{} };
    }
    NodeOperateReturnTypeWithKey returnType = d->_navigationModel->addPageNode(pageTitle, targetExpanderKey, awesome);
    if (returnType.nodeOperateReturnType == NXNavigationType::NodeOperateReturnType::Success)
    {
        d->_pageMetaMap.insert(returnType.nodeKey, page->metaObject());
        d->_pageNewWindowCountMap.insert(returnType.nodeKey, 0);
        NXNavigationNode* node = d->_navigationModel->getNavigationNode(returnType.nodeKey);
        NXNavigationNode* originalNode = node->getOriginalNode();
        if (d->_compactMenuMap.contains(originalNode))
        {
            NXMenu* menu = d->_compactMenuMap.value(originalNode);
            QAction* action = menu->addNXIconAction(node->getAwesome(), node->getNodeTitle());
            QObject::connect(action, &QAction::triggered, this, [=]() {
                d->onTreeViewClicked(node->getModelIndex());
            });
        }
        else
        {
            NXMenu* menu = new NXMenu(const_cast<NXNavigationBar*>(this));
            QAction* action = menu->addNXIconAction(node->getAwesome(), node->getNodeTitle());
            QObject::connect(action, &QAction::triggered, this, [=]() {
                d_ptr->onTreeViewClicked(node->getModelIndex());
            });
            d_ptr->_compactMenuMap.insert(originalNode, menu);
        }
        d_ptr->_addStackedPage(page, returnType.nodeKey);
        d->_initNodeModelIndex(QModelIndex());
        d->_resetNodeSelected();
    }
    return returnType;
}

NodeOperateReturnTypeWithKey NXNavigationBar::addPageNode(const QString& pageTitle, QWidget* page, int keyPoints, NXIconType::IconName awesome)
{
    Q_D(NXNavigationBar);
    if (!page)
    {
        return { NXNavigationType::PageInvalid, QString{} };
    }
    NodeOperateReturnTypeWithKey returnType = d_ptr->_navigationModel->addPageNode(pageTitle, keyPoints, awesome);
    if (returnType.nodeOperateReturnType == NXNavigationType::Success)
    {
        d->_pageMetaMap.insert(returnType.nodeKey, page->metaObject());
        d->_pageNewWindowCountMap.insert(returnType.nodeKey, 0);
        d->_addStackedPage(page, returnType.nodeKey);
        d->_initNodeModelIndex(QModelIndex());
        d->_resetNodeSelected();
    }
    return returnType;
}

NodeOperateReturnTypeWithKey NXNavigationBar::addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, int keyPoints, NXIconType::IconName awesome)
{
    Q_D(NXNavigationBar);
    if (!page)
    {
        return { NXNavigationType::PageInvalid, QString{} };
    }
    if (targetExpanderKey.isEmpty())
    {
        return { NXNavigationType::TargetNodeInvalid, QString{} };
    }
    NodeOperateReturnTypeWithKey returnType = d_ptr->_navigationModel->addPageNode(pageTitle, targetExpanderKey, keyPoints, awesome);
    if (returnType.nodeOperateReturnType == NXNavigationType::Success)
    {
        d->_pageMetaMap.insert(returnType.nodeKey, page->metaObject());
        d->_pageNewWindowCountMap.insert(returnType.nodeKey, 0);
        NXNavigationNode* node = d_ptr->_navigationModel->getNavigationNode(returnType.nodeKey);
        NXNavigationNode* originalNode = node->getOriginalNode();
        if (d_ptr->_compactMenuMap.contains(originalNode))
        {
            NXMenu* menu = d_ptr->_compactMenuMap.value(originalNode);
            QAction* action = menu->addNXIconAction(node->getAwesome(), node->getNodeTitle());
            QObject::connect(action, &QAction::triggered, this, [=]() {
                d_ptr->onTreeViewClicked(node->getModelIndex());
            });
        }
        else
        {
            NXMenu* menu = new NXMenu(const_cast<NXNavigationBar*>(this));
            QAction* action = menu->addNXIconAction(node->getAwesome(), node->getNodeTitle());
            QObject::connect(action, &QAction::triggered, this, [=]() {
                d_ptr->onTreeViewClicked(node->getModelIndex());
            });
            d_ptr->_compactMenuMap.insert(originalNode, menu);
        }
        d_ptr->_addStackedPage(page, returnType.nodeKey);
        d->_initNodeModelIndex(QModelIndex());
        d->_resetNodeSelected();
    }
    return returnType;
}

NodeOperateReturnTypeWithKey NXNavigationBar::addFooterNode(const QString& footerTitle, int keyPoints, NXIconType::IconName awesome)
{
    return addFooterNode(footerTitle, nullptr, keyPoints, awesome);
}

NodeOperateReturnTypeWithKey NXNavigationBar::addFooterNode(const QString& footerTitle, QWidget* page, int keyPoints, NXIconType::IconName awesome)
{
    NodeOperateReturnTypeWithKey returnType = d_ptr->_footerModel->addFooterNode(footerTitle, page != nullptr, keyPoints, awesome);
    if (returnType.nodeOperateReturnType == NXNavigationType::Success)
    {
        d_ptr->_addFooterPage(page, returnType.nodeKey);
    }
    return returnType;
}

QString NXNavigationBar::getNavigationRootKey() const
{
	Q_D(const NXNavigationBar);
    return d->_navigationModel->getRootNode()->getNodeKey();
}

bool NXNavigationBar::getNavigationNodeIsExpanded(const QString& expanderKey) const
{
    Q_D(const NXNavigationBar);
    NXNavigationNode* node = d->_navigationModel->getNavigationNode(expanderKey);
    if (!node || !node->getIsExpanderNode())
    {
        return false;
    }
    return d->_navigationView->isExpanded(node->getModelIndex());
}

void NXNavigationBar::expandNavigationNode(const QString& expanderKey)
{
    Q_D(NXNavigationBar);
    NXNavigationNode* node = d->_navigationModel->getNavigationNode(expanderKey);
    if (!node || !node->getIsExpanderNode())
    {
        return;
    }
    d->_expandOrCollapseExpanderNode(node, true);
    d->_resetNodeSelected();
}

void NXNavigationBar::collapseNavigationNode(const QString& expanderKey)
{
    Q_D(NXNavigationBar);
    NXNavigationNode* node = d->_navigationModel->getNavigationNode(expanderKey);
    if (!node || !node->getIsExpanderNode())
    {
        return;
    }
    d->_expandOrCollapseExpanderNode(node, false);
    d->_resetNodeSelected();
}

void NXNavigationBar::removeNavigationNode(const QString& nodeKey)
{
    Q_D(NXNavigationBar);
    NXNavigationNode* node = d->_navigationModel->getNavigationNode(nodeKey);
    if (!node)
    {
        node = d->_footerModel->getNavigationNode(nodeKey);
    }
    if (!node)
    {
        return;
    }
    if (node->getIsFooterNode())
    {
        Q_EMIT navigationNodeRemoved(NXNavigationType::FooterNode, nodeKey);
        d->_footerModel->removeNavigationNode(nodeKey);
        d->_footerView->setFixedHeight(40 * d->_footerModel->getFooterNodeCount());
    }
    else
    {
        QStringList removeKeyList = d->_navigationModel->removeNavigationNode(nodeKey);
        d->_initNodeModelIndex(QModelIndex());
        for (const auto& removeKey : removeKeyList)
        {
            d->_pageMetaMap.remove(removeKey);
            d->_pageNewWindowCountMap.remove(removeKey);
            Q_EMIT navigationNodeRemoved(NXNavigationType::PageNode, removeKey);
        }
    }
    d->_navigationSuggestBox->removeSuggestion(d->_suggestKeyMap.value(nodeKey));
}

void NXNavigationBar::setNodeKeyPoints(const QString& nodeKey, int keyPoints)
{
    Q_D(NXNavigationBar);
    NXNavigationNode* node = d->_navigationModel->getNavigationNode(nodeKey);
    if (!node)
    {
        node = d->_footerModel->getNavigationNode(nodeKey);
    }
    else if (node->getIsExpanderNode() || keyPoints < 0)
    {
        return;
    }
    if (!node)
    {
        return;
    }
    node->setKeyPoints(keyPoints);
    if (node->getIsFooterNode())
    {
        d->_footerView->viewport()->update();
    }
    else
    {
        d->_navigationView->viewport()->update();
    }
}

int NXNavigationBar::getNodeKeyPoints(const QString& nodeKey) const
{
    Q_D(const NXNavigationBar);
    NXNavigationNode* node = d->_navigationModel->getNavigationNode(nodeKey);
    if (!node)
    {
        node = d->_footerModel->getNavigationNode(nodeKey);
    }
    else if (node->getIsExpanderNode())
    {
        return -1;
    }
    if (!node)
    {
        return -1;
    }
    return node->getKeyPoints();
}

NXNavigationType::NodeOperateReturnType NXNavigationBar::setNavigationNodeTitle(const QString& nodeTitle, const QString& nodeKey)
{
    Q_D(NXNavigationBar);
    NXNavigationNode* node = d->_navigationModel->getNavigationNode(nodeKey);
    if (!node)
    {
        node = d->_footerModel->getNavigationNode(nodeKey);
    }
    if (node)
    {
        node->setNodeTitle(nodeTitle);
        update();
        return NXNavigationType::Success;
    }
    else
    {
        return NXNavigationType::TargetNodeInvalid;
    }
}

QString NXNavigationBar::getNavigationNodeTitle(const QString& nodeKey) const
{
    Q_D(const NXNavigationBar);
    NXNavigationNode* node = d->_navigationModel->getNavigationNode(nodeKey);
    if (!node)
    {
        node = d->_footerModel->getNavigationNode(nodeKey);
    }
    return node == nullptr ? QString{} : node->getNodeTitle();
}

void NXNavigationBar::navigation(const QString& pageKey, bool isLogClicked, bool isRouteBack)
{
    Q_D(NXNavigationBar);
    NXNavigationNode* node = d->_navigationModel->getNavigationNode(pageKey);
    if (!node)
    {
        node = d->_footerModel->getNavigationNode(pageKey);
    }
    if (node)
    {
        if (node->getIsFooterNode())
        {
            d->onFooterViewClicked(node->getModelIndex(), isLogClicked, isRouteBack);
        }
        else
        {
            if (!node->getIsExpanderNode())
            {
                d->onTreeViewClicked(node->getModelIndex(), isLogClicked, isRouteBack);
            }
        }
    }
}

void NXNavigationBar::setDisplayMode(NXNavigationType::NavigationDisplayMode displayMode, bool isAnimation)
{
    Q_D(NXNavigationBar);
    if (d->_currentDisplayMode == displayMode || displayMode == NXNavigationType::Auto)
    {
        return;
    }
    d->_doComponentAnimation(displayMode, isAnimation);
    d->_raiseNavigationBar();
    Q_EMIT displayModeChanged(displayMode);
}

NXNavigationType::NodeOperateReturnType NXNavigationBar::navigationPageNodeSwitch(const QString& targetPageNodeKey)
{
    Q_D(NXNavigationBar);
    if (NXNavigationNode* node = d->_navigationModel->getNavigationNode(targetPageNodeKey))
    {
        bool isNotOnlyPageNode = node->getIsExpanderNode() || node->getIsChildNode(node) || node->getIsFooterNode();
        if (isNotOnlyPageNode) return NXNavigationType::TargetNodeTypeError;
        NXNavigationNode* rootNode = node->getParentNode();
        QList<NXNavigationNode*> childNodes = rootNode->getChildrenNodes();
        int index = childNodes.indexOf(node);
        QModelIndex newIndex = d->_navigationModel->index(index, 0, rootNode->getModelIndex());
        d->onTreeViewClicked(newIndex);
        return NXNavigationType::Success;
    }
    return NXNavigationType::TargetNodeInvalid;
}

int NXNavigationBar::getPageOpenInNewWindowCount(const QString& nodeKey) const
{
    Q_D(const NXNavigationBar);
    if (!d->_pageNewWindowCountMap.contains(nodeKey))
    {
        return 0;
    }
    return d->_pageNewWindowCountMap[nodeKey];
}

void NXNavigationBar::paintEvent(QPaintEvent* event)
{
    Q_D(NXNavigationBar);
    if (!d->_pIsTransparent)
    {
        QPainter painter(this);
        painter.save();
        painter.setPen(NXThemeColor(d->_themeMode, PopupBorder));
        painter.setBrush(NXThemeColor(d->_themeMode, PopupBase));
        QRect baseRect = this->rect();
        baseRect.adjust(-1, 0, -1, 0);
        QPainterPath path;
        path.moveTo(baseRect.topLeft());
        path.lineTo(QPoint(baseRect.right() - 8, baseRect.y()));
        path.arcTo(QRectF(baseRect.right() - 16, baseRect.y(), 16, 16), 90, -90);
        path.lineTo(QPoint(baseRect.right(), baseRect.bottom() - 8));
        path.arcTo(QRectF(baseRect.right() - 16, baseRect.bottom() - 16, 16, 16), 0, -90);
        path.lineTo(baseRect.bottomLeft());
        path.closeSubpath();
        painter.drawPath(path);
        painter.restore();
    }
    QWidget::paintEvent(event);
}
