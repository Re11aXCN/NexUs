#ifndef NXTREEVIEW_H
#define NXTREEVIEW_H

#include <QTreeView>

#include "stdafx.h"

class NXTreeViewPrivate;
class NX_EXPORT NXTreeView : public QTreeView
{
    Q_OBJECT
    Q_Q_CREATE(NXTreeView)
    Q_PROPERTY_CREATE_Q_H(int, ItemHeight)
    Q_PROPERTY_CREATE_Q_H(int, HeaderMargin)
public:
    explicit NXTreeView(QWidget* parent = nullptr);
    ~NXTreeView();
};

#endif // NXTREEVIEW_H
