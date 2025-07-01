#ifndef T_SETTING_H
#define T_SETTING_H

#include "T_BasePage.h"
class NXRadioButton;
class NXToggleSwitch;
class NXComboBox;
class T_Setting : public T_BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit T_Setting(QWidget* parent = nullptr);
    ~T_Setting();

private:
    NXComboBox* _themeComboBox{nullptr};
    NXToggleSwitch* _micaSwitchButton{nullptr};
    NXToggleSwitch* _logSwitchButton{nullptr};
    NXRadioButton* _minimumButton{nullptr};
    NXRadioButton* _compactButton{nullptr};
    NXRadioButton* _maximumButton{nullptr};
    NXRadioButton* _autoButton{nullptr};
};

#endif // T_SETTING_H
