#ifndef NXADVANCEDTOOLBUTTON_H
#define NXADVANCEDTOOLBUTTON_H
#include <QToolButton>
#include "stdafx.h"
class NX_EXPORT NXAdvancedToolButton : public QToolButton {
    Q_OBJECT

public:
    explicit NXAdvancedToolButton(QWidget* parent = nullptr);

    Q_SIGNAL void rightReleased();
    Q_SIGNAL void rightPressed();
    Q_SIGNAL void middleReleased();
    Q_SIGNAL void middlePressed();

protected:
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
};
#endif // !V
