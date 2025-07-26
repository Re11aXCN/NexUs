#ifndef NXCENTRALSTACKEDWIDGET_H
#define NXCENTRALSTACKEDWIDGET_H

#include <QStackedWidget>

#include "NXDef.h"

class QGraphicsBlurEffect;
class NX_EXPORT NXCentralStackedWidget : public QStackedWidget
{
    Q_OBJECT
    Q_PROPERTY_CREATE(qreal, ScaleAnimationRatio)
    Q_PROPERTY_CREATE(qreal, ScaleAnimationPixOpacity)
    Q_PROPERTY_CREATE(qreal, FlipAnimationRatio)
    Q_PROPERTY_CREATE(int, PopupAnimationYOffset)
    Q_PROPERTY_CREATE(int, BlurAnimationRadius)
public:
    explicit NXCentralStackedWidget(QWidget* parent = nullptr);
    ~NXCentralStackedWidget() override;
    Q_SLOT void onThemeModeChanged(NXThemeType::ThemeMode themeMode);

    void setIsTransparent(bool isTransparent);
    bool getIsTransparent() const;

    void setIsHasRadius(bool isHasRadius);

    void doWindowStackSwitch(NXWindowType::StackSwitchMode stackSwitchMode, int nodeIndex, bool isRouteBack);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    NXWindowType::StackSwitchMode _stackSwitchMode{NXWindowType::StackSwitchMode::Popup};
    NXThemeType::ThemeMode _themeMode;
    bool _isTransparent{false};
    bool _isHasRadius{true};
    bool _isDrawNewPix{false};
    QPixmap _targetStackPix;
    QPixmap _currentStackPix;
    QGraphicsBlurEffect* _blurEffect{nullptr};

    void _getTargetStackPix();
    void _getCurrentStackPix();
};

#endif // NXCENTRALSTACKEDWIDGET_H
