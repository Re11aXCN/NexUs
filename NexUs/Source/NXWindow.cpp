#include "NXWindow.h"

#include <QApplication>
#include <QDockWidget>
#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QResizeEvent>
#include <QScreen>
#include <QStackedWidget>
#include <QStyleOption>
#include <QTimer>
#include <QToolBar>
#include <QVBoxLayout>

#include "NXApplication.h"
#include "DeveloperComponents/NXCentralStackedWidget.h"
#include "NXEventBus.h"
#include "NXMenu.h"
#include "NXNavigationBar.h"
#include "NXNavigationRouter.h"
#include "NXTheme.h"
#include "DeveloperComponents/NXWindowStyle.h"
#include "private/NXAppBarPrivate.h"
#include "private/NXNavigationBarPrivate.h"
#include "private/NXWindowPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(NXWindow, int, ThemeChangeTime)
Q_PROPERTY_CREATE_Q_CPP(NXWindow, NXNavigationType::NavigationDisplayMode, NavigationBarDisplayMode)
Q_TAKEOVER_NATIVEEVENT_CPP(NXWindow, d_func()->_appBar);
NXWindow::NXWindow(QWidget* parent)
    : QMainWindow{parent}, d_ptr(new NXWindowPrivate())
{
    Q_D(NXWindow);
    d->q_ptr = this;
    setProperty("NXBaseClassName", "NXWindow");
    resize(1020, 680); 

    d->_pThemeChangeTime = 700;
    d->_pNavigationBarDisplayMode = NXNavigationType::NavigationDisplayMode::Auto;
    QObject::connect(this, &NXWindow::pNavigationBarDisplayModeChanged, d, &NXWindowPrivate::onDisplayModeChanged);

    d->_appBar = new NXAppBar(this);
    QObject::connect(d->_appBar, &NXAppBar::routeBackButtonClicked, this, []() {
        NXNavigationRouter::getInstance()->navigationRouteBack();
    });
    QObject::connect(d->_appBar, &NXAppBar::closeButtonClicked, this, &NXWindow::closeButtonClicked);
    
    d->_navigationBar = new NXNavigationBar(this);
    QObject::connect(NXNavigationRouter::getInstance(), &NXNavigationRouter::navigationRouterStateChanged, this, [d](bool isEnable) {
        d->_appBar->setRouteBackButtonEnable(isEnable);
    });

    QObject::connect(d->_navigationBar, &NXNavigationBar::userInfoCardClicked, this, &NXWindow::userInfoCardClicked);
    QObject::connect(d->_navigationBar, &NXNavigationBar::navigationNodeClicked, d, &NXWindowPrivate::onNavigationNodeClicked);
    QObject::connect(d->_navigationBar, &NXNavigationBar::navigationNodeAdded, d, &NXWindowPrivate::onNavigationNodeAdded);
    QObject::connect(d->_navigationBar, &NXNavigationBar::navigationNodeRemoved, d, &NXWindowPrivate::onNavigationNodeRemoved);

    d->_centerStackedWidget = new NXCentralStackedWidget(this);
    d->_centerStackedWidget->setContentsMargins(0, 0, 0, 0);
    QWidget* centralWidget = new QWidget(this);
    d->_centerLayout = new QHBoxLayout(centralWidget);
    d->_centerLayout->setSpacing(0);
    d->_centerLayout->addWidget(d->_navigationBar);
    d->_centerLayout->addWidget(d->_centerStackedWidget);
    d->_centerLayout->setContentsMargins(d->_contentsMargins, 0, 0, 0);

    d->_focusEvent = new NXEvent("WMWindowClicked", "onWMWindowClickedEvent", d);
    d->_focusEvent->registerAndInit();

    QObject::connect(d->_appBar, &NXAppBar::navigationButtonClicked, d, &NXWindowPrivate::onNavigationButtonClicked);

    d->_themeMode = nxTheme->getThemeMode();
    QObject::connect(nxTheme, &NXTheme::themeModeChanged, d, &NXWindowPrivate::onxThemeModeChanged);
    QObject::connect(d->_appBar, &NXAppBar::themeChangeButtonClicked, d, &NXWindowPrivate::onxThemeReadyChange);
    d->_isInitFinished = true;
    setCentralWidget(centralWidget);
    centralWidget->installEventFilter(this);

    setObjectName("NXWindow");
    setStyleSheet("#NXWindow{background-color:transparent;}");
    setStyle(new NXWindowStyle(style()));

    
    QTimer::singleShot(1, this, [=] {
        QPalette palette = this->palette();
        palette.setBrush(QPalette::Window, NXThemeColor(d->_themeMode, WindowBase));
        this->setPalette(palette);
    });
    nxApp->syncMica(this);
    QObject::connect(nxApp, &NXApplication::pIsEnableMicaChanged, this, [=]() {
        d->onxThemeModeChanged(d->_themeMode);
        });
}

NXWindow::~NXWindow()
{
}

void NXWindow::setIsStayTop(bool isStayTop)
{
    Q_D(NXWindow);
    d->_appBar->setIsStayTop(isStayTop);
    Q_EMIT pIsStayTopChanged();
}

bool NXWindow::getIsStayTop() const
{
    return d_ptr->_appBar->getIsStayTop();
}

void NXWindow::setIsFixedSize(bool isFixedSize)
{
    Q_D(NXWindow);
    d->_appBar->setIsFixedSize(isFixedSize);
    Q_EMIT pIsFixedSizeChanged();
}

bool NXWindow::getIsFixedSize() const
{
    return d_ptr->_appBar->getIsFixedSize();
}

void NXWindow::setIsDefaultClosed(bool isDefaultClosed)
{
    Q_D(NXWindow);
    d->_appBar->setIsDefaultClosed(isDefaultClosed);
    Q_EMIT pIsDefaultClosedChanged();
}

bool NXWindow::getIsDefaultClosed() const
{
    return d_ptr->_appBar->getIsDefaultClosed();
}

void NXWindow::setAppBarHeight(int appBarHeight)
{
    Q_D(NXWindow);
    d->_appBar->setAppBarHeight(appBarHeight);
    Q_EMIT pAppBarHeightChanged();
}

int NXWindow::getAppBarHeight() const
{
    Q_D(const NXWindow);
    return d->_appBar->getAppBarHeight();
}

QWidget* NXWindow::getCustomWidget() const
{
    Q_D(const NXWindow);
    return d->_appBar->getCustomWidget();
}

void NXWindow::setCustomWidgetMaximumWidth(int width)
{
    Q_D(NXWindow);
    d->_appBar->setCustomWidgetMaximumWidth(width);
    Q_EMIT pCustomWidgetMaximumWidthChanged();
}

int NXWindow::getCustomWidgetMaximumWidth() const
{
    Q_D(const NXWindow);
    return d->_appBar->getCustomWidgetMaximumWidth();
}

void NXWindow::setIsCentralStackedWidgetTransparent(bool isTransparent)
{
    Q_D(NXWindow);
    d->_centerStackedWidget->setIsTransparent(isTransparent);
}

bool NXWindow::getIsCentralStackedWidgetTransparent() const
{
    Q_D(const NXWindow);
    return d->_centerStackedWidget->getIsTransparent();
}

void NXWindow::moveToCenter()
{
    if (isMaximized() || isFullScreen())
    {
        return;
    }
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    auto geometry = screen()->availableGeometry();
#else
    auto geometry = qApp->screenAt(this->geometry().center())->geometry();
#endif
    setGeometry((geometry.left() + geometry.right() - width()) / 2, (geometry.top() + geometry.bottom() - height()) / 2, width(), height());
}

void NXWindow::setCustomWidget(NXAppBarType::CustomArea customArea, QWidget* widget)
{
    Q_D(NXWindow);
    d->_appBar->setCustomWidget(customArea, widget);
    Q_EMIT customWidgetChanged();
}

void NXWindow::setIsNavigationBarEnable(bool isVisible)
{
    Q_D(NXWindow);
    d->_isNavigationEnable = isVisible;
    d->_navigationBar->setVisible(isVisible);
    d->_centerLayout->setContentsMargins(isVisible ? d->_contentsMargins : 0, 0, 0, 0);
    d->_centerStackedWidget->setIsHasRadius(isVisible);
}

bool NXWindow::getIsNavigationBarEnable() const
{
    return d_ptr->_isNavigationEnable;
}

void NXWindow::setIsLeftButtonPressedToggleNavigation(bool isPressed)
{
    Q_D(NXWindow);
    d->_navigationBar->setIsLeftButtonPressedToggleNavigation(isPressed);
}

void NXWindow::setNavigationNodeDragAndDropEnable(bool isEnable)
{
    Q_D(NXWindow);
    d->_navigationBar->setNavigationNodeDragAndDropEnable(isEnabled);
}

void NXWindow::setUserInfoCardVisible(bool isVisible)
{
    Q_D(NXWindow);
    d->_navigationBar->setUserInfoCardVisible(isVisible);
}

void NXWindow::setUserInfoCardPixmap(QPixmap pix)
{
    Q_D(NXWindow);
    d->_navigationBar->setUserInfoCardPixmap(pix);
}

void NXWindow::setUserInfoCardTitle(const QString& title)
{
    Q_D(NXWindow);
    d->_navigationBar->setUserInfoCardTitle(title);
}

void NXWindow::setUserInfoCardSubTitle(const QString& subTitle)
{
    Q_D(NXWindow);
    d->_navigationBar->setUserInfoCardSubTitle(subTitle);
}

NXNavigationType::NodeOperateReturnType NXWindow::setNodeTitle(const QString& nodeTitle, const QString& nodeKey)
{
    Q_D(NXWindow);
    return d->_navigationBar->setNodeTitle(nodeTitle, nodeKey);
}

NXNavigationType::NodeOperateReturnType NXWindow::navigationPageNodeSwitch(const QString& targetPageKey)
{
    Q_D(NXWindow);
    return d->_navigationBar->navigationPageNodeSwitch(targetPageKey);
}

void NXWindow::setNavigationPageOpenPolicy(std::function<void(const QString&/*nodeKey*/)>&& openNavigationPageFunc)
{
    Q_D(NXWindow);
    d->_navigationBar->setNavigationPageOpenPolicy(std::move(openNavigationPageFunc));
}

NodeOperateReturnTypeWithKey NXWindow::addExpanderNode(const QString& expanderTitle, NXIconType::IconName awesome) const
{
    Q_D(const NXWindow);
    return d->_navigationBar->addExpanderNode(expanderTitle, awesome);
}

NodeOperateReturnTypeWithKey NXWindow::addExpanderNode(const QString& expanderTitle, const QString& targetExpanderKey, NXIconType::IconName awesome) const
{
    Q_D(const NXWindow);
    return d->_navigationBar->addExpanderNode(expanderTitle, targetExpanderKey, awesome);
}

NodeOperateReturnTypeWithKey NXWindow::addPageNode(const QString& pageTitle, QWidget* page, NXIconType::IconName awesome) const
{
    Q_D(const NXWindow);
    return d->_navigationBar->addPageNode(pageTitle, page, awesome);
}

NodeOperateReturnTypeWithKey NXWindow::addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, NXIconType::IconName awesome) const
{
    Q_D(const NXWindow);
    return d->_navigationBar->addPageNode(pageTitle, page, targetExpanderKey, awesome);
}

NodeOperateReturnTypeWithKey NXWindow::addPageNode(const QString& pageTitle, QWidget* page, int keyPoints, NXIconType::IconName awesome) const
{
    Q_D(const NXWindow);
    return d->_navigationBar->addPageNode(pageTitle, page, keyPoints, awesome);
}

NodeOperateReturnTypeWithKey NXWindow::addPageNode(const QString& pageTitle, QWidget* page, const QString& targetExpanderKey, int keyPoints, NXIconType::IconName awesome) const
{
    Q_D(const NXWindow);
    return d->_navigationBar->addPageNode(pageTitle, page, targetExpanderKey, keyPoints, awesome);
}

NodeOperateReturnTypeWithKey NXWindow::addFooterNode(const QString& footerTitle, int keyPoints, NXIconType::IconName awesome) const
{
    Q_D(const NXWindow);
    return d->_navigationBar->addFooterNode(footerTitle, nullptr, keyPoints, awesome);
}

NodeOperateReturnTypeWithKey NXWindow::addFooterNode(const QString& footerTitle, QWidget* page, int keyPoints, NXIconType::IconName awesome) const
{
    Q_D(const NXWindow);
    return d->_navigationBar->addFooterNode(footerTitle, page, keyPoints, awesome);
}

bool NXWindow::getNavigationNodeIsExpanded(const QString& expanderKey) const
{
    Q_D(const NXWindow);
    return d->_navigationBar->getNavigationNodeIsExpanded(expanderKey);
}

void NXWindow::expandNavigationNode(const QString& expanderKey)
{
    Q_D(NXWindow);
    d->_navigationBar->expandNavigationNode(expanderKey);
}

void NXWindow::collpaseNavigationNode(const QString& expanderKey)
{
    Q_D(NXWindow);
    d->_navigationBar->collpaseNavigationNode(expanderKey);
}


void NXWindow::removeNavigationNode(const QString& nodeKey) const
{
    Q_D(const NXWindow);
    d->_navigationBar->removeNavigationNode(nodeKey);
}

void NXWindow::setNodeKeyPoints(const QString& nodeKey, int keyPoints)
{
    Q_D(NXWindow);
    d->_navigationBar->setNodeKeyPoints(nodeKey, keyPoints);
}

int NXWindow::getNodeKeyPoints(const QString& nodeKey) const
{
    Q_D(const NXWindow);
    return d->_navigationBar->getNodeKeyPoints(nodeKey);
}

std::tuple<NXNavigationType::NavigationNodeType, QString, QWidget*> NXWindow::currentVisibleWidget() const
{
    Q_D(const NXWindow);
    return d->_currentVisibleWidget;
}

void NXWindow::navigation(const QString& pageKey)
{
    Q_D(NXWindow);
    d->_navigationBar->navigation(pageKey);
}

void NXWindow::setWindowButtonFlag(NXAppBarType::ButtonType buttonFlag, bool isEnable)
{
    Q_D(NXWindow);
    d->_appBar->setWindowButtonFlag(buttonFlag, isEnable);
}

void NXWindow::setWindowButtonFlags(NXAppBarType::ButtonFlags buttonFlags)
{
    Q_D(NXWindow);
    d->_appBar->setWindowButtonFlags(buttonFlags);
}

NXAppBarType::ButtonFlags NXWindow::getWindowButtonFlags() const
{
    return d_ptr->_appBar->getWindowButtonFlags();
}

void NXWindow::closeWindow()
{
    Q_D(NXWindow);
    d->_isWindowClosing = true;
    d->_appBar->closeWindow();
}

bool NXWindow::eventFilter(QObject* watched, QEvent* event)
{
    Q_D(NXWindow);
    switch (event->type())
    {
    case QEvent::Resize:
    {
        d->_doNavigationDisplayModeChange();
        break;
    }
    default:
    {
        break;
    }
    }
    return QMainWindow::eventFilter(watched, event);
}

QMenu* NXWindow::createPopupMenu()
{
    NXMenu* menu = nullptr;
    QList<QDockWidget*> dockwidgets = findChildren<QDockWidget*>();
    if (dockwidgets.size())
    {
        menu = new NXMenu(this);
        for (int i = 0; i < dockwidgets.size(); ++i)
        {
            QDockWidget* dockWidget = dockwidgets.at(i);
            if (dockWidget->parentWidget() == this)
            {
                menu->addAction(dockwidgets.at(i)->toggleViewAction());
            }
        }
        menu->addSeparator();
    }

    QList<QToolBar*> toolbars = findChildren<QToolBar*>();
    if (toolbars.size())
    {
        if (!menu)
        {
            menu = new NXMenu(this);
        }
        for (int i = 0; i < toolbars.size(); ++i)
        {
            QToolBar* toolBar = toolbars.at(i);
            if (toolBar->parentWidget() == this)
            {
                menu->addAction(toolbars.at(i)->toggleViewAction());
            }
        }
    }
    if (menu)
    {
        menu->setMenuItemHeight(28);
    }
    return menu;
}
