#include "NXShadowGraphicsEffect.h"
#include <QPainter>
#include "NXTheme.h"
#include "private/NXShadowGraphicsEffectPrivate.h"

NXShadowGraphicsEffect::NXShadowGraphicsEffect(QObject* parent /*= nullptr*/)
    : QGraphicsEffect{ parent }, d_ptr(new NXShadowGraphicsEffectPrivate())
{
    Q_D(NXShadowGraphicsEffect);
    d->q_ptr = this;
    d->_pBlur = 0.0;
    d->_pSpread = 0.0;
    d->_pLightColor = NXThemeColor(d->_themeMode, BasicBaseAlpha);
    d->_pDarkColor = NXThemeColor(d->_themeMode, BasicBaseAlpha);
    d->_pRotateMode = NXWidgetType::BoxShadow::RotateMode::Rotate45;
    d->_pProjectionType = NXWidgetType::BoxShadow::ProjectionType::Inset;
    d->_pLightOffset = QPointF{ 0.0,0.0 };
    d->_pDarkOffset = QPointF{ 0.0,0.0 };
    /*NXShadowGraphicsEffect* shadow = new NXShadowGraphicsEffect(this);
    shadow->setBlur(30.0);
    shadow->setLightColor(QColor(0, 0x1E, 0x9A, 102));
    shadow->setDarkColor(Constant::SWITCH_CIRCLE_DARK_SHADOW_COLOR);
    shadow->setLightOffset({ -5,-5 });
    shadow->setDarkOffset({ 5,5 });
    shadow->setProjectionType(NXWidgetType::BoxShadow::ProjectionType::Outset);
    shadow->setRotateMode(NXWidgetType::BoxShadow::RotateMode::Rotate45);
    setGraphicsEffect(shadow);*/
}

NXShadowGraphicsEffect::~NXShadowGraphicsEffect()
{

}

void NXShadowGraphicsEffect::setDarkOffset(const QPointF& size) {
    Q_D(NXShadowGraphicsEffect);
    d->_pDarkOffset = size;
    update();
}

QPointF NXShadowGraphicsEffect::getDarkOffset() const {
    Q_D(const NXShadowGraphicsEffect);
    return d->_pDarkOffset;
}

void NXShadowGraphicsEffect::setLightOffset(const QPointF& size) {
    Q_D(NXShadowGraphicsEffect);
    d->_pLightOffset = size;
    update();
}

QPointF NXShadowGraphicsEffect::getLightOffset() const {
    Q_D(const NXShadowGraphicsEffect);
    return d->_pLightOffset;
}

void NXShadowGraphicsEffect::setRotateMode(const NXWidgetType::BoxShadow::RotateMode& mode) {
    Q_D(NXShadowGraphicsEffect);
    d->_pRotateMode = mode;
    update();
}

NXWidgetType::BoxShadow::RotateMode NXShadowGraphicsEffect::getRotateMode() const {
    Q_D(const NXShadowGraphicsEffect);
    return d->_pRotateMode;
}

void NXShadowGraphicsEffect::setProjectionType(const NXWidgetType::BoxShadow::ProjectionType& type) {
    Q_D(NXShadowGraphicsEffect);
    d->_pProjectionType = type;
    update();
}

NXWidgetType::BoxShadow::ProjectionType NXShadowGraphicsEffect::getProjectionType() const {
    Q_D(const NXShadowGraphicsEffect);
    return d->_pProjectionType;
}

void NXShadowGraphicsEffect::setBlur(const qreal& blur) {
    Q_D(NXShadowGraphicsEffect);
    d->_pBlur = blur;
    update();
}

qreal NXShadowGraphicsEffect::getBlur() const {
    Q_D(const NXShadowGraphicsEffect);
    return d->_pBlur;
}

void NXShadowGraphicsEffect::setSpread(const qreal& spread) {
    Q_D(NXShadowGraphicsEffect);
    d->_pSpread = spread;
    update();
}

qreal NXShadowGraphicsEffect::getSpread() const {
    Q_D(const NXShadowGraphicsEffect);
    return d->_pSpread;
}

void NXShadowGraphicsEffect::setLightColor(const QColor& color) {
    Q_D(NXShadowGraphicsEffect);
    d->_pLightColor = color;
    update();
}

QColor NXShadowGraphicsEffect::getLightColor() const {
    Q_D(const NXShadowGraphicsEffect);
    return d->_pLightColor;
}

void NXShadowGraphicsEffect::setDarkColor(const QColor& color) {
    Q_D(NXShadowGraphicsEffect);
    d->_pDarkColor = color;
    update();
}

QColor NXShadowGraphicsEffect::getDarkColor() const {
    Q_D(const NXShadowGraphicsEffect);
    return d->_pDarkColor;
}

QRectF NXShadowGraphicsEffect::boundingRectFor(const QRectF& rect) const
{
    Q_D(const NXShadowGraphicsEffect);
    
    // 对于内阴影，不需要扩展边界
    if (d->_pProjectionType == NXWidgetType::BoxShadow::ProjectionType::Inset) {
        return rect.united(rect.translated(0, 0));
    }
    
    // 对于外阴影，需要扩展边界以容纳阴影
    qreal boundary = d->_pBlur + d->_pSpread * M_SQRT1_2;
    
    // 无论是矩形还是圆形阴影，都需要在所有方向上扩展边界
    return rect.united(rect.translated(0, 0).adjusted(-boundary, -boundary, boundary, boundary));
}

void NXShadowGraphicsEffect::draw(QPainter* painter)
{
    Q_D(NXShadowGraphicsEffect);
    QPoint pos;
    const QPixmap pixmap = sourcePixmap(Qt::DeviceCoordinates, &pos, PadToEffectiveBoundingRect);
    if (pixmap.isNull()) return;
    if (d->_pBlur <= 0.0) return;
    QTransform restoreTransform = painter->worldTransform();
    painter->setWorldTransform(QTransform());
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter->setPen(Qt::NoPen);

    if (d->_pProjectionType == NXWidgetType::BoxShadow::ProjectionType::Inset) {
        d->_drawInsetShadow(painter, pixmap, pos);
    }
    else {
        d->_drawOutsetShadow(painter, pixmap, pos);
    }

    painter->setWorldTransform(restoreTransform);
}
