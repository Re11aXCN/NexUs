﻿#ifndef NXGRAPHICSLINEITEMPRIVATE_H
#define NXGRAPHICSLINEITEMPRIVATE_H

#include <QMap>
#include <QObject>
#include <QPointF>

#include "stdafx.h"
class NXGraphicsItem;
class NXGraphicsLineItem;
class NXGraphicsLineItemPrivate
{
    Q_D_CREATE(NXGraphicsLineItem)
    Q_PRIVATE_CREATE_D(QPointF, StartPoint);
    Q_PRIVATE_CREATE_D(QPointF, EndPoint);
    Q_PRIVATE_CREATE_D(NXGraphicsItem*, StartItem);
    Q_PRIVATE_CREATE_D(NXGraphicsItem*, EndItem);
    Q_PRIVATE_CREATE_D(int, StartItemPort);
    Q_PRIVATE_CREATE_D(int, EndItemPort);

public:
    explicit NXGraphicsLineItemPrivate();
    ~NXGraphicsLineItemPrivate();

private:
    bool _isCreateWithItem{true};
    QMap<NXGraphicsItem*, int> _linkItemMap;

};

#endif // NXGRAPHICSLINEITEMPRIVATE_H
