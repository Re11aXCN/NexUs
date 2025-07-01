#ifndef NXSPINBOXPRIVATE_H
#define NXSPINBOXPRIVATE_H

#include <QObject>

#include "stdafx.h"
class NXSpinBox;
class NXMenu;
class NXSpinBoxStyle;
class NXSpinBoxPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXSpinBox)
public:
    explicit NXSpinBoxPrivate(QObject* parent = nullptr);
    ~NXSpinBoxPrivate();

private:
    NXMenu* _createStandardContextMenu();
    std::shared_ptr<NXSpinBoxStyle> _spinBoxStyle{ nullptr };
};

#endif // NXSPINBOXPRIVATE_H
