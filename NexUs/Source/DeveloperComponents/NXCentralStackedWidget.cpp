#include "NXCentralStackedWidget.h"

#include <QPainter>
#include <QPainterPath>

#include "NXTheme.h"
NXCentralStackedWidget::NXCentralStackedWidget(QWidget* parent)
    : QStackedWidget(parent)
{
    setObjectName("NXCentralStackedWidget");
    _themeMode = nxTheme->getThemeMode();
    QObject::connect(nxTheme, &NXTheme::themeModeChanged, this, &NXCentralStackedWidget::onxThemeModeChanged);
}

NXCentralStackedWidget::~NXCentralStackedWidget()
{
}

void NXCentralStackedWidget::onxThemeModeChanged(NXThemeType::ThemeMode themeMode)
{
    _themeMode = themeMode;
}

void NXCentralStackedWidget::setIsTransparent(bool isTransparent)
{
    this->_isTransparent = isTransparent;
    update();
}

bool NXCentralStackedWidget::getIsTransparent() const
{
    return _isTransparent;
}

void NXCentralStackedWidget::setIsHasRadius(bool isHasRadius)
{
    this->_isHasRadius = isHasRadius;
    update();
}

void NXCentralStackedWidget::paintEvent(QPaintEvent* event)
{
    if (!_isTransparent)
    {
        QPainter painter(this);
        QRect targetRect = this->rect();
        targetRect.adjust(1, 1, 10, 10);
        painter.save();
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(QPen(NXThemeColor(_themeMode, BasicBaseLine), 1.5));
        painter.setBrush(NXThemeColor(_themeMode, WindowCentralStackBase));
        painter.drawRoundedRect(targetRect, 10, 10);
        painter.restore();
    }
    QStackedWidget::paintEvent(event);
}
