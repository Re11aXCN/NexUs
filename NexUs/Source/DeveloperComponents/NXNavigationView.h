#ifndef NXNAVIGATIONVIEW_H
#define NXNAVIGATIONVIEW_H

#include <QObject>
#include <QTreeView>
#include "stdafx.h"
class NXScrollBar;
class NXNavigationStyle;
class NXToolTip;
class NXNavigationView : public QTreeView
{
    Q_OBJECT
    Q_PROPERTY_CREATE(bool, IsLeftButtonPressedToggleNavigation)
public:
    explicit NXNavigationView(QWidget* parent = nullptr);
    ~NXNavigationView();
    void navigationNodeStateChange(QVariantMap data);
    void setNavigationNodeDragAndDropEnable(bool isEnable);
    QAbstractItemView::DropIndicatorPosition dropIndicatorPositionOverride() const;
    Q_SLOT void onCustomContextMenuRequested(const QPoint& pos);
Q_SIGNALS:
    Q_SIGNAL void navigationClicked(const QModelIndex& index);
    Q_SIGNAL void navigationOpenNewWindow(const QString& nodeKey);
    Q_SIGNAL void navigationCloseCurrentWindow(const QString& nodeKey);
protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void dragEnterEvent(QDragEnterEvent* event) override;
    virtual void dropEvent(QDropEvent* event) override;
    virtual void dragMoveEvent(QDragMoveEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
    virtual bool eventFilter(QObject* watched, QEvent* event) override;

private:
    QModelIndex _hoveredIndex; 
    NXNavigationStyle* _navigationStyle{nullptr};
    NXToolTip* _compactToolTip{ nullptr };
};

#endif // NXNAVIGATIONVIEW_H
