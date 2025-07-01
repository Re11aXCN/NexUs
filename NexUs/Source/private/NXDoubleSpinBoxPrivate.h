#ifndef NXDOUBLESPINBOXPRIVATE_H
#define NXDOUBLESPINBOXPRIVATE_H

#include <QObject>

#include "stdafx.h"
class NXMenu;
class NXDoubleSpinBox;
class NXDoubleSpinBoxPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXDoubleSpinBox)
public:
    explicit NXDoubleSpinBoxPrivate(QObject* parent = nullptr);
    ~NXDoubleSpinBoxPrivate();

private:
    NXMenu* _createStandardContextMenu();
};

#endif // NXDOUBLESPINBOXPRIVATE_H
