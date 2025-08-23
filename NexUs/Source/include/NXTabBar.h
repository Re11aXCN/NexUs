#ifndef NXTABBAR_H
#define NXTABBAR_H

#include <QDrag>
#include <QTabBar>

#include "NXProperty.h"
class NXTabBarPrivate;
class NX_EXPORT NXTabBar : public QTabBar
{
    Q_OBJECT
    Q_Q_CREATE(NXTabBar)
public:
    explicit NXTabBar(QWidget* parent = nullptr);
    ~NXTabBar();
Q_SIGNALS:
    Q_SIGNAL void tabBarPress(int index);
    Q_SIGNAL void tabDragCreate(QDrag* drag);
    Q_SIGNAL void tabDragDrop(const QMimeData* mimeData);

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void dragEnterEvent(QDragEnterEvent* event) override;
    virtual void dropEvent(QDropEvent* event) override;
};

#endif // NXTABBAR_H
