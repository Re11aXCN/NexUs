#ifndef NXWINDOWPRIVATE_H
#define NXWINDOWPRIVATE_H

#include <QLinearGradient>
#include <QMap>
#include <QObject>

#include "NXDef.h"
class NXEvent;
class NXWindow;
class NXNavigationBar;
class NXAppBar;
class NXCentralStackedWidget;
class QHBoxLayout;
class QVBoxLayout;
class NXThemeAnimationWidget;
class NXWindowPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXWindow)
    Q_PROPERTY_CREATE_D(int, ThemeChangeTime)
    Q_PROPERTY_CREATE_D(NXNavigationType::NavigationDisplayMode, NavigationBarDisplayMode)
public:
    explicit NXWindowPrivate(QObject* parent = nullptr);
    ~NXWindowPrivate() override;
    Q_SLOT void onNavigationButtonClicked();
    Q_INVOKABLE void onWMWindowClickedEvent(QVariantMap data);
    Q_SLOT void onxThemeReadyChange();
    Q_SLOT void onDisplayModeChanged();
    Q_SLOT void onxThemeModeChanged(NXThemeType::ThemeMode themeMode);
    Q_SLOT void onNavigationNodeClicked(NXNavigationType::NavigationNodeType nodeType, const QString& nodeKey);
    Q_SLOT void onNavigationNodeAdded(NXNavigationType::NavigationNodeType nodeType, const QString& nodeKey, QWidget* page);
    Q_SLOT void onNavigationNodeRemoved(NXNavigationType::NavigationNodeType nodeType, const QString& nodeKey);

private:
    bool _isWindowClosing{ false };
    bool _isNavigationDisplayModeChanged{ false };
    bool _isNavigationEnable{ true };
    bool _isNavigationBarExpanded{ false };
    bool _isWMClickedAnimationFinished{ true };
    bool _isInitFinished{ false };
    int _contentsMargins{ 5 };
    int _navigationTargetIndex{ 0 };

    NXNavigationType::NavigationDisplayMode _currentNavigationBarDisplayMode{ NXNavigationType::Maximal };
    NXThemeType::ThemeMode _themeMode;
    QMap<QString, QWidget*> _routeMap; // key__nodeKey title可以一致  value__Page
    QImage _lightBaseImage;
    QImage _darkBaseImage;
    NXEvent* _focusEvent{ nullptr };
    NXNavigationBar* _navigationBar{ nullptr };
    NXCentralStackedWidget* _centerStackedWidget{ nullptr };
    NXAppBar* _appBar{ nullptr };
    QHBoxLayout* _centerLayout{ nullptr };
    NXThemeAnimationWidget* _animationWidget{ nullptr };
    std::tuple<NXNavigationType::NavigationNodeType, QString, QWidget*> _currentVisibleWidget{ NXNavigationType::NavigationNodeType::PageNode, QString{}, nullptr};
    qreal _distance(QPoint point1, QPoint point2);
    void _resetWindowLayout(bool isAnimation);
    void _doNavigationDisplayModeChange();
};

#endif // NXWINDOWPRIVATE_H
