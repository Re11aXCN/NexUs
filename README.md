# NexUs
**ElaWidgetTools QT开源库的修改封装部分代码**，修改记录见[doc/TrackRecord.md](./doc/TrackRecord.md)

<u>**版权归 ElaWidgetTools  作者[Liniyous](https://github.com/Liniyous)所有**</u>

仓库链接：<u>https://github.com/Liniyous/ElaWidgetTools/commits/main/</u>



# 使用教程

## 安装插件(如果是使用Sln)

1. 打开VS2022 -> 扩展 -> 管理扩展 -> 搜索Qt，安装Qt Visual Studio Tools

2. 安装完成打开，扩展 -> Qt VS Tools -> Qt Versions
3. 点击Add按钮，输入Name和Location（示例：Name输入`6.6.2_msvc2019_64`，Location输入`E:\Qt\6.6.2\msvc2019_64`）

完成上述操作 VS2022应该能够识别Qt路径并能够识别加载对应的HEADERS资源



## 构建选择(Sln、CMake、PreMake(待完善))/项目输出说明

### 动态库、静态库、可执行程序、调试文件

进入Build文件夹，运行对应平台的脚本，即可会自动将构建项目配置拷贝一份到对应位置，然后使用IDE打开即可编译运行

项目 <font color="red">动态库、静态库、可执行程序、调试文件</font>  均输出到 <font color="red">Bin\ <项目名字>_ <构建目标> _ <平台架构></font>

示例路径：`NexUs\Bin\NexUs_RelWithDebInfo_x64`



### 中间文件

#### 1、obj文件

* **Sln**

Sln构建会在 `Bin目录下` 生成 <font color="red">Intermediate文件夹</font>，然后生成 `<项目名字>_ <构建目标> _ <平台架构>` 文件夹（和上述一致）， 生成 <font color="red">obj文件</font> 均放到此处

示例路径：`NexUs\Bin\Intermediate\NexUs_Release_x64`

* **CMake**

CMake构建的中间文件 未移动/拷贝到 Bin\Intermediate 下，在标准的out文件夹下

示例路径：

mocs_compilation.cpp.obj处在路径：`NexUs\out\build\debug\NexUs\CMakeFiles\NexUs.dir\NexUs_autogen`

其他Q_OBJECT等需要生成MOC的对象路径：`NexUs\out\build\debug\NexUs\CMakeFiles\NexUs.dir\Source`



#### 2、moc、rcc、uic文件

* **Sln**

Sln构建会在每个具体的项目下生成 <font color="red">GeneratedFiles</font> 文件夹，其中rcc、uic的生成文件对于构建模式不印象其内容，moc文件则会被放到生成的构建文件夹当中，因为不同构建模式可能存在代码优化

示例结构：

> * NexUs\NexUs\GeneratedFiles
>   * qrc_NexUs.cpp
>   * RelWithDebInfo
>     * moc_NXAppBar.cpp
>     * ...
>   * Debug
>     * moc_NXAppBar.cpp
>     * ...
>
> * NexUs\NexUsExample\GeneratedFiles
>
>   * qrc_NexUsExample.cpp
>   * ui_NexUsExample.h
>
>   * RelWithDebInfo
>     * moc_T_About.cpp
>     * ...
>
>   * Debug
>     * moc_T_About.cpp
>     * ...



* **CMake**

CMake生成moc_XXX.cpp文件路径未进行移动，没有GeneratedFiles文件夹，当然你可以进行配置CMakeLists.txt进行操作或者编写脚本移动，笔者这里就不实现了

示例路径：

`NexUs\out\build\debug\NexUs\NexUs_autogen\哈希值`，哈希值有

- 5OPOMGIKHL：库使用基本文件，MOC文件如：moc_NXCheckBox.cpp

* EWIEGA46WW：qrc资源，qrc_NexUs.cpp在目录下
* MDIJTLMGDY：DeveloperComponents文件夹下内容，MOC文件如：moc_NXBaseListView.cpp
* S4JZKUS5AE：private文件夹下内容，MOC文件如：moc_NXDoubleSpinBoxPrivate.cpp

`NexUs\out\build\debug\NexUs\NexUs_autogen\mocs_compilation.cpp`（包含了所有moc文件路径）

NexUsExample的路径类似，不再说明



### 构建速度/输出文件大小

* **Sln**： 编译较慢	在Debug、Release模式下生成的文件比CMake生成稍大一点点，RebWithDebInfo生成则会小一些
* **CMake**：编译很快 Debug、RebWithDebInfo会拷贝不必要的文件类型`到 Bin 中`，不必要文件类型有`exp、idb、ilk`（Windows下，其他平台没测试）可以删除
* **PreMake**：可以生成Sln、Makefile、Xcode项目，`当前的lua脚本还没有完全完善和测试`

> Windows (生成Visual Studio项目)：
>
> ```
> BUILD\premake5.exe vs2022
> ```
>
> Linux (生成Makefile)：
>
> ```
> ./BUILD/premake5 gmake2
> ```
>
> macOS (生成Xcode项目)：
>
> ```
> ./BUILD/premake5 xcode4
> ```
>
> 生成的项目文件将支持MSBuild编译（Windows）和GNU编译器（Linux/macOS），实现了完全的跨平台构建支持，当然你可以选择直接使用CMake。



## Example替换

同步Ela主仓库的Example之后需要进行以下替换进行适配已修改的代码，才能正确编译



ELA			替换	NX

Ela	 		替换	NX

T_Ela	 	替换	T_NX

eTheme   替换 	nxTheme

eApp		替换 	nxApp

":/include	替换	":/Resource





T_Popup.cpp

getCurrentColorRGB	替换为	getCurrent4ChannelColor





T_NXPacketIO.h/.cpp

需要加上前缀逻辑

T_NXPacketIO.h

#include "XIO/NXXIO_PacketRegistry.h"
#include "Util/UtCallbackHolder.h"

T_NXPacketIO.cpp

#include "XIO/NXXIO_Connection.h"
#include "XIO/NXXIO_Interface.h"
#include "XIO/NXXIO_PacketRegistry.h"

然后T_NXPacketIO.h新增header
#include <QPixmap>





mainwindow.h

_elaDxgiKey	替换为	_rootKey

mainwindow.cpp

构造函数新增初始化	_rootKey(getNavigationRootKey())

```cpp
void MainWindow::initContent()
{
    _homePage = new T_Home(this);
#ifdef Q_OS_WIN
    _elaScreenPage = new T_NXScreen(this);
#endif
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

    addPageNode("HOME", _homePage, NXIconType::House);
#ifdef Q_OS_WIN
    // 默认是root 添加一个expander节点
    NXNodeOperateResult dxgiResult = addExpanderNode("NXDxgi", NXIconType::TvMusic);
    addPageNode("NXScreen", _elaScreenPage, *dxgiResult, 3, NXIconType::ObjectGroup);
#endif
    // navigation(elaScreenWidget->property("NXPageKey").toString());
    addPageNode("NXBaseComponents", _baseComponentsPage, NXIconType::CabinetFiling);

    _viewKey = *addExpanderNode("NXView", _rootKey, NXIconType::CameraViewfinder);
    addPageNode("NXListView", _listViewPage, _viewKey, 9, NXIconType::List);
    addPageNode("NXTableView", _tableViewPage, _viewKey, NXIconType::Table);
    addPageNode("NXTreeView", _treeViewPage, _viewKey, NXIconType::ListTree);
    expandNavigationNode(_viewKey);

    addPageNode("NXGraphics", _graphicsPage, 9, NXIconType::Paintbrush);
    addPageNode("NXCard", _cardPage, NXIconType::Cards);
    addPageNode("NXNavigation", _navigationPage, NXIconType::LocationArrow);
    addPageNode("NXPopup", _popupPage, NXIconType::Envelope);
    addPageNode("NXIcon", _iconPage, 99, NXIconType::FontCase);

    auto testKey_1 = addExpanderNode("TEST_EXPAND_NODE1", _rootKey, NXIconType::Acorn);
    auto testKey_2 = addExpanderNode("TEST_EXPAND_NODE2", *testKey_1, NXIconType::Acorn);
    addPageNode("TEST_NODE3", new QWidget(this), *testKey_2, NXIconType::Acorn);
    for (int i = 0; i < 10; i++)
    {
        addExpanderNode(QString("TEST_EXPAND_NODE%1").arg(i + 4), *testKey_2, NXIconType::Acorn);
    }
    addExpanderNode("TEST_EXPAND_NODE14", _rootKey, NXIconType::Acorn);
    addExpanderNode("TEST_EXPAND_NODE5", _rootKey, NXIconType::Acorn);
    addExpanderNode("TEST_EXPAND_NODE16", _rootKey, NXIconType::Acorn);

    _aboutKey = *addFooterNode("About", nullptr, 0, NXIconType::User);
    _aboutPage = new T_About();

    _aboutPage->hide();
    connect(this, &NXWindow::navigationNodeClicked, this, [this](NXNavigationType::NavigationNodeType nodeType, QString nodeKey) {
        if (_aboutKey == nodeKey)
        {
            _aboutPage->moveToCenter();
            _aboutPage->show();
        }
        });
    _settingKey = *addFooterNode("Setting", _settingPage, 0, NXIconType::GearComplex);
    connect(this, &MainWindow::userInfoCardClicked, this, [=]() {
        this->navigation(_homePage->property("NXPageKey").toString());
        });
#ifdef Q_OS_WIN
    connect(_homePage, &T_Home::elaScreenNavigation, this, [=]() {
        this->navigation(_elaScreenPage->property("NXPageKey").toString());
        });
#endif
    connect(_homePage, &T_Home::elaBaseComponentNavigation, this, [=]() {
        this->navigation(_baseComponentsPage->property("NXPageKey").toString());
        });
    connect(_homePage, &T_Home::elaSceneNavigation, this, [=]() {
        this->navigation(_graphicsPage->property("NXPageKey").toString());
        });
    connect(_homePage, &T_Home::elaIconNavigation, this, [=]() {
        this->navigation(_iconPage->property("NXPageKey").toString());
        });
    connect(_homePage, &T_Home::elaCardNavigation, this, [=]() {
        this->navigation(_cardPage->property("NXPageKey").toString());
        });
    qDebug() << "已注册的事件列表" << NXEventBus::getInstance()->getRegisteredEventsName();
}
```

