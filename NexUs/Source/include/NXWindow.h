#ifndef NXWINDOW_H
#define NXWINDOW_H

#include <QMainWindow>

#include "NXAppBar.h"
class NXWindowPrivate;
class NX_EXPORT NXWindow : public QMainWindow
{
    Q_OBJECT
    Q_Q_CREATE(NXWindow)
    Q_PROPERTY_CREATE_Q_H(bool, IsCentralStackedWidgetTransparent)
    Q_PROPERTY_CREATE_Q_H(bool, IsStayTop)
    Q_PROPERTY_CREATE_Q_H(bool, IsFixedSize)
    Q_PROPERTY_CREATE_Q_H(bool, IsDefaultClosed)
    Q_PROPERTY_CREATE_Q_H(int, AppBarHeight)
    Q_PROPERTY_CREATE_Q_H(int, CustomWidgetMaximumWidth)
    Q_PROPERTY_CREATE_Q_H(int, ThemeChangeTime)
    Q_PROPERTY_CREATE_Q_H(NXNavigationType::NavigationDisplayMode, NavigationBarDisplayMode)
    Q_TAKEOVER_NATIVEEVENT_H
public:
    explicit NXWindow(QWidget* parent = nullptr);
    ~NXWindow() override;

    void moveToCenter();

    void setCustomWidget(NXAppBarType::CustomArea customArea, QWidget* customWidget);
    QWidget* getCustomWidget() const;
    void setIsNavigationBarEnable(bool isEnable);
    bool getIsNavigationBarEnable() const;
    void setIsLeftButtonPressedToggleNavigation(bool isPressed);
    void setNavigationNodeDragAndDropEnable(bool isEnable);
    void setUserInfoCardVisible(bool isVisible);
    void setUserInfoCardPixmap(QPixmap pix);
    void setUserInfoCardTitle(const QString& title);
    void setUserInfoCardSubTitle(const QString& subTitle);
    NXNavigationType::NodeOperateReturnType setNodeTitle(const QString& nodeTitle, const QString& nodeKey);

    NodeOperateReturnTypeWithKey addExpanderNode(const QString& expanderTitle, NXIconType::IconName awesome = NXIconType::None) const;
    NodeOperateReturnTypeWithKey addExpanderNode(const QString& expanderTitle, const QString& targetExpanderKey, NXIconType::IconName awesome = NXIconType::None) const;
    NodeOperateReturnTypeWithKey addPageNode(const QString& pageTitle, QWidget* page, NXIconType::IconName awesome = NXIconType::None) const;
    NodeOperateReturnTypeWithKey addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, NXIconType::IconName awesome = NXIconType::None) const;
    NodeOperateReturnTypeWithKey addPageNode(const QString& pageTitle, QWidget* page, int keyPoints = 0, NXIconType::IconName awesome = NXIconType::None) const;
    NodeOperateReturnTypeWithKey addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, int keyPoints = 0, NXIconType::IconName awesome = NXIconType::None) const;
    NodeOperateReturnTypeWithKey addFooterNode(const QString& footerTitle, int keyPoints = 0, NXIconType::IconName awesome = NXIconType::None) const;
    NodeOperateReturnTypeWithKey addFooterNode(const QString& footerTitle, QWidget* page, int keyPoints = 0, NXIconType::IconName awesome = NXIconType::None) const;

    bool getNavigationNodeIsExpanded(const QString& expanderKey) const;
    void expandNavigationNode(const QString& expanderKey);
    void collpaseNavigationNode(const QString& expanderKey);
    // 内部已经执行page的deleteLater()
    void removeNavigationNode(const QString& nodeKey) const;

    void setNodeKeyPoints(const QString& nodeKey, int keyPoints);
    int getNodeKeyPoints(const QString& nodeKey) const;

    std::tuple<NXNavigationType::NavigationNodeType, QString, QWidget*> currentVisibleWidget() const;
    void navigation(const QString& pageKey);
    void setWindowButtonFlag(NXAppBarType::ButtonType buttonFlag, bool isEnable = true);
    void setWindowButtonFlags(NXAppBarType::ButtonFlags buttonFlags);
    NXAppBarType::ButtonFlags getWindowButtonFlags() const;

    void closeWindow();
    Q_SLOT NXNavigationType::NodeOperateReturnType navigationPageNodeSwitch(const QString& targetPageKey);
    void setNavigationPageOpenPolicy(std::function<void(const QString&/*nodeKey*/)>&& openNavigationPageFunc);
Q_SIGNALS:
    Q_SIGNAL void userInfoCardClicked();
    Q_SIGNAL void closeButtonClicked();
    Q_SIGNAL void navigationNodeClicked(NXNavigationType::NavigationNodeType nodeType, const QString& nodeKey);
    Q_SIGNAL void navigationNodeToggled(NXNavigationType::NavigationNodeType nodeType, const QString& nodeKey, QWidget* widget);
    Q_SIGNAL void navigationNodeAdded(NXNavigationType::NavigationNodeType nodeType, const QString& nodeKey, QWidget* widget);
    Q_SIGNAL void navigationNodeRemoved(NXNavigationType::NavigationNodeType nodeType, const QString& nodeKey);
    Q_SIGNAL void customWidgetChanged();
protected:
    virtual bool eventFilter(QObject* watched, QEvent* event) override;
    virtual QMenu* createPopupMenu() override;
};

#endif // NXWINDOW_H
