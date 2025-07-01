#ifndef NXNAVIGATIONBARPRIVATE_H
#define NXNAVIGATIONBARPRIVATE_H

#include <QMap>
#include <QObject>

#include "NXDef.h"
class QLayout;
class NXMenu;
class QVBoxLayout;
class QHBoxLayout;
class QLinearGradient;

class NXNavigationBar;
class NXNavigationNode;
class NXNavigationModel;
class NXNavigationView;
class NXNavigationDelegate;
class NXSuggestBox;
class NXInteractiveCard;

class NXBaseListView;
class NXFooterModel;
class NXFooterDelegate;
class NXIconButton;
class NXToolButton;
class NXNavigationBarPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXNavigationBar)
    Q_PROPERTY_CREATE(int, NavigationViewWidth);
    Q_PROPERTY_CREATE_D(bool, IsTransparent)

public:
    explicit NXNavigationBarPrivate(QObject* parent = nullptr);
    ~NXNavigationBarPrivate() override;
    Q_SLOT void onNavigationButtonClicked();
    Q_SLOT void onNavigationOpenNewWindow(QString nodeKey);

    Q_INVOKABLE void onNavigationRouteBack(QVariantMap routeData);

    //鏍稿績璺宠浆閫昏緫
    void onTreeViewClicked(const QModelIndex& index, bool isLogRoute = true);
    void onFooterViewClicked(const QModelIndex& index, bool isLogRoute = true);

private:
    bool _isShowUserCard{ true };

    NXThemeType::ThemeMode _themeMode;
    NXNavigationType::NavigationDisplayMode _currentDisplayMode{ NXNavigationType::NavigationDisplayMode::Maximal };
    QMap<QString, QString> _suggestKeyMap;
    QMap<QString, const QMetaObject*> _pageMetaMap;
    QMap<NXNavigationNode*, NXMenu*> _compactMenuMap;
    QList<NXNavigationNode*> _lastExpandedNodesList;

    QVBoxLayout* _navigationButtonLayout{nullptr};
    QHBoxLayout* _navigationSuggestLayout{nullptr};
    QVBoxLayout* _userButtonLayout{nullptr};

    NXIconButton* _userButton{nullptr};
    NXToolButton* _searchButton{nullptr};
    NXToolButton* _navigationButton{nullptr};
    NXNavigationModel* _navigationModel{nullptr};
    NXNavigationView* _navigationView{nullptr};
    NXBaseListView* _footerView{nullptr};
    NXFooterModel* _footerModel{nullptr};
    NXFooterDelegate* _footerDelegate{nullptr};
    NXSuggestBox* _navigationSuggestBox{nullptr};
    NXInteractiveCard* _userCard{nullptr};

    void _initNodeModelIndex(const QModelIndex& parentIndex);
    void _resetNodeSelected();
    void _expandSelectedNodeParent();
    void _expandOrCollpaseExpanderNode(NXNavigationNode* node, bool isExpand);

    void _addStackedPage(QWidget* page, QString pageKey);
    void _addFooterPage(QWidget* page, QString footKey);

    void _raiseNavigationBar();

    void _doComponentAnimation(NXNavigationType::NavigationDisplayMode displayMode, bool isAnimation);
    void _handleNavigationExpandState(bool isSave);
    void _handleMaximalToCompactLayout();
    void _handleCompactToMaximalLayout();
    void _resetLayout();

    void _doNavigationBarWidthAnimation(NXNavigationType::NavigationDisplayMode displayMode, bool isAnimation);
    void _doNavigationViewWidthAnimation(bool isAnimation);
    void _doNavigationButtonAnimation(bool isCompact, bool isAnimation);
    void _doSearchButtonAnimation(bool isCompact, bool isAnimation);
    void _doUserButtonAnimation(bool isCompact, bool isAnimation);
};

#endif // NXNAVIGATIONBARPRIVATE_H
