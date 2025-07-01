#include "NXNavigationBar.h"

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
NXNavigationBar::NXNavigationBar(QWidget* parent)
    : QWidget{parent}, d_ptr(new NXNavigationBarPrivate())
{
    Q_D(NXNavigationBar);
    d->q_ptr = this;
    setFixedWidth(300);
    d->_pIsTransparent = true;

    //鐢ㄦ埛鍗＄墖
    d->_userCard = new NXInteractiveCard(this);
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

    QObject::connect(d->_navigationSuggestBox, &NXSuggestBox::suggestionClicked, this, [=](QString suggestText, QVariantMap suggestData) {
        NXNavigationNode* node = nullptr;
        if (suggestData.value("NXNodeType").toString() == "Stacked")
        {
            node = d->_navigationModel->getNavigationNode(suggestData.value("NXPageKey").toString());
            if (node)
            {
                d->onTreeViewClicked(node->getModelIndex());
            }
        }
        else
        {
            node = d->_footerModel->getNavigationNode(suggestData.value("NXPageKey").toString());
            if (node)
            {
                d->onFooterViewClicked(node->getModelIndex());
            }
        }
    });

    d->_navigationModel = new NXNavigationModel(this);
    d->_navigationView = new NXNavigationView(this);
    d->_navigationView->setModel(d->_navigationModel);
    QObject::connect(d->_navigationView, &NXNavigationView::navigationClicked, this, [=](const QModelIndex& index) { d->onTreeViewClicked(index); });
    QObject::connect(d->_navigationView, &NXNavigationView::navigationOpenNewWindow, d, &NXNavigationBarPrivate::onNavigationOpenNewWindow);

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
    QObject::connect(d->_footerView, &NXBaseListView::clicked, this, [=](const QModelIndex& index) { d->onFooterViewClicked(index); });

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignLeft);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 10, 5, 0);
    mainLayout->addLayout(userCardLayout);
    mainLayout->addSpacing(4);
    mainLayout->addLayout(d->_navigationSuggestLayout);
    mainLayout->addSpacing(4);
    mainLayout->addWidget(d->_navigationView);
    mainLayout->addWidget(d->_footerView);

    //涓婚璁剧疆
    d->_themeMode = nxTheme->getThemeMode();
    QObject::connect(nxTheme, &NXTheme::themeModeChanged, this, [=](NXThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
    });
}

NXNavigationBar::~NXNavigationBar()
{
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

void NXNavigationBar::setUserInfoCardTitle(QString title)
{
    Q_D(NXNavigationBar);
    d->_userCard->setTitle(title);
}

void NXNavigationBar::setUserInfoCardSubTitle(QString subTitle)
{
    Q_D(NXNavigationBar);
    d->_userCard->setSubTitle(subTitle);
}

NXNavigationType::NodeOperateReturnType NXNavigationBar::addExpanderNode(QString expanderTitle, QString& expanderKey, NXIconType::IconName awesome)
{
    Q_D(NXNavigationBar);
    NXNavigationType::NodeOperateReturnType returnType = d_ptr->_navigationModel->addExpanderNode(expanderTitle, expanderKey, awesome);
    if (returnType == NXNavigationType::Success)
    {
        d->_initNodeModelIndex(QModelIndex());
        d->_resetNodeSelected();
    }
    return returnType;
}

NXNavigationType::NodeOperateReturnType NXNavigationBar::addExpanderNode(QString expanderTitle, QString& expanderKey, QString targetExpanderKey, NXIconType::IconName awesome)
{
    Q_D(NXNavigationBar);
    NXNavigationType::NodeOperateReturnType returnType = d->_navigationModel->addExpanderNode(expanderTitle, expanderKey, targetExpanderKey, awesome);
    if (returnType == NXNavigationType::Success)
    {
        d->_initNodeModelIndex(QModelIndex());
        d->_resetNodeSelected();
    }
    return returnType;
}

NXNavigationType::NodeOperateReturnType NXNavigationBar::addPageNode(QString pageTitle, QWidget* page, NXIconType::IconName awesome)
{
    Q_D(NXNavigationBar);
    if (!page)
    {
        return NXNavigationType::PageInvalid;
    }
    QString pageKey;
    NXNavigationType::NodeOperateReturnType returnType = d_ptr->_navigationModel->addPageNode(pageTitle, pageKey, awesome);
    if (returnType == NXNavigationType::Success)
    {
        d->_pageMetaMap.insert(pageKey, page->metaObject());
        d->_addStackedPage(page, pageKey);
        d->_initNodeModelIndex(QModelIndex());
        d->_resetNodeSelected();
    }
    return returnType;
}

NXNavigationType::NodeOperateReturnType NXNavigationBar::addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, NXIconType::IconName awesome)
{
    Q_D(NXNavigationBar);
    if (!page)
    {
        return NXNavigationType::PageInvalid;
    }
    if (targetExpanderKey.isEmpty())
    {
        return NXNavigationType::TargetNodeInvalid;
    }
    QString pageKey;
    NXNavigationType::NodeOperateReturnType returnType = d->_navigationModel->addPageNode(pageTitle, pageKey, targetExpanderKey, awesome);
    if (returnType == NXNavigationType::NodeOperateReturnType::Success)
    {
        d->_pageMetaMap.insert(pageKey, page->metaObject());
        NXNavigationNode* node = d->_navigationModel->getNavigationNode(pageKey);
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
        d_ptr->_addStackedPage(page, pageKey);
        d->_initNodeModelIndex(QModelIndex());
        d->_resetNodeSelected();
    }
    return returnType;
}

NXNavigationType::NodeOperateReturnType NXNavigationBar::addPageNode(QString pageTitle, QWidget* page, int keyPoints, NXIconType::IconName awesome)
{
    Q_D(NXNavigationBar);
    if (!page)
    {
        return NXNavigationType::PageInvalid;
    }
    QString pageKey;
    NXNavigationType::NodeOperateReturnType returnType = d_ptr->_navigationModel->addPageNode(pageTitle, pageKey, keyPoints, awesome);
    if (returnType == NXNavigationType::Success)
    {
        d->_pageMetaMap.insert(pageKey, page->metaObject());
        d->_addStackedPage(page, pageKey);
        d->_initNodeModelIndex(QModelIndex());
        d->_resetNodeSelected();
    }
    return returnType;
}

NXNavigationType::NodeOperateReturnType NXNavigationBar::addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, int keyPoints, NXIconType::IconName awesome)
{
    Q_D(NXNavigationBar);
    if (!page)
    {
        return NXNavigationType::PageInvalid;
    }
    if (targetExpanderKey.isEmpty())
    {
        return NXNavigationType::TargetNodeInvalid;
    }
    QString pageKey;
    NXNavigationType::NodeOperateReturnType returnType = d_ptr->_navigationModel->addPageNode(pageTitle, pageKey, targetExpanderKey, keyPoints, awesome);
    if (returnType == NXNavigationType::Success)
    {
        d->_pageMetaMap.insert(pageKey, page->metaObject());
        NXNavigationNode* node = d_ptr->_navigationModel->getNavigationNode(pageKey);
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
        d_ptr->_addStackedPage(page, pageKey);
        d->_initNodeModelIndex(QModelIndex());
        d->_resetNodeSelected();
    }
    return returnType;
}

NXNavigationType::NodeOperateReturnType NXNavigationBar::addFooterNode(QString footerTitle, QString& footerKey, int keyPoints, NXIconType::IconName awesome)
{
    return addFooterNode(footerTitle, nullptr, footerKey, keyPoints, awesome);
}

NXNavigationType::NodeOperateReturnType NXNavigationBar::addFooterNode(QString footerTitle, QWidget* page, QString& footerKey, int keyPoints, NXIconType::IconName awesome)
{
    NXNavigationType::NodeOperateReturnType returnType = d_ptr->_footerModel->addFooterNode(footerTitle, footerKey, page ? true : false, keyPoints, awesome);
    if (returnType == NXNavigationType::Success)
    {
        d_ptr->_addFooterPage(page, footerKey);
    }
    return returnType;
}

bool NXNavigationBar::getNavigationNodeIsExpanded(QString expanderKey) const
{
    Q_D(const NXNavigationBar);
    NXNavigationNode* node = d->_navigationModel->getNavigationNode(expanderKey);
    if (!node || !node->getIsExpanderNode())
    {
        return false;
    }
    return d->_navigationView->isExpanded(node->getModelIndex());
}

void NXNavigationBar::expandNavigationNode(QString expanderKey)
{
    Q_D(NXNavigationBar);
    NXNavigationNode* node = d->_navigationModel->getNavigationNode(expanderKey);
    if (!node || !node->getIsExpanderNode())
    {
        return;
    }
    d->_expandOrCollpaseExpanderNode(node, true);
    d->_resetNodeSelected();
}

void NXNavigationBar::collpaseNavigationNode(QString expanderKey)
{
    Q_D(NXNavigationBar);
    NXNavigationNode* node = d->_navigationModel->getNavigationNode(expanderKey);
    if (!node || !node->getIsExpanderNode())
    {
        return;
    }
    d->_expandOrCollpaseExpanderNode(node, false);
    d->_resetNodeSelected();
}

void NXNavigationBar::removeNavigationNode(QString nodeKey)
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
            Q_EMIT navigationNodeRemoved(NXNavigationType::PageNode, removeKey);
        }
    }
    d->_navigationSuggestBox->removeSuggestion(d->_suggestKeyMap.value(nodeKey));
}

void NXNavigationBar::setNodeKeyPoints(QString nodeKey, int keyPoints)
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

int NXNavigationBar::getNodeKeyPoints(QString nodeKey) const
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

void NXNavigationBar::navigation(QString pageKey, bool isLogClicked)
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
            d->onFooterViewClicked(node->getModelIndex(), isLogClicked);
        }
        else
        {
            if (!node->getIsExpanderNode())
            {
                d->onTreeViewClicked(node->getModelIndex(), isLogClicked);
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
