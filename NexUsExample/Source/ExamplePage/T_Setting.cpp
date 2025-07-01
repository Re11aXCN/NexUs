#include "T_Setting.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "NXApplication.h"
#include "NXComboBox.h"
#include "NXLog.h"
#include "NXRadioButton.h"
#include "NXScrollPageArea.h"
#include "NXText.h"
#include "NXTheme.h"
#include "NXToggleSwitch.h"
#include "NXWindow.h"
T_Setting::T_Setting(QWidget* parent)
    : T_BasePage(parent)
{
    // 预览窗口标题
    NXWindow* window = dynamic_cast<NXWindow*>(parent);
    setWindowTitle("Setting");

    NXText* themeText = new NXText("主题设置", this);
    themeText->setWordWrap(false);
    themeText->setTextPixelSize(18);

    _themeComboBox = new NXComboBox(this);
    _themeComboBox->addItem("日间模式");
    _themeComboBox->addItem("夜间模式");
    NXScrollPageArea* themeSwitchArea = new NXScrollPageArea(this);
    QHBoxLayout* themeSwitchLayout = new QHBoxLayout(themeSwitchArea);
    NXText* themeSwitchText = new NXText("主题切换", this);
    themeSwitchText->setWordWrap(false);
    themeSwitchText->setTextPixelSize(15);
    themeSwitchLayout->addWidget(themeSwitchText);
    themeSwitchLayout->addStretch();
    themeSwitchLayout->addWidget(_themeComboBox);
    connect(_themeComboBox, QOverload<int>::of(&NXComboBox::currentIndexChanged), this, [=](int index) {
        if (index == 0)
        {
            nxTheme->setThemeMode(NXThemeType::Light);
        }
        else
        {
            nxTheme->setThemeMode(NXThemeType::Dark);
        }
    });
    connect(nxTheme, &NXTheme::themeModeChanged, this, [=](NXThemeType::ThemeMode themeMode) {
        _themeComboBox->blockSignals(true);
        if (themeMode == NXThemeType::Light)
        {
            _themeComboBox->setCurrentIndex(0);
        }
        else
        {
            _themeComboBox->setCurrentIndex(1);
        }
        _themeComboBox->blockSignals(false);
    });

    NXText* helperText = new NXText("应用程序设置", this);
    helperText->setWordWrap(false);
    helperText->setTextPixelSize(18);

    _micaSwitchButton = new NXToggleSwitch(this);
    NXScrollPageArea* micaSwitchArea = new NXScrollPageArea(this);
    QHBoxLayout* micaSwitchLayout = new QHBoxLayout(micaSwitchArea);
    NXText* micaSwitchText = new NXText("启用云母效果(跨平台)", this);
    micaSwitchText->setWordWrap(false);
    micaSwitchText->setTextPixelSize(15);
    micaSwitchLayout->addWidget(micaSwitchText);
    micaSwitchLayout->addStretch();
    micaSwitchLayout->addWidget(_micaSwitchButton);
    connect(_micaSwitchButton, &NXToggleSwitch::toggled, this, [=](bool checked) {
        nxApp->setIsEnableMica(checked);
    });

    _logSwitchButton = new NXToggleSwitch(this);
    NXScrollPageArea* logSwitchArea = new NXScrollPageArea(this);
    QHBoxLayout* logSwitchLayout = new QHBoxLayout(logSwitchArea);
    NXText* logSwitchText = new NXText("启用日志功能", this);
    logSwitchText->setWordWrap(false);
    logSwitchText->setTextPixelSize(15);
    logSwitchLayout->addWidget(logSwitchText);
    logSwitchLayout->addStretch();
    logSwitchLayout->addWidget(_logSwitchButton);
    connect(_logSwitchButton, &NXToggleSwitch::toggled, this, [=](bool checked) {
        NXLog::getInstance()->initMessageLog(checked);
        if (checked)
        {
            qDebug() << "日志已启用!";
        }
        else
        {
            qDebug() << "日志已关闭!";
        }
    });

    _minimumButton = new NXRadioButton("Minimum", this);
    _compactButton = new NXRadioButton("Compact", this);
    _maximumButton = new NXRadioButton("Maximum", this);
    _autoButton = new NXRadioButton("Auto", this);
    _autoButton->setChecked(true);
    NXScrollPageArea* displayModeArea = new NXScrollPageArea(this);
    QHBoxLayout* displayModeLayout = new QHBoxLayout(displayModeArea);
    NXText* displayModeText = new NXText("导航栏模式选择", this);
    displayModeText->setWordWrap(false);
    displayModeText->setTextPixelSize(15);
    displayModeLayout->addWidget(displayModeText);
    displayModeLayout->addStretch();
    displayModeLayout->addWidget(_minimumButton);
    displayModeLayout->addWidget(_compactButton);
    displayModeLayout->addWidget(_maximumButton);
    displayModeLayout->addWidget(_autoButton);
    connect(_minimumButton, &NXRadioButton::toggled, this, [=](bool checked) {
        if (checked)
        {
            window->setNavigationBarDisplayMode(NXNavigationType::Minimal);
        }
    });
    connect(_compactButton, &NXRadioButton::toggled, this, [=](bool checked) {
        if (checked)
        {
            window->setNavigationBarDisplayMode(NXNavigationType::Compact);
        }
    });
    connect(_maximumButton, &NXRadioButton::toggled, this, [=](bool checked) {
        if (checked)
        {
            window->setNavigationBarDisplayMode(NXNavigationType::Maximal);
        }
    });
    connect(_autoButton, &NXRadioButton::toggled, this, [=](bool checked) {
        if (checked)
        {
            window->setNavigationBarDisplayMode(NXNavigationType::Auto);
        }
    });

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("Setting");
    QVBoxLayout* centerLayout = new QVBoxLayout(centralWidget);
    centerLayout->addSpacing(30);
    centerLayout->addWidget(themeText);
    centerLayout->addSpacing(10);
    centerLayout->addWidget(themeSwitchArea);
    centerLayout->addSpacing(15);
    centerLayout->addWidget(helperText);
    centerLayout->addSpacing(10);
    centerLayout->addWidget(logSwitchArea);
    centerLayout->addWidget(micaSwitchArea);
    centerLayout->addWidget(displayModeArea);
    centerLayout->addStretch();
    centerLayout->setContentsMargins(0, 0, 0, 0);
    addCentralWidget(centralWidget, true, true, 0);
}

T_Setting::~T_Setting()
{
}
