#include "NXToggleButton.h"

#include <QEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>

#include "NXTheme.h"
#include "private/NXToggleButtonPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(NXToggleButton, int, BorderRadius)
Q_PROPERTY_CREATE_Q_CPP(NXToggleButton, QString, Text)
NXToggleButton::NXToggleButton(QWidget* parent)
    : QWidget(parent), d_ptr(new NXToggleButtonPrivate())
{
    Q_D(NXToggleButton);
    d->q_ptr = this;
    d->_pBorderRadius = 3;
    d->_themeMode = nxTheme->getThemeMode();
    d->_pToggleAlpha = 0;
    setMouseTracking(true);
    setFixedSize(80, 32);
    QFont font = this->font();
    font.setPixelSize(15);
    setFont(font);
    setObjectName("NXToggleButton");
    setStyleSheet("#NXToggleButton{background-color:transparent;}");
    connect(nxTheme, &NXTheme::themeModeChanged, this, [=](NXThemeType::ThemeMode themeMode) { d->_themeMode = themeMode; });
}

NXToggleButton::NXToggleButton(QString text, QWidget* parent)
    : NXToggleButton(parent)
{
    Q_D(NXToggleButton);
    d->_pText = text;
}

NXToggleButton::~NXToggleButton()
{
}

void NXToggleButton::setIsToggled(bool isToggled)
{
    Q_D(NXToggleButton);
    d->_isToggled = isToggled;
    d->_pToggleAlpha = isToggled ? 255 : 0;
    update();
    Q_EMIT toggled(isToggled);
}

bool NXToggleButton::getIsToggled() const
{
    Q_D(const NXToggleButton);
    return d->_isToggled;
}

bool NXToggleButton::event(QEvent* event)
{
    switch (event->type())
    {
    case QEvent::Enter:
    case QEvent::Leave:
    {
        update();
        break;
    }
    default:
    {
        break;
    }
    }
    return QWidget::event(event);
}

void NXToggleButton::mousePressEvent(QMouseEvent* event)
{
    Q_D(NXToggleButton);
    d->_isPressed = true;
    update();
    QWidget::mouseReleaseEvent(event);
}

void NXToggleButton::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(NXToggleButton);
    d->_isPressed = false;
    d->_isAlphaAnimationFinished = false;
    d->_isToggled = !d->_isToggled;
    QPropertyAnimation* alphaAnimation = new QPropertyAnimation(d, "pToggleAlpha");
    connect(alphaAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        update();
    });
    connect(alphaAnimation, &QPropertyAnimation::finished, this, [=]() {
        d->_isAlphaAnimationFinished = true;
    });
    alphaAnimation->setDuration(250);
    alphaAnimation->setStartValue(d->_pToggleAlpha);
    if (d->_isToggled)
    {
        alphaAnimation->setEndValue(255);
    }
    else
    {
        alphaAnimation->setEndValue(0);
    }
    alphaAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    Q_EMIT this->toggled(d->_isToggled);
    QWidget::mouseReleaseEvent(event);
}

void NXToggleButton::paintEvent(QPaintEvent* event)
{
    Q_D(NXToggleButton);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    painter.save();
    QRect foregroundRect(1, 1, width() - 2, height() - 2);
    if (d->_isAlphaAnimationFinished)
    {
        if (d->_isToggled)
        {
            painter.setPen(NXThemeColor(d->_themeMode, BasicBorder));
            painter.setBrush(isEnabled() ? d->_isPressed ? NXThemeColor(d->_themeMode, PrimaryPress) : (underMouse() ? NXThemeColor(d->_themeMode, PrimaryHover) : NXThemeColor(d->_themeMode, PrimaryNormal)) : NXThemeColor(d->_themeMode, BasicDisable));
        }
        else
        {
            painter.setPen(NXThemeColor(d->_themeMode, BasicBorder));
            painter.setBrush(isEnabled() ? d->_isPressed ? NXThemeColor(d->_themeMode, BasicPress) : (underMouse() ? NXThemeColor(d->_themeMode, BasicHover) : NXThemeColor(d->_themeMode, BasicBase)) : NXThemeColor(d->_themeMode, BasicDisable));
        }
    }
    else
    {
        painter.setPen(Qt::NoPen);
        QColor toggleColor = NXThemeColor(d->_themeMode, PrimaryNormal);
        toggleColor.setAlpha(d->_pToggleAlpha);
        painter.setBrush(toggleColor);
    }
    painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);
    
    if (!d->_isPressed && !d->_isToggled)
    {
        painter.setPen(NXThemeColor(d->_themeMode, BasicBaseLine));
        painter.drawLine(foregroundRect.x() + d->_pBorderRadius, height() - 1, foregroundRect.x() + foregroundRect.width() - d->_pBorderRadius, height() - 1);
    }

    //鏂囧瓧缁樺埗
    painter.setPen(isEnabled() ? d->_isToggled ? NXThemeColor(d->_themeMode, BasicTextInvert) : NXThemeColor(d->_themeMode, BasicText) : NXThemeColor(d->_themeMode, BasicTextDisable));
    painter.drawText(foregroundRect, Qt::AlignCenter, d->_pText);
    painter.restore();
}
