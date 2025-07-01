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
    void setUserInfoCardVisible(bool isVisible);
    void setUserInfoCardPixmap(QPixmap pix);
    void setUserInfoCardTitle(QString title);
    void setUserInfoCardSubTitle(QString subTitle);
    NXNavigationType::NodeOperateReturnType addExpanderNode(QString expanderTitle, QString& expanderKey, NXIconType::IconName awesome = NXIconType::None) const;
    NXNavigationType::NodeOperateReturnType addExpanderNode(QString expanderTitle, QString& expanderKey, QString targetExpanderKey, NXIconType::IconName awesome = NXIconType::None) const;
    NXNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, NXIconType::IconName awesome = NXIconType::None) const;
    NXNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, NXIconType::IconName awesome = NXIconType::None) const;
    NXNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, int keyPoints = 0, NXIconType::IconName awesome = NXIconType::None) const;
    NXNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, int keyPoints = 0, NXIconType::IconName awesome = NXIconType::None) const;
    NXNavigationType::NodeOperateReturnType addFooterNode(QString footerTitle, QString& footerKey, int keyPoints = 0, NXIconType::IconName awesome = NXIconType::None) const;
    NXNavigationType::NodeOperateReturnType addFooterNode(QString footerTitle, QWidget* page, QString& footerKey, int keyPoints = 0, NXIconType::IconName awesome = NXIconType::None) const;

    bool getNavigationNodeIsExpanded(QString expanderKey) const;
    void expandNavigationNode(QString expanderKey);
    void collpaseNavigationNode(QString expanderKey);
    void removeNavigationNode(QString nodeKey) const;

    void setNodeKeyPoints(QString nodeKey, int keyPoints);
    int getNodeKeyPoints(QString nodeKey) const;

    void navigation(QString pageKey);
    void setWindowButtonFlag(NXAppBarType::ButtonType buttonFlag, bool isEnable = true);
    void setWindowButtonFlags(NXAppBarType::ButtonFlags buttonFlags);
    NXAppBarType::ButtonFlags getWindowButtonFlags() const;

    void closeWindow();
Q_SIGNALS:
    Q_SIGNAL void userInfoCardClicked();
    Q_SIGNAL void closeButtonClicked();
    Q_SIGNAL void navigationNodeClicked(NXNavigationType::NavigationNodeType nodeType, QString nodeKey);
    Q_SIGNAL void customWidgetChanged();

protected:
    virtual bool eventFilter(QObject* watched, QEvent* event) override;
    virtual QMenu* createPopupMenu() override;
};

#endif // NXWINDOW_H
