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
    void setUserInfoCardTitle(QString title);
    void setUserInfoCardSubTitle(QString subTitle);

    NXNavigationType::NodeOperateReturnType addExpanderNode(QString expanderTitle, QString& expanderKey, NXIconType::IconName awesome = NXIconType::None);
    NXNavigationType::NodeOperateReturnType addExpanderNode(QString expanderTitle, QString& expanderKey, QString targetExpanderKey, NXIconType::IconName awesome = NXIconType::None);
    NXNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, NXIconType::IconName awesome = NXIconType::None);
    NXNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, NXIconType::IconName awesome = NXIconType::None);
    NXNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, int keyPoints = 0, NXIconType::IconName awesome = NXIconType::None);
    NXNavigationType::NodeOperateReturnType addPageNode(QString pageTitle, QWidget* page, QString targetExpanderKey, int keyPoints = 0, NXIconType::IconName awesome = NXIconType::None);
    NXNavigationType::NodeOperateReturnType addFooterNode(QString footerTitle, QString& footerKey, int keyPoints = 0, NXIconType::IconName awesome = NXIconType::None);
    NXNavigationType::NodeOperateReturnType addFooterNode(QString footerTitle, QWidget* page, QString& footerKey, int keyPoints = 0, NXIconType::IconName awesome = NXIconType::None);

    bool getNavigationNodeIsExpanded(QString expanderKey) const;
    void expandNavigationNode(QString expanderKey);
    void collpaseNavigationNode(QString expanderKey);
    void removeNavigationNode(QString nodeKey);

    void setNodeKeyPoints(QString nodeKey, int keyPoints);
    int getNodeKeyPoints(QString nodeKey) const;

    void navigation(QString pageKey, bool isLogClicked = true);
    void setDisplayMode(NXNavigationType::NavigationDisplayMode displayMode, bool isAnimation = true);

Q_SIGNALS:
    Q_SIGNAL void userInfoCardClicked();
    Q_SIGNAL void navigationNodeClicked(NXNavigationType::NavigationNodeType nodeType, QString nodeKey);
    Q_SIGNAL void navigationNodeAdded(NXNavigationType::NavigationNodeType nodeType, QString nodeKey, QWidget* page);
    Q_SIGNAL void navigationNodeRemoved(NXNavigationType::NavigationNodeType nodeType, QString nodeKey);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // NXNAVIGATIONBAR_H
