#include "NXAdvancedToolButton.h"
#include <QMouseEvent>
NXAdvancedToolButton::NXAdvancedToolButton(QWidget* parent)
    : QToolButton(parent)
{

}

void NXAdvancedToolButton::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::RightButton) {
        Q_EMIT rightReleased();
    }
    else if (event->button() == Qt::MiddleButton) {
        Q_EMIT middleReleased();
    }
    QToolButton::mouseReleaseEvent(event);
}

void NXAdvancedToolButton::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::RightButton) {
        Q_EMIT rightPressed();
    }
    else if (event->button() == Qt::MiddleButton) {
        Q_EMIT middlePressed();
    }
    QToolButton::mousePressEvent(event);
}
