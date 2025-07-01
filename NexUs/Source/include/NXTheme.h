#ifndef NXTHEME_H
#define NXTHEME_H

#include <QObject>

#include "NXDef.h"
#include "singleton.h"

#define nxTheme NXTheme::getInstance()
#define NXThemeColor(themeMode, themeColor) nxTheme->getThemeColor(themeMode, NXThemeType::themeColor)
class QPainter;
class NXThemePrivate;
class NX_EXPORT NXTheme : public QObject
{
    Q_OBJECT
    Q_Q_CREATE(NXTheme)
    Q_SINGLETON_CREATE_H(NXTheme)
private:
    explicit NXTheme(QObject* parent = nullptr);
    ~NXTheme();
public:
    void setThemeMode(NXThemeType::ThemeMode themeMode);
    NXThemeType::ThemeMode getThemeMode() const;

    void drawEffectShadow(QPainter* painter, QRect widgetRect, int shadowBorderWidth, int borderRadius);

    void setThemeColor(NXThemeType::ThemeMode themeMode, NXThemeType::ThemeColor themeColor, QColor newColor);
    const QColor& getThemeColor(NXThemeType::ThemeMode themeMode, NXThemeType::ThemeColor themeColor);
Q_SIGNALS:
    Q_SIGNAL void themeModeChanged(NXThemeType::ThemeMode themeMode);
};

#endif // NXTHEME_H
