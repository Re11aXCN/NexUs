#ifndef NXNAVIGATIONBAR_H
#define NXNAVIGATIONBAR_H

#include <QWidget>

#include "NXDef.h"

class NXNavigationBarPrivate;
class NX_EXPORT NXNavigationBar : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(NXNavigationBar)
    Q_PROPERTY_CREATE_Q_H(bool, IsTransparent)
    Q_PROPERTY_CREATE_Q_H(bool, IsAllowPageOpenInNewWindow)
    Q_PROPERTY_CREATE_Q_H(int, NavigationBarWidth)
public:
    explicit NXNavigationBar(QWidget* parent = nullptr);
    ~NXNavigationBar() override;
    void setUserInfoCardVisible(bool isVisible);
    void setUserInfoCardPixmap(QPixmap pix);
    void setUserInfoCardTitle(const QString& title);
    void setUserInfoCardSubTitle(const QString& subTitle);
    void setNavigationPageOpenPolicy(std::function<void(const QString&/*nodeKey*/)>&& openNavigationPageFunc);
    void setIsLeftButtonPressedToggleNavigation(bool isPressed);
    void setNavigationNodeDragAndDropEnable(bool isEnable);
    void setToolTipOffset(int offsetX, int offsetY);

	NodeOperateReturnTypeWithKey addExpanderNode(const QString& expanderTitle, NXIconType::IconName awesome = NXIconType::None);
    NodeOperateReturnTypeWithKey addExpanderNode(const QString& expanderTitle, const QString& targetExpanderKey, NXIconType::IconName awesome = NXIconType::None);
    NodeOperateReturnTypeWithKey addPageNode(const QString& pageTitle, QWidget* page, NXIconType::IconName awesome = NXIconType::None);
    NodeOperateReturnTypeWithKey addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, NXIconType::IconName awesome = NXIconType::None);
    NodeOperateReturnTypeWithKey addPageNode(const QString& pageTitle, QWidget* page, int keyPoints = 0, NXIconType::IconName awesome = NXIconType::None);
    NodeOperateReturnTypeWithKey addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, int keyPoints = 0, NXIconType::IconName awesome = NXIconType::None);
    NodeOperateReturnTypeWithKey addFooterNode(const QString& footerTitle, int keyPoints = 0, NXIconType::IconName awesome = NXIconType::None);
    NodeOperateReturnTypeWithKey addFooterNode(const QString& footerTitle, QWidget* page, int keyPoints = 0, NXIconType::IconName awesome = NXIconType::None);

    QString getNavigationRootKey() const;
    bool getNavigationNodeIsExpanded(const QString& expanderKey) const;
    void expandNavigationNode(const QString& expanderKey);
    void collapseNavigationNode(const QString& expanderKey);
    void removeNavigationNode(const QString& nodeKey);

    void setNodeKeyPoints(const QString& nodeKey, int keyPoints);
    int getNodeKeyPoints(const QString& nodeKey) const;
    NXNavigationType::NodeOperateReturnType setNavigationNodeTitle(const QString& nodeTitle, const QString& nodeKey);
    QString getNavigationNodeTitle(const QString& nodeKey) const;

    void navigation(const QString& pageKey, bool isLogClicked = true, bool isRouteBack = false);
    void setDisplayMode(NXNavigationType::NavigationDisplayMode displayMode, bool isAnimation = true);
    int getPageOpenInNewWindowCount(const QString& nodeKey) const;

    Q_SLOT NXNavigationType::NodeOperateReturnType navigationPageNodeSwitch(const QString& targetPageNodeKey);
Q_SIGNALS:
    Q_SIGNAL void pageOpenInNewWindow(const QString& nodeKey);
    Q_SIGNAL void userInfoCardClicked();
    Q_SIGNAL void navigationNodeClicked(NXNavigationType::NavigationNodeType nodeType, const QString& clickedNodeKey, bool isRouteBack);
    Q_SIGNAL void navigationNodeAdded(NXNavigationType::NavigationNodeType nodeType, const QString& addedNodeKey, QWidget* page);
    Q_SIGNAL void navigationNodeRemoved(NXNavigationType::NavigationNodeType nodeType, const QString& removedNodeKey);
    Q_SIGNAL void displayModeChanged(NXNavigationType::NavigationDisplayMode displayMode);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // NXNAVIGATIONBAR_H
