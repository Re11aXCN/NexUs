#ifndef NXACRYLICURLCARDPRIVATE_H
#define NXACRYLICURLCARDPRIVATE_H

#include <QMap>
#include <QObject>
#include <QPixmap>

#include "NXDef.h"
#include "stdafx.h"
class NXAcrylicUrlCard;
class NXAcrylicUrlCardPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXAcrylicUrlCard)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(qreal, MainOpacity)
    Q_PROPERTY_CREATE_D(qreal, NoiseOpacity)
    Q_PROPERTY_CREATE_D(int, BrushAlpha)
    Q_PROPERTY_CREATE_D(QString, Title);
    Q_PROPERTY_CREATE_D(QString, SubTitle);
    Q_PROPERTY_CREATE_D(int, TitlePixelSize);
    Q_PROPERTY_CREATE_D(int, SubTitlePixelSize);
    Q_PROPERTY_CREATE_D(int, TitleSpacing);
    Q_PROPERTY_CREATE_D(int, SubTitleSpacing);
    Q_PROPERTY_CREATE_D(QPixmap, CardPixmap);
    Q_PROPERTY_CREATE_D(QSize, CardPixmapSize);
    Q_PROPERTY_CREATE_D(int, CardPixmapBorderRadius)
    Q_PROPERTY_CREATE_D(NXCardPixType::PixMode, CardPixMode);
    Q_PROPERTY_CREATE_D(QString, Url);

public:
    explicit NXAcrylicUrlCardPrivate(QObject* parent = nullptr);
    ~NXAcrylicUrlCardPrivate();

private:
    QPixmap _noisePix;
    NXThemeType::ThemeMode _themeMode;
    qreal _borderWidth{1.5};
};

#endif // NXACRYLICURLCARDPRIVATE_H
