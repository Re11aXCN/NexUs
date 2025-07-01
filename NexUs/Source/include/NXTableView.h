#ifndef NXTABLEVIEW_H
#define NXTABLEVIEW_H

#include <QTableView>

#include "stdafx.h"
struct NXAdjustParams {
    int left;
    int top;
    int right;
    int bottom;
    NXAdjustParams(int l = 0, int t = 0, int r = 0, int b = 0) : left(l), top(t), right(r), bottom(b) {}
};
class NXModelIndexWidgetPrivate;
class NX_EXPORT NXModelIndexWidget : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(NXModelIndexWidget)
public:
    explicit NXModelIndexWidget(const QModelIndex& index, QWidget* parent = nullptr);
    ~NXModelIndexWidget();
    const QModelIndex& index() const;
    void setIndex(const QModelIndex& index);
Q_SIGNALS:
    Q_SIGNAL void updateIndex(const QModelIndex& index);
    Q_SIGNAL void entered();
    Q_SIGNAL void leaved();
protected:
    virtual void enterEvent(QEnterEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event);
};
class NXTableViewPrivate;
class NX_EXPORT NXTableView : public QTableView
{
    Q_OBJECT
    Q_Q_CREATE(NXTableView)
    Q_PROPERTY_CREATE_Q_H(int, HeaderMargin)
    Q_PROPERTY_CREATE_Q_H(bool, DrawSelectionBackground)

public:
    explicit NXTableView(QWidget* parent = nullptr);
    ~NXTableView();
    void setIndexWidget(const QModelIndex& index, QWidget* widget);
    QWidget* indexWidget(const QModelIndex& index) const;

    QRect getAlignLeft(const QRect& cellRect, const QSize& iconSize)  const;
    QRect getAlignCenter(const QRect& cellRect, const QSize& iconSize)  const;
    QRect getAlignRight(const QRect& cellRect, const QSize& iconSize)  const;
    void setIconSize(const QSize& size);
    void setHeaderFontSize(int size);
    void setModelFontSize(int size);
    void setTableFontSize(int size);
    void setAdjustTextRect(QMap<int, NXAdjustParams> adjustParamsMap);
    void setCurrentHoverRow(int row);

Q_SIGNALS:
    Q_SIGNAL void tableViewShow();
    Q_SIGNAL void tableViewHide();
    Q_SIGNAL void currentHoverRowChanged(int row);
    Q_SIGNAL void currentHoverColumnChanged(int column);
    Q_SIGNAL void mouseRelease(const QModelIndex& index);
    Q_SIGNAL void mouseMoveOver(const QModelIndex& index);
    Q_SIGNAL void mouseMoveLeave(const QModelIndex& index);

protected:
    virtual void showEvent(QShowEvent* event) override;
    virtual void hideEvent(QHideEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;
private:
    void enterIndexWidgetUpdateHoverIndex(const QModelIndex& index);
    friend class NXModelIndexWidget;
};

#endif // NXTABLEVIEW_H
