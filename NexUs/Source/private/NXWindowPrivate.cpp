#include "NXWindowPrivate.h"

#include "NXAppBarPrivate.h"
#include "NXApplication.h"
#include "DeveloperComponents/NXCentralStackedWidget.h"
#include "NXNavigationBar.h"
#include "NXTheme.h"
#include "DeveloperComponents/NXThemeAnimationWidget.h"
#include "NXWindow.h"
#include <QApplication>
#include <QPropertyAnimation>
#include <QTimer>
#include <QVBoxLayout>
#include <QtMath>

NXWindowPrivate::NXWindowPrivate(QObject* parent)
    : QObject{parent}
{
}

NXWindowPrivate::~NXWindowPrivate()
{
}

void NXWindowPrivate::onNavigationButtonClicked()
{
    if (_isWMClickedAnimationFinished)
    {
        _isNavigationDisplayModeChanged = false;
        _resetWindowLayout(true);
        _navigationBar->setIsTransparent(false);
        _navigationBar->setDisplayMode(NXNavigationType::Maximal, false);
        _navigationBar->move(-_navigationBar->width(), _navigationBar->pos().y());
        _navigationBar->resize(_navigationBar->width(), _centerStackedWidget->height() + 1);
        QPropertyAnimation* navigationMoveAnimation = new QPropertyAnimation(_navigationBar, "pos");
        QObject::connect(navigationMoveAnimation, &QPropertyAnimation::finished, this, [=]() {
            _isNavigationBarExpanded = true;
        });
        navigationMoveAnimation->setEasingCurve(QEasingCurve::InOutSine);
        navigationMoveAnimation->setDuration(300);
        navigationMoveAnimation->setStartValue(_navigationBar->pos());
        navigationMoveAnimation->setEndValue(QPoint(0, 0));
        navigationMoveAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        _isWMClickedAnimationFinished = false;
    }
}

void NXWindowPrivate::onWMWindowClickedEvent(QVariantMap data)
{
    NXAppBarType::WMMouseActionType actionType = data.value("WMClickType").value<NXAppBarType::WMMouseActionType>();
    if (actionType == NXAppBarType::WMLBUTTONDBLCLK || actionType == NXAppBarType::WMLBUTTONUP || actionType == NXAppBarType::WMNCLBUTTONDOWN)
    {
        if (NXApplication::containsCursorToItem(_navigationBar))
        {
            return;
        }
        if (_isNavigationBarExpanded)
        {
            QPropertyAnimation* navigationMoveAnimation = new QPropertyAnimation(_navigationBar, "pos");
            QObject::connect(navigationMoveAnimation, &QPropertyAnimation::valueChanged, this, [=]() {
                if (_isNavigationDisplayModeChanged)
                {
                    _isWMClickedAnimationFinished = true;
                    _resetWindowLayout(false);
                    navigationMoveAnimation->deleteLater();
                }
                });
            QObject::connect(navigationMoveAnimation, &QPropertyAnimation::finished, this, [=]() {
                if (!_isNavigationDisplayModeChanged)
                {
                    _navigationBar->setDisplayMode(NXNavigationType::Minimal, false);
                    _resetWindowLayout(false);
                }
                _isWMClickedAnimationFinished = true;
            });
            navigationMoveAnimation->setEasingCurve(QEasingCurve::InOutSine);
            navigationMoveAnimation->setDuration(300);
            navigationMoveAnimation->setStartValue(_navigationBar->pos());
            navigationMoveAnimation->setEndValue(QPoint(-_navigationBar->width(), 0));
            navigationMoveAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            _isNavigationBarExpanded = false;
        }
    }
}

void NXWindowPrivate::onxThemeReadyChange()
{
    Q_Q(NXWindow);
    _appBar->setIsOnlyAllowMinAndClose(true);
    if (!_animationWidget)
    {
        QPoint centerPos = q->mapFromGlobal(QCursor::pos());
        _animationWidget = new NXThemeAnimationWidget(q);
        QObject::connect(_animationWidget, &NXThemeAnimationWidget::animationFinished, this, [=]() {
            _appBar->setIsOnlyAllowMinAndClose(false);
            _animationWidget = nullptr;
        });
        _animationWidget->move(0, 0);
        _animationWidget->setOldWindowBackground(q->grab(q->rect()).toImage());
        if (nxTheme->getThemeMode() == NXThemeType::Light)
        {
            nxTheme->setThemeMode(NXThemeType::Dark);
        }
        else
        {
            nxTheme->setThemeMode(NXThemeType::Light);
        }
        _animationWidget->setNewWindowBackground(q->grab(q->rect()).toImage());
        _animationWidget->setCenter(centerPos);
        qreal topLeftDis = _distance(centerPos, QPoint(0, 0));
        qreal topRightDis = _distance(centerPos, QPoint(q->width(), 0));
        qreal bottomLeftDis = _distance(centerPos, QPoint(0, q->height()));
        qreal bottomRightDis = _distance(centerPos, QPoint(q->width(), q->height()));
        QList<qreal> disList{topLeftDis, topRightDis, bottomLeftDis, bottomRightDis};
        std::sort(disList.begin(), disList.end());
        _animationWidget->setEndRadius(disList[3]);
        _animationWidget->resize(q->width(), q->height());
        _animationWidget->startAnimation(_pThemeChangeTime);
        _animationWidget->show();
    }
}

void NXWindowPrivate::onDisplayModeChanged()
{
    _currentNavigationBarDisplayMode = _pNavigationBarDisplayMode;
    switch (_pNavigationBarDisplayMode)
    {
    case NXNavigationType::Auto:
    {
        _appBar->setWindowButtonFlag(NXAppBarType::NavigationButtonHint, false);
        _doNavigationDisplayModeChange();
        break;
    }
    case NXNavigationType::Minimal:
    {
        _navigationBar->setDisplayMode(NXNavigationType::Minimal, true);
        _appBar->setWindowButtonFlag(NXAppBarType::NavigationButtonHint);
        break;
    }
    case NXNavigationType::Compact:
    {
        _navigationBar->setDisplayMode(NXNavigationType::Compact, true);
        _appBar->setWindowButtonFlag(NXAppBarType::NavigationButtonHint, false);
        break;
    }
    case NXNavigationType::Maximal:
    {
        _navigationBar->setDisplayMode(NXNavigationType::Maximal, true);
        _appBar->setWindowButtonFlag(NXAppBarType::NavigationButtonHint, false);
        break;
    }
    }
}

void NXWindowPrivate::onxThemeModeChanged(NXThemeType::ThemeMode themeMode)
{
    Q_Q(NXWindow);
    _themeMode = themeMode;
    QPalette palette = q->palette();
    if (!nxApp->getIsEnableMica())
    {
        QPalette palette = q->palette();
        palette.setBrush(QPalette::Window, NXThemeColor(_themeMode, WindowBase));
        q->setPalette(palette);
    }
}

void NXWindowPrivate::onNavigationNodeClicked(NXNavigationType::NavigationNodeType nodeType, QString nodeKey)
{
    QWidget* page = _routeMap.value(nodeKey);
    if (!page)
    {
        return;
    }
    int nodeIndex = _centerStackedWidget->indexOf(page);
    if (_navigationTargetIndex == nodeIndex || _centerStackedWidget->count() <= nodeIndex)
    {
        return;
    }
    _navigationTargetIndex = nodeIndex;
    QTimer::singleShot(180, this, [=]() {
        QWidget* currentWidget = _centerStackedWidget->widget(nodeIndex);
        _centerStackedWidget->setCurrentIndex(nodeIndex);
        QPropertyAnimation* currentWidgetAnimation = new QPropertyAnimation(currentWidget, "pos");
        currentWidgetAnimation->setEasingCurve(QEasingCurve::OutCubic);
        currentWidgetAnimation->setDuration(300);
        QPoint currentWidgetPos = currentWidget->pos();
        currentWidgetAnimation->setEndValue(currentWidgetPos);
        currentWidgetPos.setY(currentWidgetPos.y() + 80);
        currentWidgetAnimation->setStartValue(currentWidgetPos);
        currentWidgetAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        });
}

void NXWindowPrivate::onNavigationNodeAdded(NXNavigationType::NavigationNodeType nodeType, QString nodeKey, QWidget* page)
{
    if (nodeType == NXNavigationType::PageNode)
    {
        _routeMap.insert(nodeKey, page);
        _centerStackedWidget->addWidget(page);
    }
    else
    {
        _routeMap.insert(nodeKey, page);
        if (page)
        {
            _centerStackedWidget->addWidget(page);
        }
    }
}

void NXWindowPrivate::onNavigationNodeRemoved(NXNavigationType::NavigationNodeType nodeType, QString nodeKey)
{
    Q_Q(NXWindow);
    if (!_routeMap.contains(nodeKey))
    {
        return;
    }
    QWidget* page = _routeMap.value(nodeKey);
    _routeMap.remove(nodeKey);
    _centerStackedWidget->removeWidget(page);
    QWidget* currentWidget = _centerStackedWidget->currentWidget();
    if (currentWidget)
    {
        q->navigation(currentWidget->property("NXPageKey").toString());
    }
}

qreal NXWindowPrivate::_distance(QPoint point1, QPoint point2)
{
    return std::sqrt((point1.x() - point2.x()) * (point1.x() - point2.x()) + (point1.y() - point2.y()) * (point1.y() - point2.y()));
}

void NXWindowPrivate::_resetWindowLayout(bool isAnimation)
{
    if (isAnimation)
    {
        while (_centerLayout->count() > 0)
        {
            _centerLayout->takeAt(0);
        }
    }
    else
    {
        if (_centerLayout->count() == 0)
        {
            _navigationBar->setIsTransparent(true);
            _navigationBar->setDisplayMode(NXNavigationType::Minimal, false);
            _centerLayout->addWidget(_navigationBar);
            _centerLayout->addWidget(_centerStackedWidget);
        }
    }
}

void NXWindowPrivate::_doNavigationDisplayModeChange()
{
    Q_Q(NXWindow);
    if (_isWindowClosing || !_isNavigationEnable || !_isInitFinished)
    {
        return;
    }
    if (_pNavigationBarDisplayMode == NXNavigationType::Minimal)
    {
        _resetWindowLayout(false);
    }
    if (_pNavigationBarDisplayMode == NXNavigationType::Auto)
    {
        _isNavigationDisplayModeChanged = true;
        _resetWindowLayout(false);
        int width = q->centralWidget()->width();
        if (width >= 850 && _currentNavigationBarDisplayMode != NXNavigationType::Maximal)
        {
            _navigationBar->setDisplayMode(NXNavigationType::Maximal);
            _currentNavigationBarDisplayMode = NXNavigationType::Maximal;
            _appBar->setWindowButtonFlag(NXAppBarType::NavigationButtonHint, false);
        }
        else if (width >= 550 && width < 850 && _currentNavigationBarDisplayMode != NXNavigationType::Compact)
        {
            _navigationBar->setDisplayMode(NXNavigationType::Compact);
            _currentNavigationBarDisplayMode = NXNavigationType::Compact;
            _appBar->setWindowButtonFlag(NXAppBarType::NavigationButtonHint, false);
        }
        else if (width < 550 && _currentNavigationBarDisplayMode != NXNavigationType::Minimal)
        {
            _navigationBar->setDisplayMode(NXNavigationType::Minimal);
            _currentNavigationBarDisplayMode = NXNavigationType::Minimal;
            _appBar->setWindowButtonFlag(NXAppBarType::NavigationButtonHint);
        }
        _isNavigationBarExpanded = false;
    }
}