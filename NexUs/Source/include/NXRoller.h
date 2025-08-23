#ifndef NXFRAMEWORK_NEXUS_INCLUDE_NXROLLER_H_
#define NXFRAMEWORK_NEXUS_INCLUDE_NXROLLER_H_

#include "NXProperty.h"
#include <QWidget>

class NXRollerPrivate;
class NX_EXPORT NXRoller : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(NXRoller)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(int, ItemHeight)
    Q_PROPERTY_CREATE_Q_H(int, MaxVisibleItems)
    Q_PROPERTY_CREATE_Q_H(int, CurrentIndex)
    Q_PROPERTY_CREATE_Q_H(QStringList, ItemList)
public:
    explicit NXRoller(QWidget* parent = nullptr);
    ~NXRoller() override;
    QString getCurrentData() const;

protected:
    virtual void wheelEvent(QWheelEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void leaveEvent(QEvent* event);
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif //NXFRAMEWORK_NEXUS_INCLUDE_NXROLLER_H_
