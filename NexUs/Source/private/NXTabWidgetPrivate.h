#ifndef NXTABWIDGETPRIVATE_H
#define NXTABWIDGETPRIVATE_H

#include <QDrag>
#include <QObject>
#include <QPixmap>

#include "NXProperty.h"
class NXTabWidget;
class NXTabBar;
class NXCustomTabWidget;
class NXTabWidgetPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXTabWidget)
public:
    explicit NXTabWidgetPrivate(QObject* parent = nullptr);
    ~NXTabWidgetPrivate() override;

    Q_SLOT void onTabDragCreate(QDrag* drag);
    Q_SLOT void onTabDragDrop(const QMimeData* mimeData);
    Q_SLOT void onTabCloseRequested(int index);

private:
    NXTabBar* _customTabBar{nullptr};
    QList<QPointer<QWidget>> _allTabWidgetList;

    friend class NXCustomTabWidget;
};

#endif // NXTABWIDGETPRIVATE_H
