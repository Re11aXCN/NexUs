#ifndef NXMICABASEINITOBJECT_H
#define NXMICABASEINITOBJECT_H

#include <QObject>
#include <QImage>
class NXApplicationPrivate;
class NXMicaBaseInitObject : public QObject
{
    Q_OBJECT
public:
    explicit NXMicaBaseInitObject(NXApplicationPrivate* appPrivate, QObject* parent = nullptr);
    ~NXMicaBaseInitObject();
    Q_SLOT void onInitMicaBase(QImage img);
Q_SIGNALS:
    Q_SIGNAL void initFinished();

private:
    NXApplicationPrivate* _appPrivate{ nullptr };
};

#endif // NXMICABASEINITOBJECT_H
