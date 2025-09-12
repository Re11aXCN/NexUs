#include "NXWindow.h"

#include "NXApplication.h"
#include "NXEventBus.h"
#include "NXMenu.h"
#include "NXNavigationBar.h"
#include "NXNavigationRouter.h"
#include "NXTheme.h"
#include "NXCentralStackedWidget.h"
#include "DeveloperComponents/NXWindowStyle.h"
#include "private/NXAppBarPrivate.h"
#include "private/NXNavigationBarPrivate.h"
#include "private/NXWindowPrivate.h"
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
Q_PROPERTY_CREATE_Q_CPP(NXWindow, int, ThemeChangeTime)
Q_PROPERTY_CREATE_Q_CPP(NXWindow, NXNavigationType::NavigationDisplayMode, NavigationBarDisplayMode)
Q_PROPERTY_CREATE_Q_CPP(NXWindow, NXWindowType::StackSwitchMode, StackSwitchMode)
Q_TAKEOVER_NATIVEEVENT_CPP(NXWindow, d_func()->_appBar);
NXWindow::NXWindow(QWidget* parent)
    : QMainWindow{parent}, d_ptr(new NXWindowPrivate())
{
    Q_D(NXWindow);
    d->q_ptr = this;
    d->_pStackSwitchMode = NXWindowType::StackSwitchMode::Popup;
    setProperty("NXBaseClassName", "NXWindow");
    resize(1020, 680); // 默认宽高

    d->_pThemeChangeTime = 700;
    d->_pNavigationBarDisplayMode = NXNavigationType::NavigationDisplayMode::Auto;
    QObject::connect(this, &NXWindow::pNavigationBarDisplayModeChanged, d, &NXWindowPrivate::onDisplayModeChanged);

    // 自定义AppBar
    d->_appBar = new NXAppBar(this);
    QObject::connect(d->_appBar, &NXAppBar::routeBackButtonClicked, this, []() {
        NXNavigationRouter::getInstance()->navigationRouteBack();
    });
    QObject::connect(d->_appBar, &NXAppBar::closeButtonClicked, this, &NXWindow::closeButtonClicked);
    // 导航栏
    d->_navigationBar = new NXNavigationBar(this);
    // 返回按钮状态变更
    QObject::connect(NXNavigationRouter::getInstance(), &NXNavigationRouter::navigationRouterStateChanged, this, [d](bool isEnable) {
        d->_appBar->setRouteBackButtonEnable(isEnable);
    });

    // 转发用户卡片点击信号
    QObject::connect(d->_navigationBar, &NXNavigationBar::userInfoCardClicked, this, &NXWindow::userInfoCardClicked);
    // 转发点击信号 跳转处理
    QObject::connect(d->_navigationBar, &NXNavigationBar::navigationNodeClicked, d, &NXWindowPrivate::onNavigationNodeClicked);
    // 新增窗口
    QObject::connect(d->_navigationBar, &NXNavigationBar::navigationNodeAdded, d, &NXWindowPrivate::onNavigationNodeAdded);
    // 移除窗口
    QObject::connect(d->_navigationBar, &NXNavigationBar::navigationNodeRemoved, d, &NXWindowPrivate::onNavigationNodeRemoved);
    // 在新窗口打开
    QObject::connect(d->_navigationBar, &NXNavigationBar::pageOpenInNewWindow, this, &NXWindow::pageOpenInNewWindow);

    // 导航中心堆栈窗口
    d->_navigationCenterStackedWidget = new NXCentralStackedWidget(this);
    d->_navigationCenterStackedWidget->setContentsMargins(0, 0, 0, 0);
    QWidget* navigationCentralWidget = new QWidget(this);
    navigationCentralWidget->setObjectName("NXWindowNavigationCentralWidget");
    navigationCentralWidget->setStyleSheet("#NXWindowNavigationCentralWidget{background-color:transparent;}");
    navigationCentralWidget->installEventFilter(this);
    d->_centerLayout = new QHBoxLayout(navigationCentralWidget);
    d->_centerLayout->setSpacing(0);
    d->_centerLayout->addWidget(d->_navigationBar);
    d->_centerLayout->addWidget(d->_navigationCenterStackedWidget);
    d->_centerLayout->setContentsMargins(d->_contentsMargins, 0, 0, 0);

    // 事件总线
    d->_focusEvent = new NXEvent("WMWindowClicked", "onWMWindowClickedEvent", d);
    d->_focusEvent->registerAndInit();

    // 展开导航栏
    QObject::connect(d->_appBar, &NXAppBar::navigationButtonClicked, d, &NXWindowPrivate::onNavigationButtonClicked);

    // 主题变更动画
    d->_themeMode = nxTheme->getThemeMode();
    QObject::connect(nxTheme, &NXTheme::themeModeChanged, d, &NXWindowPrivate::onThemeModeChanged);
    QObject::connect(d->_appBar, &NXAppBar::themeChangeButtonClicked, d, &NXWindowPrivate::onThemeReadyChange);
    d->_isInitFinished = true;

    // 中心堆栈窗口
    d->_centerStackedWidget = new NXCentralStackedWidget(this);
    d->_centerStackedWidget->setIsTransparent(true);
    d->_centerStackedWidget->addWidget(navigationCentralWidget);
    setCentralWidget(d->_centerStackedWidget);
    setObjectName("NXWindow");
    setStyleSheet("#NXWindow{background-color:transparent;}");
    setStyle(new NXWindowStyle(style()));

    //延时渲染
    QTimer::singleShot(1, this, [=] {
        QPalette palette = this->palette();
        palette.setBrush(QPalette::Window, NXThemeColor(d->_themeMode, WindowBase));
        this->setPalette(palette);
    });
    nxApp->syncWindowDisplayMode(this);
    QObject::connect(nxApp, &NXApplication::pWindowDisplayModeChanged, this, [=]() {
        d->onThemeModeChanged(d->_themeMode);
    });
}

NXWindow::~NXWindow()
{
    nxApp->syncWindowDisplayMode(this, false);
    delete this->style();
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

void NXWindow::setCustomWidget(NXAppBarType::CustomArea customArea, QWidget* widget)
{
    Q_D(NXWindow);
    d->_appBar->setCustomWidget(customArea, widget);
    Q_EMIT customWidgetChanged();
}

QWidget* NXWindow::getCustomWidget() const
{
    Q_D(const NXWindow);
    return d->_appBar->getCustomWidget();
}

void NXWindow::setCustomMenu(QMenu* customMenu)
{
    Q_D(NXWindow);
    d->_appBar->setCustomMenu(customMenu);
    Q_EMIT customMenuChanged();
}

QMenu* NXWindow::getCustomMenu() const
{
    Q_D(const NXWindow);
    return d->_appBar->getCustomMenu();
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
    d->_navigationCenterStackedWidget->setIsTransparent(isTransparent);
}

bool NXWindow::getIsCentralStackedWidgetTransparent() const
{
    Q_D(const NXWindow);
    return d->_navigationCenterStackedWidget->getIsTransparent();
}

void NXWindow::setIsAllowPageOpenInNewWindow(bool isAllowPageOpenInNewWindow)
{
    Q_D(NXWindow);
    d->_navigationBar->setIsAllowPageOpenInNewWindow(isAllowPageOpenInNewWindow);
    Q_EMIT pIsAllowPageOpenInNewWindowChanged();
}

bool NXWindow::getIsAllowPageOpenInNewWindow() const
{
    Q_D(const NXWindow);
    return d->_navigationBar->getIsAllowPageOpenInNewWindow();
}

void NXWindow::setNavigationBarWidth(int navigationBarWidth)
{
    Q_D(NXWindow);
    d->_navigationBar->setNavigationBarWidth(navigationBarWidth);
    Q_EMIT pNavigationBarWidthChanged();
}

int NXWindow::getNavigationBarWidth() const
{
    Q_D(const NXWindow);
    return d->_navigationBar->getNavigationBarWidth();
}

void NXWindow::setCurrentStackIndex(int currentStackIndex)
{
    Q_D(NXWindow);
    if (currentStackIndex >= d->_centerStackedWidget->count() || currentStackIndex < 0 || currentStackIndex == d->_centralStackTargetIndex) {
        return;
    }
    d->_centralStackTargetIndex = currentStackIndex;
    QVariantMap routeData;
    routeData.insert("NXCentralStackIndex", d->_centerStackedWidget->currentIndex());
    NXNavigationRouter::getInstance()->navigationRoute(d, "onNavigationRouteBack", routeData);
    d->_centerStackedWidget->doWindowStackSwitch(d->_pStackSwitchMode, currentStackIndex, false);
    Q_EMIT pCurrentStackIndexChanged();
}

int NXWindow::getCurrentStackIndex() const
{
    Q_D(const NXWindow);
    return d->_centerStackedWidget->currentIndex();
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

void NXWindow::setIsNavigationBarEnable(bool isVisible)
{
    Q_D(NXWindow);
    d->_isNavigationEnable = isVisible;
    d->_navigationBar->setVisible(isVisible);
    d->_centerLayout->setContentsMargins(isVisible ? d->_contentsMargins : 0, 0, 0, 0);
    d->_navigationCenterStackedWidget->setIsHasRadius(isVisible);
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
    d->_navigationBar->setNavigationNodeDragAndDropEnable(isEnable);
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

void NXWindow::addCentralWidget(QWidget* centralWidget)
{
    Q_D(NXWindow);
    if (!centralWidget)
    {
        return;
    }
    d->_centerStackedWidget->addWidget(centralWidget);
}

QWidget* NXWindow::getCentralWidget(int index) const
{
    Q_D(const NXWindow);
    if (index >= d->_centerStackedWidget->count() || index < 1)
    {
        return nullptr;
    }
    return d->_centerStackedWidget->widget(index);
}

QString NXWindow::getNavigationRootKey() const
{
	Q_D(const NXWindow);
	return d->_navigationBar->getNavigationRootKey();
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

int NXWindow::getPageOpenInNewWindowCount(const QString& nodeKey) const
{
    Q_D(const NXWindow);
    return d->_navigationBar->getPageOpenInNewWindowCount(nodeKey);
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

void NXWindow::navigation(const QString& pageKey)
{
    Q_D(NXWindow);
    d->_navigationBar->navigation(pageKey);
}

int NXWindow::getCurrentNavigationIndex() const
{
    Q_D(const NXWindow);
    return d->_navigationCenterStackedWidget->currentIndex();
}

QString NXWindow::getCurrentNavigationPageKey() const
{
    Q_D(const NXWindow);
    return d->_navigationCenterStackedWidget->currentWidget()->property("NXPageKey").toString();
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
