#ifndef NXSCROLLPAGEPRIVATE_H
#define NXSCROLLPAGEPRIVATE_H

#include <QMap>
#include <QObject>

#include "stdafx.h"
class NXScrollPage;
class NXScrollArea;
class QHBoxLayout;
class QVBoxLayout;
class NXText;
class QStackedWidget;
class NXBreadcrumbBar;
class NXScrollPagePrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXScrollPage)
    Q_PROPERTY_CREATE_D(QWidget*, CustomWidget)
public:
    explicit NXScrollPagePrivate(QObject* parent = nullptr);
    ~NXScrollPagePrivate();
    Q_INVOKABLE void onNavigationRouteBack(QVariantMap routeData);

private:
    QHBoxLayout* _pageTitlNXyout{nullptr};
    QVBoxLayout* _mainLayout{nullptr};
    QStackedWidget* _centralStackedWidget{nullptr};
    NXBreadcrumbBar* _breadcrumbBar{nullptr};
    QMap<QString, int> _centralWidgetMap;
    int _navigationTargetIndex{0};
    int _pageTitleSpacing{0};
    bool _isGrabGesture{false};
    void _switchCentralStackIndex(int targetIndex, int lastIndex);
};

#endif // NXSCROLLPAGEPRIVATE_H
