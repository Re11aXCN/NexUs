#ifndef NXAPPLICATIONPRIVATE_H
#define NXAPPLICATIONPRIVATE_H

#include <QColor>
#include <QIcon>
#include <QObject>

#include "NXDef.h"
class NXApplication;
class NXApplicationPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXApplication)
    NXThemeType::ThemeMode _themeMode;

    Q_PROPERTY_CREATE_D(bool, IsEnableMica)
    Q_PROPERTY_CREATE_D(QString, MicaImagePath)
public:
    explicit NXApplicationPrivate(QObject* parent = nullptr);
    ~NXApplicationPrivate() override;
    Q_SLOT void onxThemeModeChanged(NXThemeType::ThemeMode themeMode);

Q_SIGNALS:
    Q_SIGNAL void initMicaBase(QImage img);

protected:
    virtual bool eventFilter(QObject* watched, QEvent* event) override;

private:
    friend class NXMicaBaseInitObject;
    QVector<QWidget*> _micaWidgetList;
    QImage _lightBaseImage;
    QImage _darkBaseImage;
    void _initMicaBaseImage(QImage img);
    QRect _calculateWindowVirtualGeometry(QWidget* widget);
    void _updateMica(QWidget* widget, bool isProcessEvent = true);
    void _updateAllMicaWidget();
};

#endif // NXAPPLICATIONPRIVATE_H
