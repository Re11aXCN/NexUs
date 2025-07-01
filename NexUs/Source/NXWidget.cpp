#include "NXWidget.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QPainter>
#include <QScreen>

#include "NXApplication.h"
#include "NXTheme.h"
#include "private/NXWidgetPrivate.h"
Q_TAKEOVER_NATIVEEVENT_CPP(NXWidget, d_func()->_appBar);
Q_PROPERTY_CREATE_Q_CPP(NXWidget, QStackedLayout*, ModuleStackedLayout)
NXWidget::NXWidget(QWidget* parent, const QString& moduleButton4JsonConfigPath)
    : QWidget{parent}, d_ptr(new NXWidgetPrivate())
{
    Q_D(NXWidget);
    d->q_ptr = this;
    resize(500, 500);
    setWindowTitle("NXWidget");
    setObjectName("NXWidget");
    d->_appBar = new NXAppBar(this, !moduleButton4JsonConfigPath.isEmpty());
    if (!moduleButton4JsonConfigPath.isEmpty()) {
       d->_pModuleStackedLayout = new QStackedLayout(this);
    }
    d->_appBar->setIsStayTop(false);
    d->_appBar->setWindowButtonFlags(NXAppBarType::StayTopButtonHint | NXAppBarType::MinimizeButtonHint | NXAppBarType::MaximizeButtonHint | NXAppBarType::CloseButtonHint);
    QObject::connect(d->_appBar, &NXAppBar::routeBackButtonClicked, this, &NXWidget::routeBackButtonClicked);
    QObject::connect(d->_appBar, &NXAppBar::navigationButtonClicked, this, &NXWidget::navigationButtonClicked);
    QObject::connect(d->_appBar, &NXAppBar::themeChangeButtonClicked, this, &NXWidget::themeChangeButtonClicked);
    QObject::connect(d->_appBar, &NXAppBar::closeButtonClicked, this, &NXWidget::closeButtonClicked);
    // 主题
    d->_themeMode = nxTheme->getThemeMode();
    QObject::connect(nxTheme, &NXTheme::themeModeChanged, this, [=](NXThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
        update();
    });
    d->_isEnableMica = nxApp->getIsEnableMica();
    QObject::connect(nxApp, &NXApplication::pIsEnableMicaChanged, this, [=]() {
        d->_isEnableMica = nxApp->getIsEnableMica();
        update();
        });
    nxApp->syncMica(this);
}

NXWidget::~NXWidget()
{
}

void NXWidget::setIsStayTop(bool isStayTop)
{
    Q_D(NXWidget);
    d->_appBar->setIsStayTop(isStayTop);
}

bool NXWidget::getIsStayTop() const
{
    return d_ptr->_appBar->getIsStayTop();
}

void NXWidget::setIsFixedSize(bool isFixedSize)
{
    Q_D(NXWidget);
    d->_appBar->setIsFixedSize(isFixedSize);
}

bool NXWidget::getIsFixedSize() const
{
    return d_ptr->_appBar->getIsFixedSize();
}

void NXWidget::setIsDefaultClosed(bool isDefaultClosed)
{
    Q_D(NXWidget);
    d->_appBar->setIsDefaultClosed(isDefaultClosed);
    Q_EMIT pIsDefaultClosedChanged();
}

bool NXWidget::getIsDefaultClosed() const
{
    Q_D(const NXWidget);
    return d->_appBar->getIsDefaultClosed();
}

void NXWidget::setAppBarHeight(int appBarHeight)
{
    Q_D(NXWidget);
    d->_appBar->setAppBarHeight(appBarHeight);
    Q_EMIT pAppBarHeightChanged();
}

int NXWidget::getAppBarHeight() const
{
    Q_D(const NXWidget);
    return d->_appBar->getAppBarHeight();
}

void NXWidget::moveToCenter()
{
    if (isMaximized() || isFullScreen())
    {
        return;
    }
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    auto geometry = screen()->availableGeometry();
#else
    auto geometry = qApp->screenAt(this->geometry().center())->geometry();
#endif
    setGeometry((geometry.left() + geometry.right() - width()) / 2, (geometry.top() + geometry.bottom() - height()) / 2, width(), height());
}

void NXWidget::setWindowButtonFlag(NXAppBarType::ButtonType buttonFlag, bool isEnable)
{
    Q_D(NXWidget);
    d->_appBar->setWindowButtonFlag(buttonFlag, isEnable);
}

void NXWidget::setWindowButtonFlags(NXAppBarType::ButtonFlags buttonFlags)
{
    Q_D(NXWidget);
    d->_appBar->setWindowButtonFlags(buttonFlags);
}

NXAppBarType::ButtonFlags NXWidget::getWindowButtonFlags() const
{
    return d_ptr->_appBar->getWindowButtonFlags();
}

NXAppBar* NXWidget::appBar() const
{
    Q_D(const NXWidget);
    return d->_appBar;
}

void NXWidget::paintEvent(QPaintEvent* event)
{
    Q_D(NXWidget);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(NXThemeColor(d->_themeMode, WindowBase));
    painter.drawRect(rect());
    painter.restore();
    QWidget::paintEvent(event);
}
