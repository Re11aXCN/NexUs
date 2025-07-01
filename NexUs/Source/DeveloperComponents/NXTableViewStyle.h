#ifndef NXTABLEVIEWSTYLE_H
#define NXTABLEVIEWSTYLE_H
#include <QModelIndex>
#include <QProxyStyle>

#include "NXDef.h"
struct NXAdjustParam;
class QStyleOptionViewItem;
class NXTableViewStyle : public QProxyStyle
{
    Q_OBJECT
    Q_PROPERTY_CREATE(int, HeaderMargin)
    Q_PROPERTY_CREATE(int, CurrentHoverRow)
	Q_PROPERTY_CREATE(int, BorderRadius)
	Q_PROPERTY_CREATE(bool, IsDrawAlternateRowsEnabled)
	Q_PROPERTY_CREATE(bool, IsSelectionEffectsEnabled)

public:
    explicit NXTableViewStyle(QStyle* style = nullptr);
    ~NXTableViewStyle() override;
    void drawPrimitive(PrimitiveElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr, const QWidget* widget = nullptr) const override;

    void setAdjustParams(const QMap<int, NXAdjustParam>& adjustParamMap);
    void setHeaderAdjustParam(const QMap<int, NXAdjustParam>& adjustParamMap);
    QMap<int, NXAdjustParam> getAdjustParams() const;private:

    int _horizontalPadding{ 11 };
    NXThemeType::ThemeMode _themeMode;
    int _borderRadius{ 3 };
    QMap<int, NXAdjustParam> _adjustParamsMap;
    QMap<int, NXAdjustParam> _headerAdjustParamMap;
};

#endif // NXTABLEVIEWSTYLE_H
