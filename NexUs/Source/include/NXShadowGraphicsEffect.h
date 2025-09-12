#ifndef NX_SHADOW_GRAPHICSEFFECT_H_
#define NX_SHADOW_GRAPHICSEFFECT_H_

#include <QGraphicsEffect>
#include "NXDef.h"
class NXShadowGraphicsEffectPrivate;
class NX_EXPORT NXShadowGraphicsEffect : public QGraphicsEffect
{
    Q_OBJECT
    Q_Q_CREATE(NXShadowGraphicsEffect)
    Q_PRIVATE_CREATE_COMPLEX_H(QColor, LightColor)
    Q_PRIVATE_CREATE_COMPLEX_H(QColor, DarkColor)
    Q_PRIVATE_CREATE_COMPLEX_H(NXShadowGraphicsEffectType::RotateMode, RotateMode)
    Q_PRIVATE_CREATE_COMPLEX_H(NXShadowGraphicsEffectType::ProjectionMode, ProjectionMode)
    Q_PRIVATE_CREATE_COMPLEX_H(qreal, Blur)
    Q_PRIVATE_CREATE_COMPLEX_H(qreal, Spread)
    Q_PRIVATE_CREATE_COMPLEX_H(QPointF, LightOffset)
    Q_PRIVATE_CREATE_COMPLEX_H(QPointF, DarkOffset)
public:
    explicit NXShadowGraphicsEffect(QObject* parent = nullptr);
    ~NXShadowGraphicsEffect();
protected:
    QRectF boundingRectFor(const QRectF& rect) const override;
    void draw(QPainter* painter) override;
};

#endif // !NX_SHADOW_GRAPHICSEFFECT_H_
