#ifndef NXNAVIGATIONROUTER_H
#define NXNAVIGATIONROUTER_H

#include <QObject>
#include <QVariantMap>

#include "NXDef.h"
#include "singleton.h"

class NXNavigationRouterPrivate;
class NX_EXPORT NXNavigationRouter : public QObject
{
    Q_OBJECT
    Q_Q_CREATE(NXNavigationRouter)
    Q_SINGLETON_CREATE_H(NXNavigationRouter)
    Q_PROPERTY_CREATE_Q_H(int, MaxRouteCount)
private:
    explicit NXNavigationRouter(QObject* parent = nullptr);
    ~NXNavigationRouter();

Q_SIGNALS:
    Q_SIGNAL void navigationRouterStateChanged(bool state);

public:
    NXNavigationRouterType::NavigationRouteType navigationRoute(QObject* routeObject, QString routeFunctionName, const QVariantMap& routeData = {}, Qt::ConnectionType connectionType = Qt::AutoConnection);
    void navigationRouteBack();
};

#endif // NXNAVIGATIONROUTER_H
