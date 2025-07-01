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
