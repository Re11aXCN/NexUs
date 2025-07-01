﻿#ifndef NXTABLEVIEWPRIVATE_H
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
	Q_PRIVATE_CREATE(NXTableViewStyle*, TableViewStyle)
	Q_PRIVATE_CREATE(bool, DrawSelectionBackground)

public:
    explicit NXTableViewPrivate(QObject* parent = nullptr);
    ~NXTableViewPrivate();
};

#endif // NXTABLEVIEWPRIVATE_H
