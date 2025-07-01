#ifndef NXSPINBOX_H
#define NXSPINBOX_H

#include <QSpinBox>

#include "stdafx.h"

class NXSpinBoxPrivate;
class NX_EXPORT NXSpinBox : public QSpinBox
{
    Q_OBJECT
    Q_Q_CREATE(NXSpinBox)
public:
    explicit NXSpinBox(QWidget* parent = nullptr);
    ~NXSpinBox();

protected:
    void contextMenuEvent(QContextMenuEvent* event) override;
};

#endif // NXSPINBOX_H
