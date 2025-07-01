#ifndef NXFRAMEWORK_NXWIDGETTOOLS_DEVELOPERCOMPONENTS_NXDRAWERCONTAINER_H_
#define NXFRAMEWORK_NXWIDGETTOOLS_DEVELOPERCOMPONENTS_NXDRAWERCONTAINER_H_

#include <QWidget>

#include "NXDef.h"
#include <QVBoxLayout>

#include <QGraphicsOpacityEffect>
class NXDrawerContainer : public QWidget
{
    Q_OBJECT
    Q_PRIVATE_CREATE(int, BorderRadius)
public:
    explicit NXDrawerContainer(QWidget* parent = nullptr);
    ~NXDrawerContainer() override;

    void addWidget(QWidget* widget);
    void removeWidget(QWidget* widget);

    void doDrawerAnimation(bool isExpand);

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QGraphicsOpacityEffect* _opacityEffect{nullptr};
    NXThemeType::ThemeMode _themeMode;
    QVBoxLayout* _mainLayout{nullptr};
    QVBoxLayout* _containerLayout{nullptr};
    QWidget* _containerWidget{nullptr};
    QList<QWidget*> _drawerWidgetList;
};

#endif //NXFRAMEWORK_NXWIDGETTOOLS_DEVELOPERCOMPONENTS_NXDRAWERCONTAINER_H_
