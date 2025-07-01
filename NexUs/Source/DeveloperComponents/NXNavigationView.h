#ifndef NXNAVIGATIONVIEW_H
#define NXNAVIGATIONVIEW_H

#include <QObject>
#include <QTreeView>

class NXScrollBar;
class NXNavigationStyle;
class NXToolTip;
class NXNavigationView : public QTreeView
{
    Q_OBJECT
public:
    explicit NXNavigationView(QWidget* parent = nullptr);
    ~NXNavigationView();
    void navigationNodeStateChange(QVariantMap data);

    Q_SLOT void onCustomContextMenuRequested(const QPoint& pos);
Q_SIGNALS:
    Q_SIGNAL void navigationClicked(const QModelIndex& index);
    Q_SIGNAL void navigationOpenNewWindow(QString nodeKey);

protected:    
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual bool eventFilter(QObject* watched, QEvent* event) override;

private:
    NXNavigationStyle* _navigationStyle{nullptr};
    NXToolTip* _compactToolTip{ nullptr };

};

#endif // NXNAVIGATIONVIEW_H
