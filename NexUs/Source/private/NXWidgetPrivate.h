#ifndef NXWIDGETPRIVATE_H
#define NXWIDGETPRIVATE_H

#include <QObject>
#include "NXDef.h"

class QStackedLayout;
class NXWidget;
class NXAppBar;
class NXWidgetPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXWidget)
    Q_PROPERTY_CREATE_D(QStackedLayout*, ModuleStackedLayout)
public:
    explicit NXWidgetPrivate(QObject* parent = nullptr);
    ~NXWidgetPrivate();

private:
    bool _isEnableMica;
    NXThemeType::ThemeMode _themeMode;
    NXAppBar* _appBar{nullptr};
};

#endif // NXWIDGETPRIVATE_H
