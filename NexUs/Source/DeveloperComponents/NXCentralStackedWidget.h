#ifndef NXCENTRALSTACKEDWIDGET_H
#define NXCENTRALSTACKEDWIDGET_H

#include <QStackedWidget>

#include "NXDef.h"
class NXCentralStackedWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit NXCentralStackedWidget(QWidget* parent = nullptr);
    ~NXCentralStackedWidget();
    Q_SLOT void onxThemeModeChanged(NXThemeType::ThemeMode themeMode);

    void setIsTransparent(bool isTransparent);
    bool getIsTransparent() const;

    void setIsHasRadius(bool isHasRadius);

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    NXThemeType::ThemeMode _themeMode;
    bool _isTransparent{false};
    bool _isHasRadius{ true };
};

#endif // NXCENTRALSTACKEDWIDGET_H
