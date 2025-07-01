# NexUs/MSVC sln
**ElaWidgetTools QT开源库的修改封装部分代码**
<u>https://github.com/Liniyous/ElaWidgetTools/commits/main/</u>



## # **Track Record 1**：	Commits on Mar 12, 2025

### 1. Modify Optize1

> 已修改过NXWidget、NXWindow、NXColorDialog、NXTableView等一些基本的UI控件，修改了NXDef.h、stdafx.h宏定义

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
