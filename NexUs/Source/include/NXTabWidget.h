#ifndef NXTABWIDGET_H
#define NXTABWIDGET_H

#include <QTabWidget>

#include "NXProperty.h"

class NXTabWidgetPrivate;
class NX_EXPORT NXTabWidget : public QTabWidget
{
    Q_OBJECT
    Q_Q_CREATE(NXTabWidget)
    Q_PROPERTY_CREATE(bool, IsTabTransparent);

public:
    explicit NXTabWidget(QWidget* parent = nullptr);
    ~NXTabWidget();
    void setTabPosition(TabPosition position);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void dragEnterEvent(QDragEnterEvent* event) override;
    virtual void dropEvent(QDropEvent* event) override;
};

#endif // NXTABWIDGET_H
