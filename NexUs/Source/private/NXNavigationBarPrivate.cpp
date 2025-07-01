#include "NXNavigationBarPrivate.h"

#include <QLayout>
#include <QPropertyAnimation>

#include "NXApplication.h"
#include "DeveloperComponents/NXBaseListView.h"
#include "DeveloperComponents/NXCustomWidget.h"
#include "DeveloperComponents/NXFooterDelegate.h"
#include "DeveloperComponents/NXFooterModel.h"
#include "NXIconButton.h"
#include "NXInteractiveCard.h"
#include "NXMenu.h"
#include "NXNavigationBar.h"
#include "DeveloperComponents/NXNavigationModel.h"
#include "DeveloperComponents/NXNavigationNode.h"
#include "NXNavigationRouter.h"
#include "DeveloperComponents/NXNavigationView.h"
#include "NXSuggestBox.h"
#include "NXSuggestBoxPrivate.h"
#include "NXToolButton.h"

NXNavigationBarPrivate::NXNavigationBarPrivate(QObject* parent)
    : QObject{ parent }
{
}

NXNavigationBarPrivate::~NXNavigationBarPrivate()
{
}

void NXNavigationBarPrivate::onNavigationButtonClicked()
{
    Q_Q(NXNavigationBar);
    if (_currentDisplayMode == NXNavigationType::Compact)
    {
        q->setDisplayMode(NXNavigationType::Maximal);
    }
    else
    {
        q->setDisplayMode(NXNavigationType::Compact);
    }
}

void NXNavigationBarPrivate::onNavigationOpenNewWindow(const QString& nodeKey)
{
    Q_Q(NXNavigationBar);
    if (_openPageFunc)
    {
        _openPageFunc(nodeKey);
        return;
    }
    const QMetaObject* meta = _pageMetaMap.value(nodeKey);
    if (!meta)
    {
        return;
    }
    QWidget* widget = static_cast<QWidget*>(meta->newInstance());
    if (widget)
    {
        NXCustomWidget* floatWidget = new NXCustomWidget(q);
        floatWidget->setWindowIcon(widget->windowIcon());
        floatWidget->setWindowTitle(widget->windowTitle());
        floatWidget->setCentralWidget(widget);
        floatWidget->show();
    }
}

void NXNavigationBarPrivate::onNavigationCloseCurrentWindow(const QString& nodeKey)
{
    Q_Q(NXNavigationBar);
    const QMetaObject* meta = _pageMetaMap.value(nodeKey);
    if (!meta)
    {
        return;
    }
    q->removeNavigationNode(nodeKey);
}

void NXNavigationBarPrivate::onNavigationRouteBack(QVariantMap routeData)
{
    Q_Q(NXNavigationBar);
    QString pageKey = routeData.value("NXPageKey").toString();
    q->navigation(pageKey, false);
}

void NXNavigationBarPrivate::onTreeViewClicked(const QModelIndex& index, bool isLogRoute)
{
    Q_Q(NXNavigationBar);
    if (index.isValid())
    {
        NXNavigationNode* node = static_cast<NXNavigationNode*>(index.internalPointer());
        if (!node)
        {
            return;
        }
        if (node->getIsExpanderNode())
        {
            _expandOrCollpaseExpanderNode(node, !_navigationView->isExpanded(index));
        }
        else
        {
            if (node->getKeyPoints())
            {
                node->setKeyPoints(0);
                _navigationView->update();
            }
            NXNavigationNode* selectedNode = _navigationModel->getSelectedNode();
            if (selectedNode != node)
            {
                if (isLogRoute)
                {
                    QVariantMap routeData = QVariantMap();
                    QString pageKey;
                    if (selectedNode)
                    {
                        pageKey.append(selectedNode->getNodeKey());
                    }
                    else
                    {
                        if (_footerModel->getSelectedNode())
                        {
                            pageKey.append(_footerModel->getSelectedNode()->getNodeKey());
                        }
                    }
                    routeData.insert("NXPageKey", pageKey);
                    NXNavigationRouter::getInstance()->navigationRoute(this, "onNavigationRouteBack", routeData);
                }

                if (_footerModel->getSelectedNode())
                {
                    _footerView->clearSelection();
                    QVariantMap footerPostData = QVariantMap();
                    footerPostData.insert("SelectMarkChanged", true);
                    footerPostData.insert("LastSelectedNode", QVariant::fromValue(_footerModel->getSelectedNode()));
                    footerPostData.insert("SelectedNode", QVariant::fromValue(nullptr));
                    _footerModel->setSelectedNode(nullptr);
                    _footerDelegate->navigationNodeStateChange(footerPostData);
                }
                QVariantMap postData = QVariantMap();
                postData.insert("SelectMarkChanged", true);
                if (_navigationModel->getSelectedExpandedNode())
                {
                    postData.insert("LastSelectedNode", QVariant::fromValue(_navigationModel->getSelectedExpandedNode()));
                }
                else
                {
                    postData.insert("LastSelectedNode", QVariant::fromValue(_navigationModel->getSelectedNode()));
                }
                if (_currentDisplayMode == NXNavigationType::Compact)
                {
                    NXNavigationNode* originNode = node->getOriginalNode();
                    if (originNode == node)
                    {
                        postData.insert("SelectedNode", QVariant::fromValue(node));
                    }
                    else
                    {
                        if (originNode == _navigationModel->getSelectedExpandedNode())
                        {
                            _navigationModel->setSelectedNode(node);
                            _resetNodeSelected();
                            return;
                        }
                        _navigationModel->setSelectedExpandedNode(originNode);
                        postData.insert("SelectedNode", QVariant::fromValue(originNode));
                    }
                }
                else
                {
                    postData.insert("SelectedNode", QVariant::fromValue(node));
                }
                _navigationModel->setSelectedNode(node);
                _navigationView->navigationNodeStateChange(postData);
                if (!node->getIsVisible() && _currentDisplayMode != NXNavigationType::Compact)
                {
                    _expandSelectedNodeParent();
                }
            }
        }
        _resetNodeSelected();
        Q_EMIT q->navigationNodeClicked(NXNavigationType::PageNode, node->getNodeKey());
    }
}

void NXNavigationBarPrivate::onFooterViewClicked(const QModelIndex& index, bool isLogRoute)
{
    Q_Q(NXNavigationBar);
    NXNavigationNode* node = index.data(Qt::UserRole).value<NXNavigationNode*>();
    if (node->getKeyPoints())
    {
        node->setKeyPoints(0);
        _navigationView->update();
    }
    NXNavigationNode* selectedNode = _footerModel->getSelectedNode();

    if (selectedNode != node)
    {
        if (isLogRoute && node->getIsHasFooterPage())
        {
            QVariantMap routeData = QVariantMap();
            QString pageKey;
            if (selectedNode)
            {
                pageKey.append(selectedNode->getNodeKey());
            }
            else
            {
                if (_navigationModel->getSelectedNode())
                {
                    pageKey.append(_navigationModel->getSelectedNode()->getNodeKey());
                }
            }
            routeData.insert("NXPageKey", pageKey);
            NXNavigationRouter::getInstance()->navigationRoute(this, "onNavigationRouteBack", routeData);
        }

        if (node->getIsHasFooterPage())
        {
            if (_navigationModel->getSelectedNode() || _navigationModel->getSelectedExpandedNode())
            {
                QVariantMap mainPostData = QVariantMap();
                mainPostData.insert("SelectMarkChanged", true);
                mainPostData.insert("LastSelectedNode", QVariant::fromValue(_navigationModel->getSelectedExpandedNode() ? _navigationModel->getSelectedExpandedNode() : _navigationModel->getSelectedNode()));
                mainPostData.insert("SelectedNode", QVariant::fromValue(nullptr));
                _navigationView->clearSelection();
                _navigationView->navigationNodeStateChange(mainPostData);
                _navigationModel->setSelectedExpandedNode(nullptr);
                _navigationModel->setSelectedNode(nullptr);
            }
            _footerView->clearSelection();
            _footerView->selectionModel()->select(index, QItemSelectionModel::Select);
            QVariantMap postData = QVariantMap();
            postData.insert("SelectMarkChanged", true);
            postData.insert("LastSelectedNode", QVariant::fromValue(_footerModel->getSelectedNode()));
            postData.insert("SelectedNode", QVariant::fromValue(node));
            _footerDelegate->navigationNodeStateChange(postData);
            _footerModel->setSelectedNode(node);
        }
    }
    Q_EMIT q->navigationNodeClicked(NXNavigationType::FooterNode, node->getNodeKey());
}

void NXNavigationBarPrivate::_initNodeModelIndex(const QModelIndex& parentIndex)
{
    int rowCount = _navigationModel->rowCount(parentIndex);
    for (int row = 0; row < rowCount; ++row)
    {
        QModelIndex index = _navigationModel->index(row, 0, parentIndex);
        NXNavigationNode* childNode = static_cast<NXNavigationNode*>(index.internalPointer());
        childNode->setModelIndex(index);
        if (_navigationModel->hasChildren(index))
        {
            _initNodeModelIndex(index);
        }
    }
}

void NXNavigationBarPrivate::_resetNodeSelected()
{
    _navigationView->clearSelection();
    NXNavigationNode* selectedNode = _navigationModel->getSelectedNode();
    if (!selectedNode || !selectedNode->getModelIndex().isValid())
    {
        return;
    }
    if (selectedNode->getParentNode()->getIsRootNode() || selectedNode->getIsVisible())
    {
        _navigationView->selectionModel()->select(selectedNode->getModelIndex(), QItemSelectionModel::Select);
        if (_navigationModel->getSelectedExpandedNode())
        {
            QVariantMap postData = QVariantMap();
            postData.insert("SelectMarkChanged", true);
            postData.insert("LastSelectedNode", QVariant::fromValue(_navigationModel->getSelectedExpandedNode()));
            postData.insert("SelectedNode", QVariant::fromValue(selectedNode));
            _navigationView->navigationNodeStateChange(postData);
        }
        _navigationModel->setSelectedExpandedNode(nullptr);
    }
    else
    {
        NXNavigationNode* parentNode = selectedNode->getParentNode();
        while (parentNode && !parentNode->getParentNode()->getIsRootNode() && !parentNode->getIsVisible())
        {
            parentNode = parentNode->getParentNode();
        }
        if (!parentNode)
        {
            return;
        }

        if (!_navigationModel->getSelectedExpandedNode())
        {
            QVariantMap postData = QVariantMap();
            postData.insert("SelectMarkChanged", true);
            postData.insert("LastSelectedNode", QVariant::fromValue(_navigationModel->getSelectedNode()));
            postData.insert("SelectedNode", QVariant::fromValue(parentNode));
            _navigationView->navigationNodeStateChange(postData);
        }
        else
        {
            if (_navigationModel->getSelectedExpandedNode() != parentNode)
            {
                if (_navigationModel->getSelectedExpandedNode()->getOriginalNode() == parentNode->getOriginalNode())
                {
                    QVariantMap postData = QVariantMap();
                    postData.insert("SelectMarkChanged", true);
                    postData.insert("LastSelectedNode", QVariant::fromValue(_navigationModel->getSelectedExpandedNode()));
                    postData.insert("SelectedNode", QVariant::fromValue(parentNode));
                    _navigationView->navigationNodeStateChange(postData);
                }
            }
        }
        _navigationModel->setSelectedExpandedNode(parentNode);
        _navigationView->selectionModel()->select(parentNode->getModelIndex(), QItemSelectionModel::Select);
    }
}

void NXNavigationBarPrivate::_expandSelectedNodeParent()
{
    NXNavigationNode* parentNode = _navigationModel->getSelectedNode()->getParentNode();
    while (parentNode && !parentNode->getIsRootNode())
    {
        QVariantMap data;
        data.insert("Expand", QVariant::fromValue(parentNode));
        _navigationView->navigationNodeStateChange(data);
        parentNode->setIsExpanded(true);
        _navigationView->expand(parentNode->getModelIndex());
        parentNode = parentNode->getParentNode();
    }
}

void NXNavigationBarPrivate::_expandOrCollpaseExpanderNode(NXNavigationNode* node, bool isExpand)
{
    if (_currentDisplayMode == NXNavigationType::Compact)
    {
        if (node->getIsHasPageChild())
        {
            NXMenu* menu = _compactMenuMap.value(node);
            if (menu)
            {
                QPoint nodeTopRight = _navigationView->mapToGlobal(_navigationView->visualRect(node->getModelIndex()).topRight());
                menu->popup(QPoint(nodeTopRight.x() + 10, nodeTopRight.y()));
            }
        }
    }
    else
    {
        QModelIndex index = node->getModelIndex();
        bool isExpanded = _navigationView->isExpanded(index);
        if (node->getIsHasChild() && isExpand != isExpanded)
        {
            QVariantMap data;
            if (isExpanded)
            {
                data.insert("Collapse", QVariant::fromValue(node));
                node->setIsExpanded(isExpand);
                _navigationView->navigationNodeStateChange(data);
                _navigationView->collapse(index);
            }
            else
            {
                data.insert("Expand", QVariant::fromValue(node));
                node->setIsExpanded(true);
                _navigationView->navigationNodeStateChange(data);
                _navigationView->expand(index);
            }
        }
    }
}

void NXNavigationBarPrivate::_addStackedPage(QWidget* page, QString pageKey)
{
    Q_Q(NXNavigationBar);
    page->setProperty("NXPageKey", pageKey);
    Q_EMIT q->navigationNodeAdded(NXNavigationType::PageNode, pageKey, page);
    NXNavigationNode* node = _navigationModel->getNavigationNode(pageKey);
    QVariantMap suggestData;
    suggestData.insert("NXNodeType", "Stacked");
    suggestData.insert("NXPageKey", pageKey);
    QString suggestKey = _navigationSuggestBox->addSuggestion(node->getAwesome(), node->getNodeTitle(), suggestData);
    _suggestKeyMap.insert(pageKey, suggestKey);
}

void NXNavigationBarPrivate::_addFooterPage(QWidget* page, QString footKey)
{
    Q_Q(NXNavigationBar);
    Q_EMIT q->navigationNodeAdded(NXNavigationType::FooterNode, footKey, page);
    if (page)
    {
        page->setProperty("NXPageKey", footKey);
    }
    _footerView->setFixedHeight(40 * _footerModel->getFooterNodeCount());
    NXNavigationNode* node = _footerModel->getNavigationNode(footKey);
    QVariantMap suggestData;
    suggestData.insert("NXNodeType", "Footer");
    suggestData.insert("NXPageKey", footKey);
    QString suggestKey = _navigationSuggestBox->addSuggestion(node->getAwesome(), node->getNodeTitle(), suggestData);
    _suggestKeyMap.insert(footKey, suggestKey);
}

void NXNavigationBarPrivate::_raiseNavigationBar()
{
    Q_Q(NXNavigationBar);
    q->raise();
}

void NXNavigationBarPrivate::_doComponentAnimation(NXNavigationType::NavigationDisplayMode displayMode, bool isAnimation)
{
    switch (displayMode)
    {
    case NXNavigationType::Minimal:
    {
        _doNavigationBarWidthAnimation(displayMode, isAnimation);
        if (_currentDisplayMode == NXNavigationType::Maximal)
        {
            _searchButton->setVisible(true);
            _userCard->setVisible(false);
            _navigationSuggestBox->setVisible(false);
            if (_isShowUserCard)
            {
                _userButton->setVisible(true);
            }
            _handleNavigationExpandState(true);
        }
        _currentDisplayMode = displayMode;
        break;
    }
    case NXNavigationType::Compact:
    {
        _doNavigationBarWidthAnimation(displayMode, isAnimation);
        _doNavigationViewWidthAnimation(isAnimation);
        if (_currentDisplayMode != NXNavigationType::Minimal)
        {
            _handleMaximalToCompactLayout();
            _doNavigationButtonAnimation(true, isAnimation);
            _doSearchButtonAnimation(true, isAnimation);
            _doUserButtonAnimation(true, isAnimation);
            _navigationSuggestBox->setVisible(false);
            _handleNavigationExpandState(true);
        }
        _currentDisplayMode = displayMode;
        break;
    }
    case NXNavigationType::Maximal:
    {
        _resetLayout();
        _handleCompactToMaximalLayout();
        _doNavigationBarWidthAnimation(displayMode, isAnimation);
        _doUserButtonAnimation(false, isAnimation);
        _doNavigationButtonAnimation(false, isAnimation);
        _doSearchButtonAnimation(false, isAnimation);
        _navigationSuggestBox->setVisible(true);
        _currentDisplayMode = displayMode;
        _handleNavigationExpandState(false);
        break;
    }
    default:
    {
        break;
    }
    }
}

void NXNavigationBarPrivate::_handleNavigationExpandState(bool isSave)
{
    if (isSave)
    {
        _lastExpandedNodesList = _navigationModel->getRootExpandedNodes();
        for (auto node : _lastExpandedNodesList)
        {
            onTreeViewClicked(node->getModelIndex(), false);
        }
    }
    else
    {
        for (auto node : _lastExpandedNodesList)
        {
            _navigationView->resize(295, _navigationView->height());
            onTreeViewClicked(node->getModelIndex(), false);
        }
    }
}

void NXNavigationBarPrivate::_handleMaximalToCompactLayout()
{
    while (_navigationButtonLayout->count())
    {
        _navigationButtonLayout->takeAt(0);
    }
    if (_isShowUserCard)
    {
        _navigationButtonLayout->addSpacing(76);
    }
    else
    {
        _navigationButtonLayout->addSpacing(40);
    }

    _navigationSuggestLayout->addStretch();

    while (_userButtonLayout->count())
    {
        _userButtonLayout->takeAt(0);
    }
    _userButtonLayout->addSpacing(36);
}

void NXNavigationBarPrivate::_handleCompactToMaximalLayout()
{
    while (_navigationButtonLayout->count())
    {
        _navigationButtonLayout->takeAt(0);
    }
    _navigationButtonLayout->addSpacing(38);
    _navigationSuggestLayout->insertSpacing(0, 46);

    while (_userButtonLayout->count())
    {
        _userButtonLayout->takeAt(0);
    }
    if (_isShowUserCard)
    {
        _userButtonLayout->addSpacing(74);
    }
}

void NXNavigationBarPrivate::_resetLayout()
{
    while (_navigationButtonLayout->count())
    {
        _navigationButtonLayout->takeAt(0);
    }
    _navigationButtonLayout->addWidget(_navigationButton);
    _navigationButtonLayout->addWidget(_searchButton);

    while (_navigationSuggestLayout->count())
    {
        _navigationSuggestLayout->takeAt(0);
    }
    _navigationSuggestLayout->addLayout(_navigationButtonLayout);
    _navigationSuggestLayout->addWidget(_navigationSuggestBox);

    while (_userButtonLayout->count())
    {
        _userButtonLayout->takeAt(0);
    }
    _userButtonLayout->addWidget(_userButton);
}

void NXNavigationBarPrivate::_doNavigationBarWidthAnimation(NXNavigationType::NavigationDisplayMode displayMode, bool isAnimation)
{
    Q_Q(NXNavigationBar);
    QPropertyAnimation* navigationBarWidthAnimation = new QPropertyAnimation(q, "maximumWidth");
    navigationBarWidthAnimation->setEasingCurve(QEasingCurve::OutCubic);
    navigationBarWidthAnimation->setStartValue(q->width());
    navigationBarWidthAnimation->setDuration(isAnimation ? 285 : 0);
    switch (displayMode)
    {
    case NXNavigationType::Minimal:
    {
        connect(navigationBarWidthAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            q->setFixedWidth(value.toUInt());
            });
        navigationBarWidthAnimation->setEndValue(0);
        break;
    }
    case NXNavigationType::Compact:
    {
        connect(navigationBarWidthAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            q->setFixedWidth(value.toUInt());
            });
        navigationBarWidthAnimation->setEndValue(47);
        break;
    }
    case NXNavigationType::Maximal:
    {
        connect(navigationBarWidthAnimation, &QPropertyAnimation::finished, this, [=]() {
            _resetLayout();
            });
        connect(navigationBarWidthAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            q->setFixedWidth(value.toUInt());
            });
        navigationBarWidthAnimation->setEndValue(300);
        break;
    }
    default:
    {
        break;
    }
    }
    navigationBarWidthAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void NXNavigationBarPrivate::_doNavigationViewWidthAnimation(bool isAnimation)
{
    QPropertyAnimation* navigationViewWidthAnimation = new QPropertyAnimation(this, "pNavigationViewWidth");
    connect(navigationViewWidthAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        _navigationView->setColumnWidth(0, value.toUInt());
        });
    navigationViewWidthAnimation->setEasingCurve(QEasingCurve::OutCubic);
    navigationViewWidthAnimation->setStartValue(_navigationView->columnWidth(0));
    navigationViewWidthAnimation->setEndValue(40);
    navigationViewWidthAnimation->setDuration(isAnimation ? 285 : 0);
    navigationViewWidthAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void NXNavigationBarPrivate::_doNavigationButtonAnimation(bool isCompact, bool isAnimation)
{
    if (isCompact)
    {
        QPropertyAnimation* navigationButtonAnimation = new QPropertyAnimation(_navigationButton, "pos");
        connect(navigationButtonAnimation, &QPropertyAnimation::finished, this, [=]() {
            _resetLayout();
            });
        QPoint navigationButtonPos = _navigationButton->pos();
        navigationButtonAnimation->setStartValue(navigationButtonPos);
        if (_isShowUserCard)
        {
            navigationButtonAnimation->setEndValue(QPoint(0, 56));
        }
        else
        {
            navigationButtonAnimation->setEndValue(navigationButtonPos);
        }
        navigationButtonAnimation->setEasingCurve(QEasingCurve::OutCubic);
        navigationButtonAnimation->setDuration(isAnimation ? 285 : 0);
        navigationButtonAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else
    {
        QPropertyAnimation* navigationButtonAnimation = new QPropertyAnimation(_navigationButton, "pos");
        QPoint navigationButtonPos = _navigationButton->pos();
        navigationButtonAnimation->setStartValue(navigationButtonPos);
        if (_isShowUserCard)
        {
            navigationButtonAnimation->setEndValue(QPoint(0, 94));
        }
        else
        {
            navigationButtonAnimation->setEndValue(navigationButtonPos);
        }
        navigationButtonAnimation->setEasingCurve(QEasingCurve::InOutSine);
        navigationButtonAnimation->setDuration(isAnimation ? 130 : 0);
        navigationButtonAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void NXNavigationBarPrivate::_doSearchButtonAnimation(bool isCompact, bool isAnimation)
{
    if (isCompact)
    {
        QPoint navigationButtonPos = _navigationButton->pos();
        
        QPropertyAnimation* searchButtonAnimation = new QPropertyAnimation(_searchButton, "pos");
        if (_isShowUserCard)
        {
            searchButtonAnimation->setStartValue(QPoint(200, navigationButtonPos.y()));
            searchButtonAnimation->setEndValue(QPoint(0, navigationButtonPos.y()));
        }
        else
        {
            searchButtonAnimation->setStartValue(QPoint(200, navigationButtonPos.y() + 38));
            searchButtonAnimation->setEndValue(QPoint(0, navigationButtonPos.y() + 38));
        }
        searchButtonAnimation->setEasingCurve(QEasingCurve::OutCubic);
        searchButtonAnimation->setDuration(isAnimation ? 285 : 0);
        searchButtonAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        _searchButton->setVisible(true);
    }
    else
    {
        _searchButton->setVisible(false);
    }
}

void NXNavigationBarPrivate::_doUserButtonAnimation(bool isCompact, bool isAnimation)
{
    if (isCompact)
    {
        _userCard->setVisible(false);
        if (_isShowUserCard)
        {
            _userButton->setVisible(true);
        }
        QPropertyAnimation* userButtonAnimation = new QPropertyAnimation(_userButton, "geometry");
        connect(userButtonAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            _userButton->setFixedSize(value.toRect().size());
            });
        userButtonAnimation->setEasingCurve(QEasingCurve::OutCubic);
        userButtonAnimation->setStartValue(QRect(13, 18, 64, 64));
        userButtonAnimation->setEndValue(QRect(3, 10, 36, 36));
        userButtonAnimation->setDuration(isAnimation ? 285 : 0);
        userButtonAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else
    {
        QPropertyAnimation* userButtonAnimation = new QPropertyAnimation(_userButton, "geometry");
        connect(userButtonAnimation, &QPropertyAnimation::finished, this, [=]() {
            if (_isShowUserCard)
            {
                _userCard->setVisible(true);
            }
            _userButton->setFixedSize(36, 36);
            _userButton->setGeometry(QRect(3, 10, 36, 36));
            _userButton->setVisible(false);
            });
        connect(userButtonAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            _userButton->setFixedSize(value.toRect().size());
            });
        userButtonAnimation->setEasingCurve(QEasingCurve::InOutSine);
        userButtonAnimation->setStartValue(QRect(3, 10, 36, 36));
        userButtonAnimation->setEndValue(QRect(13, 18, 64, 64));
        userButtonAnimation->setDuration(isAnimation ? 130 : 0);
        userButtonAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}
