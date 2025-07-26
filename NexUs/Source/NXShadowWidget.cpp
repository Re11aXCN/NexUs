#include "NXShadowWidget.h"

#include <QPainter>
#include <QPainterPath>
#include "NXShadowGraphicsEffect.h"
NXShadowWidget::NXShadowWidget(QWidget* parent /*= nullptr*/)
    : QWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    setStyleSheet("background-color: transparent;");
    setFixedSize(200, 200);
    _pShadowEffect = new NXShadowGraphicsEffect(this);
    setGraphicsEffect(_pShadowEffect);
}

void NXShadowWidget::setCustomDraw(std::function<void(QPainter*, QWidget*)> customDraw)
{
    _pCustomDraw = customDraw;
}

void NXShadowWidget::setDarkOffset(const QPointF& size) {
    _pShadowEffect->setDarkOffset(size);
    update();
}

QPointF NXShadowWidget::getDarkOffset() const {
    return _pShadowEffect->getDarkOffset();
}

void NXShadowWidget::setLightOffset(const QPointF& size) {
    _pShadowEffect->setLightOffset(size);
    update();
}

QPointF NXShadowWidget::getLightOffset() const {
    return _pShadowEffect->getLightOffset();
}

void NXShadowWidget::setRotateMode(const NXWidgetType::BoxShadow::RotateMode& mode) {
    _pShadowEffect->setRotateMode(mode);
    update();
}

NXWidgetType::BoxShadow::RotateMode NXShadowWidget::getRotateMode() const {
    return _pShadowEffect->getRotateMode();
}

void NXShadowWidget::setProjectionType(const NXWidgetType::BoxShadow::ProjectionType& type) {
    _pShadowEffect->setProjectionType(type);
    update();
}

NXWidgetType::BoxShadow::ProjectionType NXShadowWidget::getProjectionType() const {
    return _pShadowEffect->getProjectionType();
}

void NXShadowWidget::setBlur(const qreal& blur) {
    _pShadowEffect->setBlur(blur);
    update();
}

qreal NXShadowWidget::getBlur() const {
    return _pShadowEffect->getBlur();
}

void NXShadowWidget::setSpread(const qreal& spread) {
    _pShadowEffect->setSpread(spread);
    update();
}

qreal NXShadowWidget::getSpread() const {
    return _pShadowEffect->getSpread();
}

void NXShadowWidget::setLightColor(const QColor& color) {
    _pShadowEffect->setLightColor(color);
    update();
}

QColor NXShadowWidget::getLightColor() const {
    return _pShadowEffect->getLightColor();
}

void NXShadowWidget::setDarkColor(const QColor& color) {
    _pShadowEffect->setDarkColor(color);
    update();
}

QColor NXShadowWidget::getDarkColor() const {
    return _pShadowEffect->getDarkColor();
}

void NXShadowWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    if (_pShadowEffect->getProjectionType() == NXWidgetType::BoxShadow::ProjectionType::Outset) {
        if (_pCustomDraw) {
            _pCustomDraw(&painter, this->parentWidget());
        }
        else {
            painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
            painter.setPen(Qt::NoPen);
            painter.setBrush(QColor{ 235, 239, 243, 255 });
            painter.drawRect(rect());
        }
    }
    QWidget::paintEvent(event);
}

