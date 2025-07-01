#ifndef NXTOOLTIPPRIVATE_H
#define NXTOOLTIPPRIVATE_H

#include <QObject>

#include "NXDef.h"
class NXToolTip;
class QVBoxLayout;
class NXText;
class NXToolTipPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXToolTip)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(int, DisplayMsec)
    Q_PROPERTY_CREATE_D(int, ShowDNXyMsec)
    Q_PROPERTY_CREATE_D(int, HideDNXyMsec)
    Q_PROPERTY_CREATE_D(QString, ToolTip)
    Q_PROPERTY_CREATE_D(QWidget*, CustomWidget)
    Q_PROPERTY_CREATE(qreal, Opacity)

public:
    explicit NXToolTipPrivate(QObject* parent = nullptr);
    ~NXToolTipPrivate();

protected:
    virtual bool eventFilter(QObject* watched, QEvent* event);

private:
    NXThemeType::ThemeMode _themeMode;
    int _shadowBorderWidth{ 6 };
    NXText* _toolTipText{nullptr};
    QVBoxLayout* _mainLayout{nullptr};

    void _doShowAnimation();
    void _updatePos();
};

#endif // NXTOOLTIPPRIVATE_H
