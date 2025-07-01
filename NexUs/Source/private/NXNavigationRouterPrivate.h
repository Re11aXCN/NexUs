#ifndef NXNAVIGATIONROUTERPRIVATE_H
#define NXNAVIGATIONROUTERPRIVATE_H
#include <QObject>
#include <QQueue>
#include <QVariantMap>

#include "stdafx.h"
class NXNavigationRouter;
class NXNavigationRouterPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXNavigationRouter)
    Q_PROPERTY_CREATE_D(int, MaxRouteCount)
public:
    explicit NXNavigationRouterPrivate(QObject* parent = nullptr);
    ~NXNavigationRouterPrivate() override;

private:
    QQueue<QVariantMap> _routeQueue;
};

#endif // NXNAVIGATIONROUTERPRIVATE_H
