#ifndef NXRADIOBUTTONPRIVATE_H
#define NXRADIOBUTTONPRIVATE_H
#include <QObject>

#include "NXDef.h"
class NXRadioButton;
class NXRadioButtonStyle;
class NXRadioButtonPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXRadioButton)
public:
    explicit NXRadioButtonPrivate(QObject* parent = nullptr);
    ~NXRadioButtonPrivate();
    Q_SLOT void onThemeChanged(NXThemeType::ThemeMode themeMode);
private:
    std::shared_ptr<NXRadioButtonStyle> _radioButtonStyle{ nullptr };
    NXTextType::TextStyle _textStyle{ NXTextType::NoStyle };
};

#endif // NXRADIOBUTTONPRIVATE_H
