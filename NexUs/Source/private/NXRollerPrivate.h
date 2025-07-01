#ifndef NXFRAMEWORK_NEXUS_PRIVATE_NXROLLERPRIVATE_H_
#define NXFRAMEWORK_NEXUS_PRIVATE_NXROLLERPRIVATE_H_

#include "NXDef.h"
#include "stdafx.h"
#include <QObject>
#include <QPoint>

class NXRoller;
class QPropertyAnimation;
class NXRollerPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXRoller)
    Q_PROPERTY_CREATE(qreal, ScrollOffset)
    Q_PROPERTY_CREATE_D(QStringList, ItemList)
	Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE_D(int, ItemHeight)
    Q_PROPERTY_CREATE_D(int, MaxVisibleItems)
    Q_PROPERTY_CREATE_D(int, CurrentIndex)
public:
    explicit NXRollerPrivate(QObject* parent = nullptr);
    ~NXRollerPrivate() override;

private:
    NXThemeType::ThemeMode _themeMode;
    int _penBorderWidth{1};
    int _indicatorMargin{3};
    qreal _targetScrollOffset{0};
    QPoint _mousePoint;
	QPropertyAnimation* _scrollAnimation{ nullptr };

    void _scroll(int delta);
};

#endif //NXFRAMEWORK_NEXUS_PRIVATE_NXROLLERPRIVATE_H_
