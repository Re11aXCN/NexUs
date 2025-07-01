#ifndef NXWIDGETTOOLS_NXLCDNUMBERSTYLE_H
#define NXWIDGETTOOLS_NXLCDNUMBERSTYLE_H
#include <QProxyStyle>

#include "NXDef.h"
class NXLCDNumberStyle : public QProxyStyle
{
    Q_OBJECT
    Q_PRIVATE_CREATE(bool, IsTransparent)
public:
    explicit NXLCDNumberStyle(QStyle* style = nullptr);
    ~NXLCDNumberStyle();
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;

private:
    NXThemeType::ThemeMode _themeMode;
};

#endif //NXWIDGETTOOLS_NXLCDNUMBERSTYLE_H
