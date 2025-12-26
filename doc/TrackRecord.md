# 一、修改的文件

## 变量替换

ElaAwesome
NXAwesome

eApp
nxApp

eWinHelper
nxWinHelper

eTheme
nxTheme

QNXpsedTimer
QElapsedTimer

TCP_NODNXY
TCP_NODELAY

## 文件改动

**.h文件中的成员变量结构布局改动**
**所有函数签名的 QString 参数 替换为了 const QString&，以宏Q_PROPERTY_CREATE_Q_H包裹的被替换了**





**DeveloperComponents/NXCheckBoxStyle.cpp**  

* 添加了_pCheckBorderRadius
* drawControl有逻辑改动

**DeveloperComponents/NXListViewStyle.cpp**

* drawControl的 case QStyle::CE_ShapedFrame: 部分 绘制条件逻辑改动

**DeveloperComponents/NXMicaBaseInitObject.cpp**

* onInitMicaBase 参数优化为引用，使用std::move移动图片，避免拷贝

**DeveloperComponents/NXTreeViewStyle.cpp**

* NXIconType::IconName枚举添加
* drawControl有逻辑改动，自定义前置图标 - 可以在每个树节点文字前添加自定义图标
NXTreeView
* setNXIcon接口添加以支持内置NXIconType::IconName字体图片渲染



**include/NXApplication.h**

* 单例模式预处理命令
include/NXDxgiManager.h
* 单例模式预处理命令
include/NXEventBus.h
* 单例模式预处理命令
include/NXExponentialBlur.h
* 单例模式预处理命令
include/NXLog.h
* 单例模式预处理命令
include/NXNavigationRouter.h
* 单例模式预处理命令
include/NXTheme.h
* 单例模式预处理命令



**NXContentDialog.cpp**

* appBar()接口提供

**NXDockWidget.cpp**

* dockClosed、dockResized信号添加



**NXMessageButton.cpp**

* message响应逻辑分离到 Private
* 提供 点击后消息发送的 disconnected 断开接口，可以使用showMessgae替代
private/NXMessageButtonPrivate.cpp
* _showMessage实现

**NXPushButton.cpp**

* 新增字体大小设置接口

**NXRadioButton.cpp**

* 默认setMouseTracking以设置鼠标手势
* 新增字体大小设置接口

**NXSlider.cpp**
* 添加 滑动时 tooltip显示value 逻辑

**NXToggleButton.cpp**

* 添加NXIconType::IconName支持内置字体的图标设置
private/NXToggleButtonPrivate.h
* 变量添加

**NXToolButton.cpp**

* 添加NXAdvancedToolButton，作为NXToolButton父类，以支持鼠标滚轮右键点击事件

**NXToolTip.cpp**

* OffSetX、OffSetY变量添加，可设置偏移
* updatePos 接口修改
private/NXToolTipPrivate.cpp
* 变量设置初始化

**NXWidget.cpp**

* appBar接口提供
* 自定义背景色接口提供
private/NXWidgetPrivate.h
* 变量设置



<u>**以下文件修改内容较多**</u>

1. 关于Navigation的

**DeveloperComponents/NXFooterModel.cpp**

* 导航栏的返回参数修改 void -> NodeOperateReturnTypeWithKey，替换引用传入获取key

**DeveloperComponents/NXNavigationModel.cpp**

* 导航栏的返回参数修改 void -> std::expected or tl::expected，替换引用传入获取key
* 拖拽功能支持，指示器显示位置
* getRootNode()接口提供

**DeveloperComponents/NXNavigationNode.cpp**

* 根节点属性修改，属于IsExpanded
* node标题接口设置添加
* 添加insertChildNode指示位置添加node接口

**DeveloperComponents/NXNavigationView.cpp**

* 拖拽功能支持，指示器显示位置
* 添加获取ToolTip内容接口
* _doCompactToolTip修改

**NXNavigationBar.cpp**

* navigationPositionSwapped信号槽建立
* navigationCloseCurrentWindow信号槽建立
* rowsMoved信号槽建立，以支持拖拽
* setIsLeftButtonPressedToggleNavigation、setNavigationNodeDragAndDropEnable接口提供支持拖拽设置
* setToolTipOffset接口提供设置提示位置偏移
* setNavigationPageOpenPolicy接口提供设置打开自定义窗口（待完善）
* getNavigationRootKey接口提供获取rootKey，以能够设置挂载
* 导航栏的返回参数修改 void -> NodeOperateReturnTypeWithKey，替换引用传入获取key

**private/NXNavigationBarPrivate.cpp**

* onNavigationCloseCurrentWindow实现
* navigationNodeClicked信号发送位置调整

**NXWindow.cpp**

* 将转发点击信号移动到跳转处理槽函数内转发
* 提供switch切换nodepage接口，根据nodekey
* setIsLeftButtonPressedToggleNavigation、setNavigationNodeDragAndDropEnable接口提供支持拖拽设置
* getNavigationRootKey接口提供获取rootKey，以能够设置挂载
* setNavigationPageOpenPolicy接口提供设置打开自定义窗口
* 导航栏的返回参数修改 void -> NodeOperateReturnTypeWithKey，替换引用传入获取key
private/NXWindowPrivate.cpp
* onNavigationNodeRemoved、onNavigationNodeClicked逻辑修改



2. 关于控件的的

**DeveloperComponents/NXTableViewStyle.cpp**

* 新增：是否启用隔行变色、是否启用选中效果、是否启用悬停效果、圆角半径变量 变量以及接口设置提供
* adjustHeaderColumnIconRect、adjustColummTextRect接口添加提供内边距设置，进行微调文本绘制位置
* CE_HeaderLabel中 添加表头图标绘制
* CE_ItemViewItem中 为第一列添加了复选框绘制逻辑
* 为 QAbstractItemView::SelectItems模式 在底部绘制水平条 表示为选中
NXTableView.cpp
* 提供IsDrawAlternateRowsEnabled、IsSelectionEffectsEnabled、IsHoverEffectsEnabled、圆角，Header字体大小，内容margin接口
* NXModelIndexWidget 添加给TableView添加小控件（逻辑可能需要更加完善）
private/NXTableViewPrivate.h
* NXModelIndexWidgetPrivate类定义
* DrawSelectionBackground采用变量

**DeveloperComponents/NXToolButtonStyle.cpp**

* _drawIcon逻辑修改，为不同的Icon设置位置被渲染的逻辑进行修改，符合视觉

**NXCheckBox.cpp**

* 字体大小、圆角、宽度接口添加

**NXColorDialog.cpp**

* 添加 alpha 处理
* getCurrentColorRGB 接口名字修改为 getCurrent4ChannelColor
DeveloperComponents/NXColorValueSliderStyle.cpp
* 添加 alpha 处理
private/NXColorDialogPrivate.cpp
* 添加 alpha 处理

**NXLineEdit.cpp**

* 添加 圆角设置接口，内容边距设置接口
* 委托构造函数添加
DeveloperComponents/NXLineEditStyle.cpp
* 添加 圆角参数设置 
* drawControl有逻辑改动
* 添加 pixelMetric 处理 LineEdit的 icon margin显示
private/NXLineEditPrivate.h
* 移除部分成员变量到NXLineEditStyle

**NXMenu.cpp**

* 添加圆角设置接口
* showEvent修改，子menu对齐
* 提供 子menu 对其到 父menu 接口 setAlignParentMenuHeight，isHasParentMenu接口进行判断是否是父menu
DeveloperComponents/NXMenuStyle.cpp
* 添加 圆角参数设置 
* drawControl有逻辑改动，绘制了悬停背景色，还在左侧绘制了一个小竖条（表示选中或焦点），并且悬停时图标和文字有轻微的位移效果

**NXText.cpp**

* 提供border type选择设置颜色粗细接口
* 字体大小接口修改
* 支持点击发送信号
private/NXTextPrivate.h
* 变量添加



3. 通用工具、枚举、宏定义相关

**include/NXDef.h**

* 枚举宏移除，分离到include/NXProperty.h
* 新增部分枚举
  include/NXProperty.h
* 新增资产宏
* 枚举导出宏修改

**include/singleton.h**

* 单例宏修改





4. 新文件添加
   private/NXSliderPrivate.cpp
   private/NXCheckBoxPrivate.cpp
   private/NXSliderPrivate.cpp
   NXSharedMemoryManager.cpp
   NXAESEncryption.cpp
   NXCentralStackedWidget.cpp （导出该控件而不是Development）
   NXShadowGraphicsEffect.cpp
   NXShadowWidget.cpp
   private/NXShadowGraphicsEffectPrivate.cpp



5. 库引入

include/expected.hpp

include/magic_enum

include/aesni



# 二、改动记录

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

## # **Track Record 3**：	Commits on Jul 8, 2025

### 1. Modify Optize1

> 1. 引入NXShadowGraphicsEffect、NXShadowWidget

## # **Track Record 4**：	Commits on Dec 26, 2025

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
> 3. 修改NXTableView、NXNavigationView、NXNavigation，优化已有的IndexWidget和指示器代码逻辑
>
> 4. NXTabWidget的双重释放问题Ela已经修复，NXCustomTabWidget、NXTabWidget合并Ela代码
>
> 5. NXTabBar版本适配
>
> ```NXTabBar.cpp
> void NXTabBar::dragEnterEvent(QDragEnterEvent* event)
> {
>     Q_D(NXTabBar);
>     if (event->mimeData()->property("DragType").toString() == "NXTabBarDrag")
>     {
>         event->acceptProposedAction();
>         auto mimeData = const_cast<QMimeData*>(event->mimeData());
>         d->_mimeData = mimeData;
> #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
>         mimeData->setProperty("TabDropIndex", tabAt(event->position().toPoint()));
> #else
>         mimeData->setProperty("TabDropIndex", tabAt(event->pos()));
> #endif
>         Q_EMIT tabDragEnter(mimeData);
>         QTimer::singleShot(10, this, [=]() {
> #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
>             QPoint pressPos(tabRect(currentIndex()).x() + 110, 0);
>             QMouseEvent pressEvent(QEvent::MouseButtonPress, pressPos, mapToGlobal(pressPos), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
> #else
>             QMouseEvent pressEvent(QEvent::MouseButtonPress, QPoint(tabRect(currentIndex()).x() + 110, 0), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
> #endif
>             QApplication::sendEvent(this, &pressEvent);
> #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
>             QPoint movePos(event->position().toPoint().x(), 0);
>             QMouseEvent moveEvent(QEvent::MouseMove, movePos, mapToGlobal(movePos), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
> #else
>             QMouseEvent moveEvent(QEvent::MouseMove, QPoint(event->pos().x(), 0), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
> #endif
>             QApplication::sendEvent(this, &moveEvent);
>             });
>     }
>     QTabBar::dragEnterEvent(event);
> }
> 
> void NXTabBar::dragMoveEvent(QDragMoveEvent* event)
> {
>     Q_D(NXTabBar);
>     if (event->mimeData()->property("DragType").toString() == "NXTabBarDrag")
>     {
> #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
>         QPoint movePos(event->position().toPoint().x(), 0);
>         QMouseEvent moveEvent(QEvent::MouseMove, movePos, mapToGlobal(movePos), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
> #else
>         QMouseEvent moveEvent(QEvent::MouseMove, QPoint(event->pos().x(), 0), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
> #endif
>         QApplication::sendEvent(this, &moveEvent);
>     }
>     QWidget::dragMoveEvent(event);
> }
> ```
>
> 6. CMake优化生成，可选自动打包和Export导出Ela三方库
> 6. 引入tl::expected，如果支持C++23的std::expected则使用标准库的，NodeOperateReturnType -> NodeOperateError, NodeOperateReturnType::Success ->NodeOperateError::None

