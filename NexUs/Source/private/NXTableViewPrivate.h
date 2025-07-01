#ifndef NXTABLEVIEWPRIVATE_H
#define NXTABLEVIEWPRIVATE_H

#include <QObject>
#include <QModelIndex>

#include "stdafx.h"

class NXModelIndexWidget;
class NXModelIndexWidgetPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXModelIndexWidget)
public:
    explicit NXModelIndexWidgetPrivate(QObject* parent = nullptr);
    ~NXModelIndexWidgetPrivate();
private:
    QModelIndex _index;
};

class NXTableView;
class NXTableViewStyle;
class NXTableViewPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXTableView)
    Q_PRIVATE_CREATE(std::shared_ptr<NXTableViewStyle>, TableViewStyle)

public:
    explicit NXTableViewPrivate(QObject* parent = nullptr);
    ~NXTableViewPrivate();
    bool _pDrawSelectionBackground{ true };
};

#endif // NXTABLEVIEWPRIVATE_H
