# NexUs/MSVC sln
**ElaWidgetTools QT开源库的修改封装部分代码**
<u>https://github.com/Liniyous/ElaWidgetTools/commits/main/</u>



## # **Track Record 1**：	Commits on Mar 12, 2025

### 1. Modify Optize1

> 已修改过NXWidget、NXWindow、NXColorDialog、NXTableView等一些基本的UI控件，修改了NXDef.h、stdafx.h宏定义
>
> * NXColorDialog：支持Alpha
>
> * NXDef.h、stdafx.h（现在为NXProperty.h）：修改代码整体设计(NXProperty.h包含宏、NXDef.h包含枚举)、新增多个宏，修改导出枚举宏设计
>
>   注意事项：NXDef.h必须是MOC文件，NXProperty.h不是MOC文件，使用VS2022当修改NXDef.h文件的时候，VS2022会错误的修改文件定义为`<ClInclude Include="Source\include\NXDef.h" />`，你需要进行修改NexUs.vcproj保持为`<QtMoc Include="Source\include\NXDef.h" />`
>
> * NXWidget：新增设置自定义亮黑主题背景色，新增接口可以操作NXAppBar
>
> * NXWindow：新增接口可以自定义打开窗口逻辑，添加节点方法设计修改，navigationNodeRemoved信号添加

### 2. Modify Optize2

> 1. 修改QString为const QString&
>
> 2. 去除addFooterNode等导航添加的方法QString&，改用返回值传递ownerKey
> 3. NXNavigationNode新增自定义标题方法
> 4. NXNavigationView右键新增删除Node的Action，并且发送信号给->Bar->NXWindow
>
> 5. NXNavigationBar新增通过key切换Page显示——navigationPageNodeSwitch
>
> ​			`NXWindow一起同步修改`
>
> 6. NXMenu新增修改边角圆形，悬停Action是显示强调样式，新增寻找父菜单方法，添加参数设置子菜单和父菜单左上角同一水平
>
> 7. NXSlider新增对齐指定的标记Tick
>
> 8. NXToolButton新增Checkable为true是强调样式，优化ToolButtonTextUnderIcon时的显示位置
>    NXAppBar注释`WM_NCCALCSIZE`部分代码，因为窗口最大化会造成边缘空缺
>
> 9. NXScrollPage修改addCentralWidget参数新增可以选择开启垂直或者水平的滚动条

### 3. Modify Optize3

> NXContentDialog修改内部按钮信号槽，如果想要设置关闭使用时外部建立即可（NXKeyBinder.cpp被修改，因为使用了NXContentDialog）
>
> NXMessageButton修改，新增showMessage方法，外部可以调用，新增disconnect按钮clicked时弹出按钮方法，外部可以取消这个点击事件，然后自行建立
> NXWindow的navigationNodeClicked信号修改新增QWidget*参数

### 4.  Modify Optize4

> 1. NXNavigationModel新增拖拽方法支持拖拽功能
>
> 2. NXNavigationNode新增insertChildNode方法
>
> 3. NXNavigationStyle尝试内置拖拽指示器逻辑case  QStyle::PE_IndicatorItemViewItemDrop:
>
> 4. NXNavigationView新增IsLeftButtonPressedToggleNavigation选择pressed切换pagenode还是released、setNavigationNodeDragAndDropEnable方法设置支持拖拽——NXWindow同步方
>
>    新增自定义dropIndicatorPositionOverride方法（qt内置的不太好用）、拖拽事件的完善、paintEvent指示器的绘制
>
> 5. NXNavigationBar增加信号槽连接model的move信号完成节点交换重绘
>
>    新增setNavigationPageOpenPolicy自定义打开逻辑（可以使用内部自定义打开custom，也可以自定义方法打开PageNode的widget）——NXWindow同步方法
>
> 6. NXWindow新增navigationNodeToggled信号替换原来的navigationNodeClicked作用，navigationNodeClicked变为点击触发即使node没有page，navigationNodeToggled是进行page切换
>
>    新增_currentVisibleWidget获取当前stackedWidget和nodekey、nodetype

### 5. Modify Optize5

> 1. NXNavigationModel、NXNavigationView，drag、drop优化，交换位置符合人的视觉行为习惯，添加颜色提示

### 6. Modify Optize6

> 1. 修改stdafx.h的qt枚举导出宏，进行宏重载方便调用
> 2. NXTreeView新增设置NXIcon图标作为文字绘制，修改NXTreeViewStyle的文字绘制逻辑，绘制选择时的文本间距及文字颜色（如果采用Model将NXIconType作为QIcon返回NXIcon::getInstance()->getNXIcon间距太宽，图标颜色不好使用QStyle::State进行修改）
> 3. NXListViewStyle的视口绘制逻辑修复
> 4. NXDockWidget增加dockClosed、dockResized信号

### 7. Modify Optize7

> 1. 修复NavigationNodeClicked/Removed的错误逻辑
> 1. NXSlider添加的仅按下拖到

------



## # **Track Record 2**：	Commits on Jun 30, 2025

### 1. Modify Optize1

> 1. 去除先前多个控件不必要的代码，如NXWidget、NXSlider等
> 2. 优化大部分逻辑
> 3. 引入NXPacketIO、magic_enum
>

## # **Track Record 3**：	Commits on Jul 8, 2025

### 1. Modify Optize1

> 1. 引入NXShadowGraphicsEffect、NXShadowWidget

## # **Track Record 4**：	Commits on Sep 7, 2025

## 1. Modify Optize1

> 1. 影响文件NXWindow.h、NXWindow.cpp、NXWindowPrivate.h
>
>    删除currentVisibleWidget()逻辑
>
>    删除navigationNodeAdded信号参数
>
>    调整navigationNodeRemoved信号和NXWindowPrivate::onNavigationNodeRemoved的逻辑，删除节点后仍旧存在节点窗口就返回当前节点窗口的key（如，有两个节点1、2，当前处在节点2，删除节点2，那么将会返回节点1窗口的key，底层是QMap按照顺序来的）和窗口指针，如果节点已经删完返回rootkey和空指针
>
>    调整onNavigationNodeClicked逻辑
>
> 2. 调整添加节点的返回值，NodeOperateReturnTypeWithKey从QPair替换为struct
>
> 3. 修复开启拖拽移动NXNavigationNode的时候，以下**拖拽情况1**的情况造成程序中断， `qDeleteAll`双重释放了-窗口节点2，不知道为什么？迭代器失效？但是控制台输出的记录顺序正确，已经提示删除了-窗口节点2，范围越界？可能是，调整逻辑不使用qDeleteAll，在`insertChildNode`添加`childNode->setParent(this);`让qt关联生命周期，不进行手动管理
>
>    示例：
>
>    * **原布局	**				**拖拽情况1**				**拖拽情况2**
>
>    根节点 
>
>    -窗口节点1				  -窗口节点1				-窗口节点1
>
>    -挂载拓展节点1 		 -挂载拓展节点1		-窗口节点2 
>
>    --窗口节点2 				-窗口节点2				-挂载拓展节点1
>
>    -窗口节点3				  -窗口节点3			    -窗口节点3
>
>    4. 导航结点交换逻辑完善，新增支持两个窗口节点直接交换位置
>
> 4. 修复  同步 `Ela Commits on Sep 7, 2025` 版本的 NXCustomTabWidget、NXTabWidget、移除窗口逻辑，原版本CMake编译不会出现双重释放情况，但使用VS2022编译后会出现该错误 (错误出现位置 析构~NXTabWidget(), 操作了不该操作被释放的QVariant)

## 2. Modify Optize2

同步作者 ZongChang-Liu 的部分拓展 ElaWidgetTools代码

https://github.com/ZongChang-Liu/ElaWidgetTools/commits/Zongchang_Liu?author=ZongChang-Liu

> 1. 修改NXToggleButton、支持添加 NXIcon，
>
>    `NXToggleButton.h、NXToggleButton.cpp、NXToggleButtonPrivate.h`
>
> 2. NXToolTip添加显示偏移api,NXSlider添加ToolTip显示值
>
>    `NXToolTip.h、NXToolTip.cpp、NXToolTipPrivate.h、NXToolTipPrivate.cpp、NXSlider.h、NXSlider.cpp、NXSliderPrivate.h(新增)、NXSliderPrivate.cpp(新增)`
>
> 3. 为NXNavigationBar添加toolTip显示位置接口
>
>    `NXNavigationBar.h、NXNavigationBar.cpp、NXNavigationView.h、NXNavigationView.cpp`
>
>    修改NXNavigationBar使其更加适合无userCard的显示效果
>
>    `NXNavigationBar.cpp`
>
>    NXNavigationBar显示模式发生改变时会发送信号
>
> 4. 修改mac下NXSpinBox焦点异常显示的bug
>
>    `NXSpinBox.cpp`

## 3. Modify Optize3

> 1. 修改`singleton.h` 单例模式更通用以来标准库，不依赖qt
>
> 2. 修改`NXProperty.h`的setter、getter的宏定义，支持普通类型如int、char直接setter、支持QString自定义cvref限定符setter，支持自定义getter返回类型加&限定，支持大类型的setter移动
>
>    ```
>    PROPERTY: 需要和信号关联
>    PRIVATE: 不需要和信号关联
>    _H/_CPP: Pimpl版本
>    _EX_H/_EX_CPP: 自定义setter、getter类型限定符
>    _MOVE: setter拷贝大数据开销大，可以使用移动，但请你注意该类型必须实现移动构造/赋值或是POD类型，setter时候可以调用std::move传参数，但请注意生命周期
>    ```
>
>    



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

// 旧
_settingPage = new T_Setting(this);
... // 替换内容
_aboutPage->hide();



// 新
    addPageNode("HOME", _homePage, NXIconType::House);
#ifdef Q_OS_WIN
    // 默认是root 添加一个expander节点
    auto [type, NXDxgiKey] = addExpanderNode("NXDxgi", NXIconType::TvMusic);
    addPageNode("NXScreen", _elaScreenPage, NXDxgiKey, 3, NXIconType::ObjectGroup);
#endif
    // navigation(elaScreenWidget->property("NXPageKey").toString());
    addPageNode("NXBaseComponents", _baseComponentsPage, NXIconType::CabinetFiling);

    _viewKey = addExpanderNode("NXView", _rootKey, NXIconType::CameraViewfinder).nodeKey;
    addPageNode("NXListView", _listViewPage, _viewKey, 9, NXIconType::List);
    addPageNode("NXTableView", _tableViewPage, _viewKey, NXIconType::Table);
    addPageNode("NXTreeView", _treeViewPage, _viewKey, NXIconType::ListTree);
    expandNavigationNode(_viewKey);

    addPageNode("NXGraphics", _graphicsPage, 9, NXIconType::Paintbrush);
    addPageNode("NXCard", _cardPage, NXIconType::Cards);
    addPageNode("NXNavigation", _navigationPage, NXIconType::LocationArrow);
    addPageNode("NXPopup", _popupPage, NXIconType::Envelope);
    addPageNode("NXIcon", _iconPage, 99, NXIconType::FontCase);
    NodeOperateReturnTypeWithKey returnType1 = addExpanderNode("TEST4", NXIconType::Acorn);
    NodeOperateReturnTypeWithKey returnType2 = addExpanderNode("TEST5", returnType1.nodeKey, NXIconType::Acorn);
    addPageNode("Third Level", new QWidget(this), returnType2.nodeKey, NXIconType::Acorn);
    addExpanderNode("TEST6", _rootKey, NXIconType::Acorn);
    addExpanderNode("TEST7", _rootKey, NXIconType::Acorn);
    addExpanderNode("TEST8", _rootKey, NXIconType::Acorn);
    addExpanderNode("TEST9", _rootKey, NXIconType::Acorn);
    addExpanderNode("TEST10", _rootKey, NXIconType::Acorn);
    addExpanderNode("TEST11", _rootKey, NXIconType::Acorn);
    addExpanderNode("TEST12", _rootKey, NXIconType::Acorn);
    addExpanderNode("TEST13", _rootKey, NXIconType::Acorn);
    addExpanderNode("TEST14", _rootKey, NXIconType::Acorn);
    addExpanderNode("TEST15", NXIconType::Acorn);
    addExpanderNode("TEST16", NXIconType::Acorn);
    addExpanderNode("TEST17", NXIconType::Acorn);

    _aboutKey = addFooterNode("About", nullptr, 0, NXIconType::User).nodeKey;
    _aboutPage = new T_About();


// 旧
    addFooterNode("Setting", _settingPage, _settingKey, 0, ElaIconType::GearComplex);
// 新
    _settingKey = addFooterNode("Setting", _settingPage, 0, NXIconType::GearComplex).nodeKey;

```

