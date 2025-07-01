#ifndef NXCUSTOMTABWIDGET_H
#define NXCUSTOMTABWIDGET_H

#include "NXCustomWidget.h"

class NXTabBar;
class NXTabWidget;
class NXCustomTabWidget : public NXCustomWidget
{
    Q_OBJECT
public:
    explicit NXCustomTabWidget(QWidget* parent = nullptr);
    ~NXCustomTabWidget();
    void addTab(QWidget* widget, QIcon& tabIcon, const QString& tabTitle);
    NXTabBar* getCustomTabBar() const;
    NXTabWidget* getCustomTabWidget() const;

private:
    NXTabBar* _customTabBar{nullptr};
    NXTabWidget* _customTabWidget{nullptr};
};

#endif // NXCUSTOMTABWIDGET_H
