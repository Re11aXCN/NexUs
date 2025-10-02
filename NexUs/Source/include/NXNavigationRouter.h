#ifndef NXNAVIGATIONROUTER_H
#define NXNAVIGATIONROUTER_H

#include <QObject>
#include <QVariantMap>

#include "NXDef.h"
#include "singleton.h"

class NXNavigationRouterPrivate;
class NX_EXPORT NXNavigationRouter : public QObject
{
#pragma push_macro("DISABLE_COPY")
#undef DISABLE_COPY
#define DISABLE_COPY(Class)
    SINGLETON_CREATE_H(NXNavigationRouter)
#pragma pop_macro("DISABLE_COPY")
    Q_OBJECT
    Q_Q_CREATE(NXNavigationRouter)
    Q_PROPERTY_CREATE_Q_H(int, MaxRouteCount)
private:
    explicit NXNavigationRouter(QObject* parent = nullptr);
    ~NXNavigationRouter() override;

Q_SIGNALS:
    Q_SIGNAL void navigationRouterStateChanged(bool state);

public:
    NXNavigationRouterType::NavigationRouteType navigationRoute(QObject* routeObject, QString routeFunctionName, const QVariantMap& routeData = {}, Qt::ConnectionType connectionType = Qt::AutoConnection);
    void navigationRouteBack();
};

#endif // NXNAVIGATIONROUTER_H
