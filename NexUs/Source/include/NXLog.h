#ifndef NXLOG_H
#define NXLOG_H

#include <QObject>

#include "singleton.h"
#include "stdafx.h"

class NXLogPrivate;
class NX_EXPORT NXLog : public QObject
{
    Q_OBJECT
    Q_Q_CREATE(NXLog)
    Q_PROPERTY_CREATE_Q_H(QString, LogSavePath)
    Q_PROPERTY_CREATE_Q_H(QString, LogFileName)
    Q_PROPERTY_CREATE_Q_H(bool, IsLogFileNameWithTime)
    Q_SINGLETON_CREATE_H(NXLog);

private:
    explicit NXLog(QObject* parent = nullptr);
    ~NXLog();

public:
    void initMessageLog(bool isEnable);
Q_SIGNALS:
    void logMessage(const QString& log);
};

#endif // NXLOG_H
