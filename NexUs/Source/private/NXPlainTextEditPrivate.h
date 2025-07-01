#ifndef NXPLAINTEXTEDITPRIVATE_H
#define NXPLAINTEXTEDITPRIVATE_H

#include <QObject>

#include "NXDef.h"
class NXEvent;
class NXPlainTextEdit;
class NXPlainTextEditStyle;
class NXPlainTextEditPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXPlainTextEdit)

public:
    explicit NXPlainTextEditPrivate(QObject* parent = nullptr);
    ~NXPlainTextEditPrivate();
    Q_INVOKABLE void onWMWindowClickedEvent(QVariantMap data);
    Q_SLOT void onThemeChanged(NXThemeType::ThemeMode themeMode);

private:
    NXThemeType::ThemeMode _themeMode;
    NXPlainTextEditStyle* _style{nullptr};
    NXEvent* _focusEvent{nullptr};
    void _changeTheme();
};

#endif // NXPLAINTEXTEDITPRIVATE_H
