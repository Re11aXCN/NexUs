#ifndef NXTEXTPRIVATE_H
#define NXTEXTPRIVATE_H

#include <QObject>

#include "NXDef.h"
class NXText;
class NXTextPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXText)
    Q_PROPERTY_CREATE_D(NXTextType::TextStyle, TextStyle)
    Q_PROPERTY_CREATE_D(NXIconType::IconName, NXIcon)
public:
    explicit NXTextPrivate(QObject* parent = nullptr);
    ~NXTextPrivate() override;
    Q_SLOT void onThemeChanged(NXThemeType::ThemeMode themeMode);

private:
    NXThemeType::ThemeMode _themeMode;
    qreal _textSpacing{0.5};
    bool _isWrapAnywhere{ false };
    void _changeTheme();

};

#endif // NXTEXTPRIVATE_H
