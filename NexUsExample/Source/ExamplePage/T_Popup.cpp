#include "T_Popup.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "NXCalendar.h"
#include "NXCalendarPicker.h"
#include "NXCheckBox.h"
#include "NXColorDialog.h"
#include "NXDrawerArea.h"
#include "NXKeyBinder.h"
#include "NXMenu.h"
#include "NXPushButton.h"
#include "NXRoller.h"
#include "NXScrollPageArea.h"
#include "NXText.h"
#include "NXToggleSwitch.h"
#include "NXToolButton.h"
T_Popup::T_Popup(QWidget* parent)
    : T_BasePage(parent)
{
    // 预览窗口标题
    setWindowTitle("NXPopup");

    // 顶部元素
    createCustomWidget("一些常用的弹出组件被放置于此，可在此界面体验其效果并按需添加进项目中");

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("NXPopup");

    _toolButton = new NXToolButton(this);
    _toolButton->setIsTransparent(false);
    _toolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    //_toolButton->setPopupMode(QToolButton::MenuButtonPopup);
    _toolButton->setText("NXToolButton");

    NXMenu* menu = new NXMenu(this);
    menu->addNXIconAction(NXIconType::JackOLantern, "JackOLantern");
    menu->addNXIconAction(NXIconType::LacrosseStick, "LacrosseStick");
    _toolButton->setMenu(menu);
    _toolButton->setNXIcon(NXIconType::Broom);

    NXScrollPageArea* toolButtonArea = new NXScrollPageArea(this);
    QHBoxLayout* toolButtonLayout = new QHBoxLayout(toolButtonArea);
    NXText* toolButtonText = new NXText("NXToolButton", this);
    toolButtonText->setTextPixelSize(15);
    toolButtonLayout->addWidget(toolButtonText);
    toolButtonLayout->addWidget(_toolButton);
    toolButtonLayout->addStretch();

    _colorDialog = new NXColorDialog(this);
    NXText* colorText = new NXText(_colorDialog->getCurrent4ChannelColor(), this);
    colorText->setTextPixelSize(15);
    NXPushButton* colorDialogButton = new NXPushButton(this);
    colorDialogButton->setFixedSize(35, 35);
    colorDialogButton->setLightDefaultColor(_colorDialog->getCurrentColor());
    colorDialogButton->setLightHoverColor(_colorDialog->getCurrentColor());
    colorDialogButton->setLightPressColor(_colorDialog->getCurrentColor());
    colorDialogButton->setDarkDefaultColor(_colorDialog->getCurrentColor());
    colorDialogButton->setDarkHoverColor(_colorDialog->getCurrentColor());
    colorDialogButton->setDarkPressColor(_colorDialog->getCurrentColor());
    connect(colorDialogButton, &NXPushButton::clicked, this, [=]() {
        _colorDialog->exec();
    });
    connect(_colorDialog, &NXColorDialog::colorSelected, this, [=](const QColor& color) {
        colorDialogButton->setLightDefaultColor(color);
        colorDialogButton->setLightHoverColor(color);
        colorDialogButton->setLightPressColor(color);
        colorDialogButton->setDarkDefaultColor(color);
        colorDialogButton->setDarkHoverColor(color);
        colorDialogButton->setDarkPressColor(color);
        colorText->setText(_colorDialog->getCurrent4ChannelColor());
    });

    NXScrollPageArea* colorDialogArea = new NXScrollPageArea(this);
    QHBoxLayout* colorDialogLayout = new QHBoxLayout(colorDialogArea);
    NXText* colorDialogText = new NXText("NXColorDialog", this);
    colorDialogText->setTextPixelSize(15);
    colorDialogLayout->addWidget(colorDialogText);
    colorDialogLayout->addWidget(colorDialogButton);
    colorDialogLayout->addWidget(colorText);
    colorDialogLayout->addStretch();

    _calendar = new NXCalendar(this);

    _calendarPicker = new NXCalendarPicker(this);
    NXScrollPageArea* calendarPickerArea = new NXScrollPageArea(this);
    QHBoxLayout* calendarPickerLayout = new QHBoxLayout(calendarPickerArea);
    NXText* calendarPickerText = new NXText("NXCalendarPicker", this);
    calendarPickerText->setTextPixelSize(15);
    calendarPickerLayout->addWidget(calendarPickerText);
    calendarPickerLayout->addWidget(_calendarPicker);
    calendarPickerLayout->addStretch();

    _keyBinder = new NXKeyBinder(this);
    NXScrollPageArea* keyBinderArea = new NXScrollPageArea(this);
    QHBoxLayout* keyBinderLayout = new QHBoxLayout(keyBinderArea);
    NXText* keyBinderText = new NXText("NXKeyBinder", this);
    keyBinderText->setTextPixelSize(15);
    keyBinderLayout->addWidget(keyBinderText);
    keyBinderLayout->addWidget(_keyBinder);
    keyBinderLayout->addStretch();

    _roller = new NXRoller(this);
    QStringList rollerItemList;
    for (int i = 0; i < 100; i++)
    {
        rollerItemList.append(QString::number(i + 1));
    }
    _roller->setItemList(rollerItemList);
    NXScrollPageArea* rollerArea = new NXScrollPageArea(this);
    rollerArea->setFixedHeight(220);
    QHBoxLayout* rollerLayout = new QHBoxLayout(rollerArea);
    NXText* rollerText = new NXText("NXRoller", this);
    rollerText->setTextPixelSize(15);
    rollerLayout->addWidget(rollerText);
    rollerLayout->addWidget(_roller);
    rollerLayout->addStretch();

    _drawer = new NXDrawerArea(this);
    QWidget* drawerHeader = new QWidget(this);
    QHBoxLayout* drawerHeaderLayout = new QHBoxLayout(drawerHeader);
    NXText* drawerIcon = new NXText(this);
    drawerIcon->setTextPixelSize(15);
    drawerIcon->setNXIcon(NXIconType::MessageArrowDown);
    drawerIcon->setFixedSize(25, 25);
    NXText* drawerText = new NXText("NXDrawer", this);
    drawerText->setTextPixelSize(15);

    NXToggleSwitch* drawerSwitch = new NXToggleSwitch(this);
    NXText* drawerSwitchText = new NXText("关", this);
    drawerSwitchText->setTextPixelSize(15);
    connect(drawerSwitch, &NXToggleSwitch::toggled, this, [=](bool toggled) {
        if (toggled)
        {
            drawerSwitchText->setText("开");
            _drawer->expand();
        }
        else
        {
            drawerSwitchText->setText("关");
            _drawer->collpase();
        }
    });
    connect(_drawer, &NXDrawerArea::expandStateChanged, this, [=](bool isExpand) {
        drawerSwitch->setIsToggled(isExpand);
    });

    drawerHeaderLayout->addWidget(drawerIcon);
    drawerHeaderLayout->addWidget(drawerText);
    drawerHeaderLayout->addStretch();
    drawerHeaderLayout->addWidget(drawerSwitchText);
    drawerHeaderLayout->addWidget(drawerSwitch);

    _drawer->setDrawerHeader(drawerHeader);
    QWidget* drawerWidget1 = new QWidget(this);
    drawerWidget1->setFixedHeight(75);
    QHBoxLayout* drawerWidget1Layout = new QHBoxLayout(drawerWidget1);
    NXCheckBox* drawerCheckBox1 = new NXCheckBox("测试窗口1", this);
    drawerWidget1Layout->addSpacing(60);
    drawerWidget1Layout->addWidget(drawerCheckBox1);

    QWidget* drawerWidget2 = new QWidget(this);
    drawerWidget2->setFixedHeight(75);
    QHBoxLayout* drawerWidget2Layout = new QHBoxLayout(drawerWidget2);
    NXCheckBox* drawerCheckBox2 = new NXCheckBox("测试窗口2", this);
    drawerWidget2Layout->addSpacing(60);
    drawerWidget2Layout->addWidget(drawerCheckBox2);

    QWidget* drawerWidget3 = new QWidget(this);
    drawerWidget3->setFixedHeight(75);
    QHBoxLayout* drawerWidget3Layout = new QHBoxLayout(drawerWidget3);
    NXCheckBox* drawerCheckBox3 = new NXCheckBox("测试窗口3", this);
    drawerWidget3Layout->addSpacing(60);
    drawerWidget3Layout->addWidget(drawerCheckBox3);

    _drawer->addDrawer(drawerWidget1);
    _drawer->addDrawer(drawerWidget2);
    _drawer->addDrawer(drawerWidget3);

    QVBoxLayout* centerVLayout = new QVBoxLayout(centralWidget);
    centerVLayout->setContentsMargins(0, 0, 0, 0);
    centerVLayout->addWidget(toolButtonArea);
    centerVLayout->addWidget(colorDialogArea);
    centerVLayout->addWidget(calendarPickerArea);
    centerVLayout->addWidget(_calendar);
    centerVLayout->addWidget(keyBinderArea);
    centerVLayout->addWidget(_drawer);
    centerVLayout->addWidget(rollerArea);
    centerVLayout->addStretch();
    addCentralWidget(centralWidget, true, false, 0);
}

T_Popup::~T_Popup()
{
}
