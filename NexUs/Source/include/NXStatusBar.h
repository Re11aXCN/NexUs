#ifndef NXSTATUSBAR_H
#define NXSTATUSBAR_H

#include <QStatusBar>

#include "stdafx.h"
class NX_EXPORT NXStatusBar : public QStatusBar
{
    Q_OBJECT
public:
    explicit NXStatusBar(QWidget* parent = nullptr);
    ~NXStatusBar();
};

#endif // NXSTATUSBAR_H
