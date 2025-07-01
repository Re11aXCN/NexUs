#ifndef NXTABBARPRIVATE_H
#define NXTABBARPRIVATE_H

#include <QObject>
#include <QPixmap>

#include "stdafx.h"
class NXTabBar;
class NXTabBarPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXTabBar)
public:
    explicit NXTabBarPrivate(QObject* parent = nullptr);
    ~NXTabBarPrivate();

private:
    QPixmap _lastDragPix;
};

#endif // NXTABBARPRIVATE_H
