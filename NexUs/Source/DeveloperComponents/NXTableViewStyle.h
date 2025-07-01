#ifndef NXTABLEVIEWSTYLE_H
#define NXTABLEVIEWSTYLE_H
#include <QModelIndex>
#include <QProxyStyle>

#include "NXDef.h"
struct NXAdjustParams;
class QStyleOptionViewItem;
class NXTableViewStyle : public QProxyStyle
{
    Q_OBJECT
    Q_PROPERTY_CREATE(int, HeaderMargin)
    Q_PROPERTY_CREATE(int, CurrentHoverRow)
    Q_PROPERTY_CREATE(QSize, IconSize)
public:
    explicit NXTableViewStyle(QStyle* style = nullptr);
    ~NXTableViewStyle();
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr, const QWidget* widget = nullptr) const override;

    void setAdjustParams(const QMap<int, NXAdjustParams>& adjustParamsMap);
    QMap<int, NXAdjustParams> getAdjustParams() const;
private:
    NXThemeType::ThemeMode _themeMode;
    int _horizontalPadding{11};
    QMap<int, NXAdjustParams> _adjustParamsMap;
};

#endif // NXTABLEVIEWSTYLE_H
