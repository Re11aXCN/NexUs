#ifndef NXSLIDER_H
#define NXSLIDER_H

#include <QSlider>

#include "NXProperty.h"
class NX_EXPORT NXSlider : public QSlider
{
    Q_OBJECT
public:
    explicit NXSlider(QWidget* parent = nullptr);
    explicit NXSlider(Qt::Orientation orientation, QWidget* parent = nullptr);
    ~NXSlider();
};

#endif // NXSLIDER_H
