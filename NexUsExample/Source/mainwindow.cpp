#include "mainwindow.h"

#include <QDebug>
#include <QGraphicsView>
#include <QHBoxLayout>

#include "NXContentDialog.h"
#include "NXDockWidget.h"
#include "NXEventBus.h"
#include "NXLog.h"
#include "NXMenu.h"
#include "NXMenuBar.h"
#include "NXProgressBar.h"
#include "NXStatusBar.h"
#include "NXText.h"
#include "NXTheme.h"
#include "NXToolBar.h"
#include "NXToolButton.h"
#include "T_About.h"
#include "T_BaseComponents.h"
#include "T_Card.h"
#include "T_Graphics.h"
#include "T_ListView.h"
#include "T_Setting.h"
#include "T_TableView.h"
#include "T_TreeView.h"
#ifdef Q_OS_WIN
#include "ExamplePage/T_NXScreen.h"
#endif

#include "ExamplePage/T_Home.h"
#include "ExamplePage/T_Icon.h"
#include "ExamplePage/T_LogWidget.h"
#include "ExamplePage/T_Navigation.h"
#include "ExamplePage/T_Popup.h"
#include "ExamplePage/T_UpdateWidget.h"

MainWindow::MainWindow(QWidget* parent)
    : NXWindow(parent)
{
    initWindow();

    //额外布局
    initEdgeLayout();

    //中心窗口
    initContent();

    // 拦截默认关闭事件
    _closeDialog = new NXContentDialog(this);
    QObject::connect(_closeDialog, &NXContentDialog::leftButtonClicked, &NXContentDialog::close);
    QObject::connect(_closeDialog, &NXContentDialog::rightButtonClicked, this, &MainWindow::closeWindow);
    QObject::connect(_closeDialog, &NXContentDialog::middleButtonClicked, this, [=]() {
        _closeDialog->close();
        showMinimized();
    });
    this->setIsDefaultClosed(false);
    QObject::connect(this, &MainWindow::closeButtonClicked, this, [=]() {
        _closeDialog->exec();
    });

    //移动到中心
    moveToCenter();
}

MainWindow::~MainWindow()
{
    delete this->_aboutPage;
}

void MainWindow::initWindow()
{
    // setIsEnableMica(true);
    // setIsCentralStackedWidgetTransparent(true);
    setWindowIcon(QIcon(":/Resource/Image/Cirno.jpg"));
    resize(1200, 740);
    // NXLog::getInstance()->initMessageLog(true);
    // nxTheme->setThemeMode(NXThemeType::Dark);
    // setIsNavigationBarEnable(false);
    // setNavigationBarDisplayMode(NXNavigationType::Compact);
    // setWindowButtonFlag(NXAppBarType::MinimizeButtonHint, false);
    setUserInfoCardPixmap(QPixmap(":/Resource/Image/Cirno.jpg"));
    setUserInfoCardTitle("NX Tool");
    setUserInfoCardSubTitle("Liniyous@gmail.com");
    setWindowTitle("NXWidgetTool");
    // setIsStayTop(true);
    // setUserInfoCardVisible(false);
}

void MainWindow::initEdgeLayout()
{
    //菜单栏
    NXMenuBar* menuBar = new NXMenuBar(this);
    menuBar->setFixedHeight(30);
    QWidget* customWidget = new QWidget(this);
    QVBoxLayout* customLayout = new QVBoxLayout(customWidget);
    customLayout->setContentsMargins(0, 0, 0, 0);
    customLayout->addWidget(menuBar);
    customLayout->addStretch();
    // this->setMenuBar(menuBar);
    this->setCustomWidget(NXAppBarType::MiddleArea, customWidget);
    this->setCustomWidgetMaximumWidth(500);

    menuBar->addNXIconAction(NXIconType::AtomSimple, "动作菜单");
    NXMenu* iconMenu = menuBar->addMenu(NXIconType::Aperture, "图标菜单");
    iconMenu->setMenuItemHeight(27);
    iconMenu->addNXIconAction(NXIconType::BoxCheck, "排序方式", QKeySequence::SelectAll);
    iconMenu->addNXIconAction(NXIconType::Copy, "复制");
    iconMenu->addNXIconAction(NXIconType::MagnifyingGlassPlus, "显示设置");
    iconMenu->addSeparator();
    iconMenu->addNXIconAction(NXIconType::ArrowRotateRight, "刷新");
    iconMenu->addNXIconAction(NXIconType::ArrowRotateLeft, "撤销");
    menuBar->addSeparator();
    NXMenu* shortCutMenu = new NXMenu("快捷菜单(&A)", this);
    shortCutMenu->setMenuItemHeight(27);
    shortCutMenu->addNXIconAction(NXIconType::BoxCheck, "排序方式", QKeySequence::Find);
    shortCutMenu->addNXIconAction(NXIconType::Copy, "复制");
    shortCutMenu->addNXIconAction(NXIconType::MagnifyingGlassPlus, "显示设置");
    shortCutMenu->addSeparator();
    shortCutMenu->addNXIconAction(NXIconType::ArrowRotateRight, "刷新");
    shortCutMenu->addNXIconAction(NXIconType::ArrowRotateLeft, "撤销");
    menuBar->addMenu(shortCutMenu);

    menuBar->addMenu("样例菜单(&B)")->addNXIconAction(NXIconType::ArrowRotateRight, "样例选项");
    menuBar->addMenu("样例菜单(&C)")->addNXIconAction(NXIconType::ArrowRotateRight, "样例选项");
    menuBar->addMenu("样例菜单(&E)")->addNXIconAction(NXIconType::ArrowRotateRight, "样例选项");
    menuBar->addMenu("样例菜单(&F)")->addNXIconAction(NXIconType::ArrowRotateRight, "样例选项");
    menuBar->addMenu("样例菜单(&G)")->addNXIconAction(NXIconType::ArrowRotateRight, "样例选项");

    //工具栏
    NXToolBar* toolBar = new NXToolBar("工具栏", this);
    toolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    toolBar->setToolBarSpacing(3);
    toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    toolBar->setIconSize(QSize(25, 25));
    // toolBar->setFloatable(false);
    // toolBar->setMovable(false);
    NXToolButton* toolButton1 = new NXToolButton(this);
    toolButton1->setNXIcon(NXIconType::BadgeCheck);
    toolBar->addWidget(toolButton1);
    NXToolButton* toolButton2 = new NXToolButton(this);
    toolButton2->setNXIcon(NXIconType::ChartUser);
    toolBar->addWidget(toolButton2);
    toolBar->addSeparator();
    NXToolButton* toolButton3 = new NXToolButton(this);
    toolButton3->setNXIcon(NXIconType::Bluetooth);
    toolButton3->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolButton3->setText("Bluetooth");
    toolBar->addWidget(toolButton3);
    NXToolButton* toolButton4 = new NXToolButton(this);
    toolButton4->setNXIcon(NXIconType::BringFront);
    toolBar->addWidget(toolButton4);
    toolBar->addSeparator();
    NXToolButton* toolButton5 = new NXToolButton(this);
    toolButton5->setNXIcon(NXIconType::ChartSimple);
    toolBar->addWidget(toolButton5);
    NXToolButton* toolButton6 = new NXToolButton(this);
    toolButton6->setNXIcon(NXIconType::FaceClouds);
    toolBar->addWidget(toolButton6);
    NXToolButton* toolButton8 = new NXToolButton(this);
    toolButton8->setNXIcon(NXIconType::Aperture);
    toolBar->addWidget(toolButton8);
    NXToolButton* toolButton9 = new NXToolButton(this);
    toolButton9->setNXIcon(NXIconType::ChartMixed);
    toolBar->addWidget(toolButton9);
    NXToolButton* toolButton10 = new NXToolButton(this);
    toolButton10->setNXIcon(NXIconType::Coins);
    toolBar->addWidget(toolButton10);
    NXToolButton* toolButton11 = new NXToolButton(this);
    toolButton11->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolButton11->setNXIcon(NXIconType::AlarmPlus);
    toolButton11->setText("AlarmPlus");
    toolBar->addWidget(toolButton11);
    NXToolButton* toolButton12 = new NXToolButton(this);
    toolButton12->setNXIcon(NXIconType::Crown);
    toolBar->addWidget(toolButton12);
    QAction* test = new QAction(this);
    test->setMenu(new QMenu(this));

    NXProgressBar* progressBar = new NXProgressBar(this);
    progressBar->setMinimum(0);
    progressBar->setMaximum(0);
    progressBar->setFixedWidth(350);
    toolBar->addWidget(progressBar);

    this->addToolBar(Qt::TopToolBarArea, toolBar);

    //停靠窗口
    NXDockWidget* logDockWidget = new NXDockWidget("日志信息", this);
    logDockWidget->setWidget(new T_LogWidget(this));
    this->addDockWidget(Qt::RightDockWidgetArea, logDockWidget);
    resizeDocks({logDockWidget}, {200}, Qt::Horizontal);

    NXDockWidget* updateDockWidget = new NXDockWidget("更新内容", this);
    updateDockWidget->setWidget(new T_UpdateWidget(this));
    this->addDockWidget(Qt::RightDockWidgetArea, updateDockWidget);
    resizeDocks({updateDockWidget}, {200}, Qt::Horizontal);

    //状态栏
    NXStatusBar* statusBar = new NXStatusBar(this);
    NXText* statusText = new NXText("初始化成功！", this);
    statusText->setTextPixelSize(14);
    statusBar->addWidget(statusText);
    this->setStatusBar(statusBar);
}

void MainWindow::initContent()
{
    _homePage = new T_Home(this);
//#ifdef Q_OS_WIN
//    _elaScreenPage = new T_NXScreen(this);
//#endif
    _iconPage = new T_Icon(this);
    _baseComponentsPage = new T_BaseComponents(this);
    _graphicsPage = new T_Graphics(this);
    _navigationPage = new T_Navigation(this);
    _popupPage = new T_Popup(this);
    _cardPage = new T_Card(this);
    _listViewPage = new T_ListView(this);
    _tableViewPage = new T_TableView(this);
    _treeViewPage = new T_TreeView(this);
    _settingPage = new T_Setting(this);

    QString testKey_1;
    QString testKey_2;
    addPageNode("HOME", _homePage, NXIconType::House);
//#ifdef Q_OS_WIN
//    addExpanderNode("NXDxgi", _elaDxgiKey, NXIconType::TvMusic);
//    addPageNode("NXScreen", _elaScreenPage, _elaDxgiKey, 3, NXIconType::ObjectGroup);
//#endif
    // navigation(elaScreenWidget->property("NXPageKey").toString());
    addPageNode("NXBaseComponents", _baseComponentsPage, NXIconType::CabinetFiling);

    addExpanderNode("NXView", _viewKey, NXIconType::CameraViewfinder);
    addPageNode("NXListView", _listViewPage, _viewKey, 9, NXIconType::List);
    addPageNode("NXTableView", _tableViewPage, _viewKey, NXIconType::Table);
    addPageNode("NXTreeView", _treeViewPage, _viewKey, NXIconType::ListTree);
    expandNavigationNode(_viewKey);

    addPageNode("NXGraphics", _graphicsPage, 9, NXIconType::Paintbrush);
    addPageNode("NXCard", _cardPage, NXIconType::Cards);
    addPageNode("NXNavigation", _navigationPage, NXIconType::LocationArrow);
    addPageNode("NXPopup", _popupPage, NXIconType::Envelope);
    addPageNode("NXIcon", _iconPage, 99, NXIconType::FontCase);
    addExpanderNode("TEST4", testKey_2, NXIconType::Acorn);
    testKey_1 = addExpanderNode("TEST5", testKey_2, NXIconType::Acorn).second;
    addPageNode("Third Level", new QWidget(this), testKey_1, NXIconType::Acorn);
    addExpanderNode("TEST6", testKey_2, NXIconType::Acorn);
    addExpanderNode("TEST7", testKey_2, NXIconType::Acorn);
    addExpanderNode("TEST8", testKey_2, NXIconType::Acorn);
    addExpanderNode("TEST9", testKey_2, NXIconType::Acorn);
    addExpanderNode("TEST10", testKey_2, NXIconType::Acorn);
    addExpanderNode("TEST11", testKey_2, NXIconType::Acorn);
    addExpanderNode("TEST12", testKey_2, NXIconType::Acorn);
    addExpanderNode("TEST13", testKey_2, NXIconType::Acorn);
    addExpanderNode("TEST14", testKey_2, NXIconType::Acorn);
    addExpanderNode("TEST15", NXIconType::Acorn);
    addExpanderNode("TEST16", NXIconType::Acorn);
    addExpanderNode("TEST17", NXIconType::Acorn);

    _aboutKey = addFooterNode("About", nullptr, 0, NXIconType::User).second;
    _aboutPage = new T_About();

    _aboutPage->hide();
    QObject::connect(this, &NXWindow::navigationNodeClicked, this, [=](NXNavigationType::NavigationNodeType nodeType, QString nodeKey) {
        if (_aboutKey == nodeKey)
        {
            _aboutPage->setFixedSize(400, 400);
            _aboutPage->moveToCenter();
            _aboutPage->show();
        }
    });
    _settingKey = addFooterNode("Setting", _settingPage, 0, NXIconType::GearComplex).second;
    QObject::connect(this, &MainWindow::userInfoCardClicked, this, [=]() {
        this->navigation(_homePage->property("NXPageKey").toString());
    });
//#ifdef Q_OS_WIN
//    QObject::connect(_homePage, &T_Home::elaScreenNavigation, this, [=]() {
//        this->navigation(_elaScreenPage->property("NXPageKey").toString());
//    });
//#endif
    QObject::connect(_homePage, &T_Home::elaBaseComponentNavigation, this, [=]() {
        this->navigation(_baseComponentsPage->property("NXPageKey").toString());
    });
    QObject::connect(_homePage, &T_Home::elaSceneNavigation, this, [=]() {
        this->navigation(_graphicsPage->property("NXPageKey").toString());
    });
    QObject::connect(_homePage, &T_Home::elaIconNavigation, this, [=]() {
        this->navigation(_iconPage->property("NXPageKey").toString());
    });
    QObject::connect(_homePage, &T_Home::elaCardNavigation, this, [=]() {
        this->navigation(_cardPage->property("NXPageKey").toString());
    });
    //qDebug() << "已注册的事件列表" << NXEventBus::getInstance()->getRegisteredEventsName();
}
