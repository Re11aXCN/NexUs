#ifndef NXTABLEVIEW_H
#define NXTABLEVIEW_H

#include <QTableView>

#include "NXProperty.h"
struct NXAdjustParam {
    int x1;
    int y1;
    int x2;
    int y2;
    NXAdjustParam(int x1 = 0, int y1 = 0, int x2 = 0, int y2 = 0) : x1(x1), y1(y1), x2(x2), y2(y2) {}
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
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius)
    Q_PROPERTY_CREATE_Q_H(bool, IsDrawAlternateRowsEnabled)
    Q_PROPERTY_CREATE_Q_H(bool, IsSelectionEffectsEnabled)
public:
    explicit NXTableView(QWidget* parent = nullptr);
    ~NXTableView();
    void setIndexWidget(const QModelIndex& index, QWidget* widget);
    QWidget* indexWidget(const QModelIndex& index) const;

    QRect getAlignLeft(const QRect& cellRect, const QSize& iconSize)  const;
    QRect getAlignCenter(const QRect& cellRect, const QSize& iconSize)  const;
    QRect getAlignRight(const QRect& cellRect, const QSize& iconSize)  const;

    void setHeaderFontSize(int size);
    void setModelFontSize(int size);
    void setTableFontSize(int size);
    void setAdjustTextRect(const QMap<int, NXAdjustParam>& adjustParamsMap);
    void setHeaderAdjustParam(const QMap<int, NXAdjustParam>& adjustParamMap);

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
