#ifndef NXTABWIDGETPRIVATE_H
#define NXTABWIDGETPRIVATE_H

#include <QDrag>
#include <QObject>
#include <QPixmap>

#include "stdafx.h"
class NXTabWidget;
class NXTabBar;
class NXTabWidgetPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXTabWidget)
public:
    explicit NXTabWidgetPrivate(QObject* parent = nullptr);
    ~NXTabWidgetPrivate();
    Q_SLOT void onTabBarPress(int index);
    Q_SLOT void onTabDragCreate(QDrag* drag);
    Q_SLOT void onTabDragDrop(const QMimeData* mimeData);
    Q_SLOT void onTabCloseRequested(int index);

private:
    NXTabBar* _customTabBar{nullptr};
};

#endif // NXTABWIDGETPRIVATE_H
