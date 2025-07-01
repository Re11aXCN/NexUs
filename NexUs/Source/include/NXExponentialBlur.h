#ifndef NXEXPONENTIALBLUR_H
#define NXEXPONENTIALBLUR_H

#include <QObject>

#include "singleton.h"
#include "stdafx.h"

class NXExponentialBlurPrivate;
class NX_EXPORT NXExponentialBlur : public QObject
{
    Q_OBJECT
    Q_SINGLETON_CREATE_H(NXExponentialBlur)
    Q_Q_CREATE(NXExponentialBlur)
private:
    explicit NXExponentialBlur(QObject* parent = nullptr);
    ~NXExponentialBlur();

public:
    static QPixmap doExponentialBlur(QImage img, const quint16& blurRadius);
};

#endif // NXEXPONENTIALBLUR_H
