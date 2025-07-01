#include "NXThemeAnimationWidget.h"

#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
NXThemeAnimationWidget::NXThemeAnimationWidget(QWidget* parent)
    : QWidget{parent}
{
    _pEndRadius = 0.01;
}

NXThemeAnimationWidget::~NXThemeAnimationWidget()
{
}

void NXThemeAnimationWidget::startAnimation(int msec)
{
    QPropertyAnimation* themeChangeAnimation = new QPropertyAnimation(this, "pRadius");
    themeChangeAnimation->setDuration(msec);
    themeChangeAnimation->setEasingCurve(QEasingCurve::InOutSine);
    QObject::connect(themeChangeAnimation, &QPropertyAnimation::finished, this, [=]() {
        Q_EMIT animationFinished();
        this->deleteLater();
    });
    QObject::connect(themeChangeAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) { update(); });
    themeChangeAnimation->setStartValue(0);
    themeChangeAnimation->setEndValue(_pEndRadius);
    themeChangeAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void NXThemeAnimationWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.drawImage(rect(), _pOldWindowBackground);
    QPainterPath path;
    path.moveTo(_pCenter.x(), _pCenter.y());
    path.addEllipse(QPointF(_pCenter.x(), _pCenter.y()), _pRadius, _pRadius);
    painter.setClipPath(path);
    painter.drawImage(rect(), _pNewWindowBackground);
    painter.restore();
}
