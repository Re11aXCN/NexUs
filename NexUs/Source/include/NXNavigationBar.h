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
public:
    explicit NXNavigationBar(QWidget* parent = nullptr);
    ~NXNavigationBar() override;
    void setUserInfoCardVisible(bool isVisible);
    void setUserInfoCardPixmap(QPixmap pix);
    void setUserInfoCardTitle(const QString& title);
    void setUserInfoCardSubTitle(const QString& subTitle);
    //template<typename Func>
    void setNavigationPageOpenPolicy(std::function<void(const QString&/*nodeKey*/)>&& openNavigationPageFunc);
    void setIsLeftButtonPressedToggleNavigation(bool isPressed);
    void setNavigationNodeDragAndDropEnable(bool isEnable);

    NodeOperateReturnTypeWithKey addExpanderNode(const QString& expanderTitle, NXIconType::IconName awesome = NXIconType::None);
    NodeOperateReturnTypeWithKey addExpanderNode(const QString& expanderTitle, const QString& targetExpanderKey, NXIconType::IconName awesome = NXIconType::None);
    NodeOperateReturnTypeWithKey addPageNode(const QString& pageTitle, QWidget* page, NXIconType::IconName awesome = NXIconType::None);
    NodeOperateReturnTypeWithKey addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, NXIconType::IconName awesome = NXIconType::None);
    NodeOperateReturnTypeWithKey addPageNode(const QString& pageTitle, QWidget* page, int keyPoints = 0, NXIconType::IconName awesome = NXIconType::None);
    NodeOperateReturnTypeWithKey addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, int keyPoints = 0, NXIconType::IconName awesome = NXIconType::None);
    NodeOperateReturnTypeWithKey addFooterNode(const QString& footerTitle, int keyPoints = 0, NXIconType::IconName awesome = NXIconType::None);
    NodeOperateReturnTypeWithKey addFooterNode(const QString& footerTitle, QWidget* page, int keyPoints = 0, NXIconType::IconName awesome = NXIconType::None);

    bool getNavigationNodeIsExpanded(const QString& expanderKey) const;
    void expandNavigationNode(const QString& expanderKey);
    void collpaseNavigationNode(const QString& expanderKey);
    void removeNavigationNode(const QString& nodeKey);

    void setNodeKeyPoints(const QString& nodeKey, int keyPoints);
    int getNodeKeyPoints(const QString& nodeKey) const;

    void navigation(const QString& pageKey, bool isLogClicked = true);
    void setDisplayMode(NXNavigationType::NavigationDisplayMode displayMode, bool isAnimation = true);
    NXNavigationType::NodeOperateReturnType setNodeTitle(const QString& nodeTitle, const QString& nodeKey);
    Q_SLOT NXNavigationType::NodeOperateReturnType navigationPageNodeSwitch(const QString& targetPageNodeKey);
Q_SIGNALS:
    Q_SIGNAL void userInfoCardClicked();
    Q_SIGNAL void navigationNodeClicked(NXNavigationType::NavigationNodeType nodeType, const QString& nodeKey);
    Q_SIGNAL void navigationNodeAdded(NXNavigationType::NavigationNodeType nodeType, const QString& nodeKey, QWidget* page);
    Q_SIGNAL void navigationNodeRemoved(NXNavigationType::NavigationNodeType nodeType, const QString& nodeKey);
protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // NXNAVIGATIONBAR_H
