#ifndef NXEVENTBUS_H
#define NXEVENTBUS_H

#include <QObject>
#include <QVariantMap>

#include "NXDef.h"
#include "singleton.h"
class NXEventPrivate;
class NX_EXPORT NXEvent : public QObject
{
    Q_OBJECT
    Q_Q_CREATE(NXEvent)
    Q_PROPERTY_CREATE_Q_H(QString, EventName);
    Q_PROPERTY_CREATE_Q_H(QString, FunctionName);
    Q_PROPERTY_CREATE_Q_H(Qt::ConnectionType, ConnectionType);

public:
    explicit NXEvent(QObject* parent = nullptr);
    explicit NXEvent(const QString& eventName, const QString& functionName, QObject* parent = nullptr);
    ~NXEvent() override;
    NXEventBusType::EventBusReturnType registerAndInit();
};

class NXEventBusPrivate;
class NX_EXPORT NXEventBus : public QObject
{
    Q_OBJECT
    Q_Q_CREATE(NXEventBus)
    Q_SINGLETON_CREATE_H(NXEventBus);

private:
    explicit NXEventBus(QObject* parent = nullptr);
    ~NXEventBus() override;

public:
    NXEventBusType::EventBusReturnType post(const QString& eventName, const QVariantMap& data = {});
    QStringList getRegisteredEventsName() const;

private:
    friend class NXEvent;
};

#endif // NXEVENTBUS_H
