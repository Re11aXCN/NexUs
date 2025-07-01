#ifndef NXTOGGLEBUTTONPRIVATE_H
#define NXTOGGLEBUTTONPRIVATE_H

#include <QObject>

#include "NXDef.h"
class NXToggleButton;
class NXToggleButtonPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXToggleButton)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE(int, ToggleAlpha)
    Q_PROPERTY_CREATE_D(QString, Text)

public:
    explicit NXToggleButtonPrivate(QObject* parent = nullptr);
    ~NXToggleButtonPrivate() override;

private:
    bool _isAlphaAnimationFinished{true};
    bool _isToggled{false};
    bool _isPressed{false};
    NXThemeType::ThemeMode _themeMode;
};

#endif // NXTOGGLEBUTTONPRIVATE_H
