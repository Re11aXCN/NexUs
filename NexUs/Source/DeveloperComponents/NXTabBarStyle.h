#ifndef NXTABBARSTYLE_H
#define NXTABBARSTYLE_H

#include <QProxyStyle>

#include "NXDef.h"
class NXTabBarStyle : public QProxyStyle
{
    Q_OBJECT
public:
    explicit NXTabBarStyle(QStyle* style = nullptr);
    ~NXTabBarStyle();
    void drawPrimitive(QStyle::PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w) const override;
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const override;
    QRect subElementRect(SubElement element, const QStyleOption* option, const QWidget* widget) const override;

private:
    NXThemeType::ThemeMode _themeMode;
};

#endif // NXTABBARSTYLE_H
