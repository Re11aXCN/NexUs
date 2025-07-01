#ifndef NXAPPLICATION_H
#define NXAPPLICATION_H

#include <QIcon>
#include <QObject>

#include "singleton.h"
#include "stdafx.h"
#define nxApp NXApplication::getInstance()
class NXApplicationPrivate;
class NX_EXPORT NXApplication : public QObject
{
    Q_OBJECT
    Q_Q_CREATE(NXApplication)
    Q_SINGLETON_CREATE_H(NXApplication)
    Q_PROPERTY_CREATE_Q_H(bool, IsEnableMica)
    Q_PROPERTY_CREATE_Q_H(QString, MicaImagePath)
private:
    explicit NXApplication(QObject* parent = nullptr);
    ~NXApplication();
public:
    void init();
    void syncMica(QWidget* widget, bool isSync = true);
    static bool containsCursorToItem(QWidget* item);
};

#endif // NXAPPLICATION_H
