﻿#ifndef NXWINDOW_H
#define NXWINDOW_H

#include <QMainWindow>
#include "NXAppBar.h"
class NXWindowPrivate;
class NX_EXPORT NXWindow : public QMainWindow
{
    Q_OBJECT
    Q_Q_CREATE(NXWindow)
    Q_PROPERTY_CREATE_Q_H(bool, IsStayTop)
    Q_PROPERTY_CREATE_Q_H(bool, IsFixedSize)
    Q_PROPERTY_CREATE_Q_H(bool, IsDefaultClosed)
    Q_PROPERTY_CREATE_Q_H(bool, IsCentralStackedWidgetTransparent)
    Q_PROPERTY_CREATE_Q_H(bool, IsAllowPageOpenInNewWindow)
    Q_PROPERTY_CREATE_Q_H(bool, IsNavigationBarEnable)
    Q_PROPERTY_CREATE_Q_H(int, NavigationBarWidth)
    Q_PROPERTY_CREATE_Q_H(int, CurrentStackIndex)
    Q_PROPERTY_CREATE_Q_H(int, AppBarHeight)
    Q_PROPERTY_CREATE_Q_H(int, CustomWidgetMaximumWidth)
    Q_PROPERTY_CREATE_Q_H(int, ThemeChangeTime)
    Q_PROPERTY_CREATE_Q_H(NXNavigationType::NavigationDisplayMode, NavigationBarDisplayMode)
    Q_PROPERTY_CREATE_Q_H(NXWindowType::StackSwitchMode, StackSwitchMode)
    Q_TAKEOVER_NATIVEEVENT_H
public:
    Q_INVOKABLE explicit NXWindow(QWidget* parent = nullptr);
    ~NXWindow() override;

    void moveToCenter();

    void setCustomWidget(NXAppBarType::CustomArea customArea, QWidget* customWidget);
    QWidget* getCustomWidget() const;

    void setCentralCustomWidget(QWidget* customWidget);
    QWidget* getCentralCustomWidget() const;

    void setCustomMenu(QMenu* customMenu);
    QMenu* getCustomMenu() const;

 	void setIsLeftButtonPressedToggleNavigation(bool isPressed);
    void setNavigationNodeDragAndDropEnable(bool isEnable);
    void setUserInfoCardVisible(bool isVisible);
    void setUserInfoCardPixmap(QPixmap pix);
    void setUserInfoCardTitle(const QString& title);
    void setUserInfoCardSubTitle(const QString& subTitle);

    NXNavigationType::NodeOperateReturnType setNavigationNodeTitle(const QString& nodeTitle, const QString& nodeKey);
    QString getNavigationNodeTitle(const QString& nodeKey) const;

    NodeOperateReturnTypeWithKey addExpanderNode(const QString& expanderTitle, NXIconType::IconName awesome = NXIconType::None) const;
    NodeOperateReturnTypeWithKey addExpanderNode(const QString& expanderTitle, const QString& targetExpanderKey, NXIconType::IconName awesome = NXIconType::None) const;
    NodeOperateReturnTypeWithKey addPageNode(const QString& pageTitle, QWidget* page, NXIconType::IconName awesome = NXIconType::None);
    NodeOperateReturnTypeWithKey addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, NXIconType::IconName awesome = NXIconType::None);
    NodeOperateReturnTypeWithKey addPageNode(const QString& pageTitle, QWidget* page, int keyPoints = 0, NXIconType::IconName awesome = NXIconType::None);
    NodeOperateReturnTypeWithKey addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, int keyPoints = 0, NXIconType::IconName awesome = NXIconType::None);
    NodeOperateReturnTypeWithKey addFooterNode(const QString& footerTitle, int keyPoints = 0, NXIconType::IconName awesome = NXIconType::None) const;
    NodeOperateReturnTypeWithKey addFooterNode(const QString& footerTitle, QWidget* page, int keyPoints = 0, NXIconType::IconName awesome = NXIconType::None);

    void addCentralWidget(QWidget* centralWidget);
    QWidget* getCentralWidget(int index) const;

	QString getNavigationRootKey() const;
    bool getNavigationNodeIsExpanded(const QString& expanderKey) const;
    void expandNavigationNode(const QString& expanderKey);
    void collapseNavigationNode(const QString& expanderKey);
	// 内部已经执行page的deleteLater()
    void removeNavigationNode(const QString& nodeKey) const;
    int getPageOpenInNewWindowCount(const QString& nodeKey) const;
    void backtrackNavigationNode(const QString& nodeKey);

    void setNodeKeyPoints(const QString& nodeKey, int keyPoints);
    int getNodeKeyPoints(const QString& nodeKey) const;

    void navigation(const QString& pageKey);
    int getCurrentNavigationIndex() const;
    QString getCurrentNavigationPageKey() const;

    void setWindowButtonFlag(NXAppBarType::ButtonType buttonFlag, bool isEnable = true);
    void setWindowButtonFlags(NXAppBarType::ButtonFlags buttonFlags);
    NXAppBarType::ButtonFlags getWindowButtonFlags() const;

    void closeWindow();
    Q_SLOT NXNavigationType::NodeOperateReturnType navigationPageNodeSwitch(const QString& targetPageKey);
    void setNavigationPageOpenPolicy(std::function<void(const QString&/*nodeKey*/)>&& openNavigationPageFunc);
Q_SIGNALS:
    Q_SIGNAL void userInfoCardClicked();
    Q_SIGNAL void closeButtonClicked();
    Q_SIGNAL void navigationNodeClicked(NXNavigationType::NavigationNodeType nodeType, const QString& clickedNodeKey, QWidget* clickedWidget);
    Q_SIGNAL void navigationNodeRemoved(NXNavigationType::NavigationNodeType nodeType, const QString& showNavWidgetNodeKey, QWidget* showNavWidget);
    Q_SIGNAL void customWidgetChanged();
    Q_SIGNAL void centralCustomWidgetChanged();
    Q_SIGNAL void customMenuChanged();
    Q_SIGNAL void pageOpenInNewWindow(const QString& nodeKey);
protected:
    virtual bool eventFilter(QObject* watched, QEvent* event) override;
    virtual QMenu* createPopupMenu() override;
};

#endif // NXWINDOW_H
