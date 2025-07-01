#ifndef NXDOUBLESPINBOX_H
#define NXDOUBLESPINBOX_H

#include <QDoubleSpinBox>

#include "stdafx.h"

class NXDoubleSpinBoxPrivate;
class NX_EXPORT NXDoubleSpinBox : public QDoubleSpinBox
{
    Q_OBJECT
    Q_Q_CREATE(NXDoubleSpinBox)
public:
    explicit NXDoubleSpinBox(QWidget* parent = nullptr);
    ~NXDoubleSpinBox();

protected:
    void contextMenuEvent(QContextMenuEvent* event) override;
};

#endif // NXDOUBLESPINBOX_H
