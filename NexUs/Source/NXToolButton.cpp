﻿#include "NXToolButton.h"

#include <QApplication>
#include <QDebug>
#include <QEvent>
#include <QMouseEvent>
#include <QPropertyAnimation>

#include "DeveloperComponents/NXToolButtonStyle.h"
#include "NXIcon.h"
#include "NXMenu.h"
#include "private/NXToolButtonPrivate.h"

NXAdvancedToolButton::NXAdvancedToolButton(QWidget* parent)
	: QToolButton(parent)
{

}

void NXAdvancedToolButton::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::RightButton) {
		Q_EMIT rightReleased();
	}
	else if (event->button() == Qt::MiddleButton) {
		Q_EMIT middleReleased();
	}
	QToolButton::mouseReleaseEvent(event);
}

void NXAdvancedToolButton::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::RightButton) {
		Q_EMIT rightPressed();
	}
	else if (event->button() == Qt::MiddleButton) {
		Q_EMIT middlePressed();
	}
	QToolButton::mousePressEvent(event);
}



NXToolButton::NXToolButton(QWidget* parent)
    : NXAdvancedToolButton(parent), d_ptr(new NXToolButtonPrivate())
{
    Q_D(NXToolButton);
    d->q_ptr = this;
    setIconSize(QSize(22, 22));
    setPopupMode(QToolButton::InstantPopup);
    d->_toolButtonStyle = new NXToolButtonStyle(style());
    setStyle(d->_toolButtonStyle);
}

NXToolButton::~NXToolButton()
{
    Q_D(NXToolButton);
    delete d->_toolButtonStyle;
}

void NXToolButton::setBorderRadius(int borderRadius)
{
    Q_D(NXToolButton);
    d->_toolButtonStyle->setBorderRadius(borderRadius);
    Q_EMIT pBorderRadiusChanged();
}

int NXToolButton::getBorderRadius() const
{
    Q_D(const NXToolButton);
    return d->_toolButtonStyle->getBorderRadius();
}

void NXToolButton::setIsSelected(bool isSelected)
{
    Q_D(NXToolButton);
    d->_toolButtonStyle->setIsSelected(isSelected);
    Q_EMIT pIsSelectedChanged();
}

bool NXToolButton::getIsSelected() const
{
    Q_D(const NXToolButton);
    return d->_toolButtonStyle->getIsSelected();
}

void NXToolButton::setIsTransparent(bool isTransparent)
{
    Q_D(NXToolButton);
    d->_toolButtonStyle->setIsTransparent(isTransparent);
    update();
}

bool NXToolButton::getIsTransparent() const
{
    Q_D(const NXToolButton);
    return d->_toolButtonStyle->getIsTransparent();
}

void NXToolButton::setMenu(NXMenu* menu)
{
    if (!menu || menu == this->menu())
    {
        return;
    }
    menu->setMenuItemHeight(27);
    QToolButton::setMenu(menu);
    menu->installEventFilter(this);
}

void NXToolButton::setNXIcon(NXIconType::IconName icon)
{
    setProperty("NXIconType", QChar((unsigned short)icon));
    setIcon(NXIcon::getInstance()->getNXIcon(NXIconType::Broom, 1));
}

bool NXToolButton::eventFilter(QObject* watched, QEvent* event)
{
    Q_D(NXToolButton);
    if (watched == menu())
    {
        switch (event->type())
        {
        case QEvent::Show:
        {
            //指示器动画
            QPropertyAnimation* rotateAnimation = new QPropertyAnimation(d->_toolButtonStyle, "pExpandIconRotate");
            QObject::connect(rotateAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
                update();
            });
            rotateAnimation->setDuration(300);
            rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
            rotateAnimation->setStartValue(d->_toolButtonStyle->getExpandIconRotate());
            rotateAnimation->setEndValue(-180);
            rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            break;
        }
        case QEvent::Hide:
        {
            //指示器动画
            QPropertyAnimation* rotateAnimation = new QPropertyAnimation(d->_toolButtonStyle, "pExpandIconRotate");
            QObject::connect(rotateAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
                update();
            });
            rotateAnimation->setDuration(300);
            rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
            rotateAnimation->setStartValue(d->_toolButtonStyle->getExpandIconRotate());
            rotateAnimation->setEndValue(0);
            rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            QMouseEvent focusEvent(QEvent::MouseButtonPress, QPoint(-1, -1), QPoint(-1, -1), Qt::NoButton, Qt::NoButton, Qt::NoModifier);
            QApplication::sendEvent(parentWidget(), &focusEvent);
            break;
        }
        default:
        {
            break;
        }
        }
    }
    return QObject::eventFilter(watched, event);
}
