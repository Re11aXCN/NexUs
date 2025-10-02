#include "NXNavigationRouter.h"

#include "private/NXNavigationRouterPrivate.h"

SINGLETON_CREATE_CPP(NXNavigationRouter)
Q_PROPERTY_CREATE_Q_CPP(NXNavigationRouter, int, MaxRouteCount)
NXNavigationRouter::NXNavigationRouter(QObject* parent)
    : QObject{parent}, d_ptr(new NXNavigationRouterPrivate())
{
    Q_D(NXNavigationRouter);
    d->q_ptr = this;
    d->_pMaxRouteCount = 25;
}

NXNavigationRouter::~NXNavigationRouter()
{
}

NXNavigationRouterType::NavigationRouteType NXNavigationRouter::navigationRoute(QObject* routeObject, QString routeFunctionName, const QVariantMap& routeData, Qt::ConnectionType connectionType)
{
    Q_D(NXNavigationRouter);
    if (!routeObject)
    {
        return NXNavigationRouterType::ObjectInvalid;
    }
    if (routeFunctionName.isEmpty())
    {
        return NXNavigationRouterType::FunctionNameInvalid;
    }
    if (d->_routeQueue.isEmpty())
    {
        Q_EMIT navigationRouterStateChanged(true);
    }
    else
    {
        if (d->_routeQueue.count() >= d->_pMaxRouteCount)
        {
            d->_routeQueue.dequeue();
        }
    }
    QVariantMap saveData;
    saveData.insert("NXRouteObject", QVariant::fromValue<QObject*>(routeObject));
    saveData.insert("NXRouteFunctionName", routeFunctionName);
    saveData.insert("NXRouteData", routeData);
    saveData.insert("NXRouteConnectionType", QVariant::fromValue<Qt::ConnectionType>(connectionType));
    d->_routeQueue.enqueue(saveData);
    return NXNavigationRouterType::Success;
}

void NXNavigationRouter::navigationRouteBack()
{
    Q_D(NXNavigationRouter);
    int routeQueueCount = d->_routeQueue.count();
    if (routeQueueCount > 0)
    {
        if (routeQueueCount == 1)
        {
            Q_EMIT navigationRouterStateChanged(false);
        }
        QVariantMap routeSaveData = d->_routeQueue.last();
        d->_routeQueue.removeLast();
        QObject* routeObject = routeSaveData.value("NXRouteObject").value<QObject*>();
        QString routeFunctionName = routeSaveData.value("NXRouteFunctionName").toString();
        QVariantMap routeData = routeSaveData.value("NXRouteData").toMap();
        Qt::ConnectionType connectionType = routeSaveData.value("NXRouteConnectionType").value<Qt::ConnectionType>();
        QMetaObject::invokeMethod(routeObject, routeFunctionName.toLocal8Bit().constData(), connectionType, Q_ARG(QVariantMap, routeData));
    }
}
