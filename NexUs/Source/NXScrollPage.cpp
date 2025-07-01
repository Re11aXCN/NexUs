#include "NXScrollPage.h"

#include <QHBoxLayout>
#include <QPainter>
#include <QResizeEvent>
#include <QScrollArea>
#include <QScrollBar>
#include <QScroller>
#include <QStackedWidget>
#include <QVBoxLayout>

#include "NXBreadcrumbBar.h"
#include "NXNavigationRouter.h"
#include "NXScrollArea.h"
#include "NXScrollBar.h"
#include "private/NXScrollPagePrivate.h"
NXScrollPage::NXScrollPage(QWidget* parent)
    : QWidget(parent), d_ptr(new NXScrollPagePrivate())
{
    Q_D(NXScrollPage);
    setProperty("NXBaseClassName", "NXScrollPage");
    d->q_ptr = this;
    d->_pCustomWidget = nullptr;
    d->_breadcrumbBar = new NXBreadcrumbBar(this);
    d->_breadcrumbBar->setTextPixelSize(28);
    QObject::connect(d->_breadcrumbBar, &NXBreadcrumbBar::breadcrumbClicked, this, [=](QString breadcrumb, QStringList lastBreadcrumbList) {
        if (d->_centralWidgetMap.contains(breadcrumb))
        {
            int widgetIndex = d->_centralWidgetMap.value(breadcrumb);
            d->_switchCentralStackIndex(widgetIndex, d->_navigationTargetIndex);
            d->_navigationTargetIndex = widgetIndex;
            QVariantMap routeData = QVariantMap();
            routeData.insert("NXScrollPageCheckSumKey", "BreadcrumbClicked");
            routeData.insert("LastBreadcrumbList", lastBreadcrumbList);
            NXNavigationRouter::getInstance()->navigationRoute(d,"onNavigationRouteBack", routeData);
        } });
    d->_pageTitlNXyout = new QHBoxLayout();
    d->_pageTitlNXyout->setContentsMargins(0, 0, 0, 0);
    d->_pageTitlNXyout->addWidget(d->_breadcrumbBar);

    d->_centralStackedWidget = new QStackedWidget(this);
    d->_centralStackedWidget->setContentsMargins(0, 0, 0, 0);

    d->_mainLayout = new QVBoxLayout(this);
    d->_mainLayout->setContentsMargins(0, 0, 0, 0);
    d->_mainLayout->addLayout(d->_pageTitlNXyout);
    d->_mainLayout->addWidget(d->_centralStackedWidget);
    setContentsMargins(20, 20, 0, 0);
}

NXScrollPage::~NXScrollPage()
{
}

void NXScrollPage::addCentralWidget(QWidget* centralWidget, bool isWidgetResizeable, bool isVerticalGrabGesture, qreal mousePressEventDNXy, Qt::ScrollBarPolicy vScrollBarPolicy, Qt::ScrollBarPolicy hScrollBarPolicy)
{
    Q_D(NXScrollPage);
    if (!centralWidget)
    {
        return;
    }
    if (centralWidget->windowTitle().isEmpty())
    {
        centralWidget->setWindowTitle(QString("Page_%1").arg(d->_centralStackedWidget->count()));
    }
    if (d->_centralStackedWidget->count() == 0)
    {
        d->_breadcrumbBar->appendBreadcrumb(centralWidget->windowTitle());
    }
    NXScrollArea* scrollArea = new NXScrollArea(this);
    scrollArea->setMouseTracking(true);
    scrollArea->setIsAnimation(Qt::Vertical, true);
    scrollArea->setIsAnimation(Qt::Horizontal, true);
    scrollArea->setWidgetResizable(isWidgetResizeable);
    scrollArea->setIsGrabGesture(isVerticalGrabGesture, mousePressEventDNXy);
    scrollArea->setIsOverShoot(Qt::Vertical, true);
    scrollArea->setIsOverShoot(Qt::Horizontal, true);
    scrollArea->setVerticalScrollBarPolicy(vScrollBarPolicy);
    scrollArea->setHorizontalScrollBarPolicy(hScrollBarPolicy);
    //if (vScrollBarPolicy != Qt::ScrollBarAlwaysOff) {
    //    NXScrollBar* floatVScrollBar = new NXScrollBar(scrollArea->verticalScrollBar(), scrollArea);
    //    floatVScrollBar->setIsAnimation(true);
    //}
    //if (hScrollBarPolicy != Qt::ScrollBarAlwaysOff) {
    //    NXScrollBar* floatHScrollBar = new NXScrollBar(scrollArea->horizontalScrollBar(), scrollArea);
    //    floatHScrollBar->setIsAnimation(true);
    //}
    scrollArea->setWidget(centralWidget);
    centralWidget->setObjectName("NXScrollPage_CentralPage");
    centralWidget->setStyleSheet("#NXScrollPage_CentralPage{background-color:transparent;}");
    d->_centralWidgetMap.insert(centralWidget->windowTitle(), d->_centralStackedWidget->count());
    d->_centralStackedWidget->addWidget(scrollArea);
}

void NXScrollPage::setCustomWidget(QWidget* widget)
{
    Q_D(NXScrollPage);
    if (!widget || widget == this)
    {
        return;
    }
    if (d->_pCustomWidget)
    {
        d->_mainLayout->removeWidget(d->_pCustomWidget);
    }
    d->_mainLayout->insertWidget(1, widget);
    d->_pCustomWidget = widget;
    Q_EMIT pCustomWidgetChanged();
}

QWidget* NXScrollPage::getCustomWidget() const
{
    Q_D(const NXScrollPage);
    return d->_pCustomWidget;
}

void NXScrollPage::navigation(int widgetIndex, bool isLogRoute)
{
    Q_D(NXScrollPage);
    if (widgetIndex >= d->_centralStackedWidget->count() || d->_navigationTargetIndex == widgetIndex)
    {
        return;
    }
    d->_switchCentralStackIndex(widgetIndex, d->_navigationTargetIndex);
    d->_navigationTargetIndex = widgetIndex;
    if (isLogRoute)
    {
        QVariantMap routeData = QVariantMap();
        routeData.insert("NXScrollPageCheckSumKey", "Navigation");
        QStringList breadcrumbList = d->_breadcrumbBar->getBreadcrumbList();
        routeData.insert("NXPageTitle", breadcrumbList.last());
        NXNavigationRouter::getInstance()->navigationRoute(d, "onNavigationRouteBack", routeData);
    }
    d->_breadcrumbBar->appendBreadcrumb(d->_centralWidgetMap.key(widgetIndex));
}

void NXScrollPage::setPageTitleSpacing(int spacing)
{
    Q_D(NXScrollPage);
    d->_pageTitlNXyout->takeAt(0);
    d->_pageTitlNXyout->addSpacing(spacing);
    d->_pageTitlNXyout->addWidget(d->_breadcrumbBar);
}

int NXScrollPage::getPageTitleSpacing() const
{
    return d_ptr->_pageTitleSpacing;
}

void NXScrollPage::setTitleVisible(bool isVisible)
{
    Q_D(NXScrollPage);
    d->_breadcrumbBar->setVisible(isVisible);
}
