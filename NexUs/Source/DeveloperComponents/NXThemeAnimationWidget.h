#ifndef NXTHEMEANIMATIONWIDGET_H
#define NXTHEMEANIMATIONWIDGET_H

#include <QWidget>

#include "stdafx.h"
class NXThemeAnimationWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY_CREATE(qreal, Radius)
    Q_PROPERTY_CREATE(qreal, EndRadius)
    Q_PROPERTY_CREATE(QPoint, Center)
    Q_PROPERTY_CREATE(QImage, OldWindowBackground)
    Q_PROPERTY_CREATE(QImage, NewWindowBackground)
public:
    explicit NXThemeAnimationWidget(QWidget* parent = nullptr);
    ~NXThemeAnimationWidget();
    void startAnimation(int msec);
Q_SIGNALS:
    Q_SIGNAL void animationFinished();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // NXTHEMEANIMATIONWIDGET_H
