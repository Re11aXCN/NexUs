#include "NXCustomTabWidget.h"

#include "NXAppBar.h"
#include "NXTabBar.h"
#include "NXTabWidget.h"
#include "private/NXTabWidgetPrivate.h"
#include <QDebug>
#include <QMimeData>
#include <QVBoxLayout>
#include <QVariant>
NXCustomTabWidget::NXCustomTabWidget(QWidget* parent)
    : NXCustomWidget(parent)
{
    resize(700, 500);
    setWindowTitle("");
#ifndef Q_OS_WIN
    setAttribute(Qt::WA_Hover);
#endif
    setWindowIcon(QIcon());
    _customTabWidget = new NXTabWidget(this);
    _customTabWidget->setIsTabTransparent(true);
    _customTabWidget->setObjectName("NXCustomTabWidget");
    QTabBar* originTabBar = _customTabWidget->tabBar();
    originTabBar->hide();
    _customTabBar = new NXTabBar(this);
    _customTabBar->setObjectName("NXCustomTabBar");
    QObject::connect(_customTabBar, &NXTabBar::tabMoved, this, [=](int from, int to) {
        _customTabWidget->tabBar()->moveTab(from, to);
        });
    QObject::connect(_customTabBar, &NXTabBar::currentChanged, this, [=](int index) {
        _customTabWidget->setCurrentIndex(index);
        });
    QObject::connect(_customTabWidget, &NXTabWidget::currentChanged, this, [=](int index) {
        if (index == -1)
        {
            close();
        }
        });
    QObject::connect(_customTabBar, &NXTabBar::tabCloseRequested, originTabBar, &QTabBar::tabCloseRequested);

    _customTabWidget->d_ptr->_customTabBar = _customTabBar;
    QObject::connect(_customTabBar, &NXTabBar::tabDragCreate, _customTabWidget->d_func(), &NXTabWidgetPrivate::onTabDragCreate);
    QObject::connect(_customTabBar, &NXTabBar::tabDragDrop, _customTabWidget->d_func(), &NXTabWidgetPrivate::onTabDragDrop);

    QWidget* customWidget = new QWidget(this);
    QVBoxLayout* customLayout = new QVBoxLayout(customWidget);
    customLayout->setContentsMargins(10, 0, 10, 0);
    customLayout->addStretch();
    customLayout->addWidget(_customTabBar);
    _appBar->setCustomWidget(NXAppBarType::LeftArea, customWidget);
    setCentralWidget(_customTabWidget);
}

NXCustomTabWidget::~NXCustomTabWidget()
{
    while (_customTabWidget->count() > 0)
    {
        QWidget* closeWidget = _customTabWidget->widget(0);
        NXTabWidget* originTabWidget = closeWidget->property("NXOriginTabWidget").value<NXTabWidget*>();

        if (originTabWidget)
        {
            closeWidget->setProperty("CurrentCustomBar", QVariant::fromValue<NXTabBar*>(nullptr));
            originTabWidget->addTab(closeWidget, _customTabWidget->tabIcon(0), _customTabWidget->tabText(0));
        }
        else
        {
            _customTabWidget->removeTab(0);
        }
    }
}

void NXCustomTabWidget::addTab(QWidget* widget, QIcon tabIcon, const QString& tabTitle)
{
    _customTabBar->addTab(tabIcon, tabTitle);
    _customTabWidget->addTab(widget, tabIcon, tabTitle);
}

NXTabBar* NXCustomTabWidget::getCustomTabBar() const
{
    return _customTabBar;
}

NXTabWidget* NXCustomTabWidget::getCustomTabWidget() const
{
    return _customTabWidget;
}
