﻿#include "NXSpinBox.h"

#include "DeveloperComponents/NXSpinBoxStyle.h"
#include "NXMenu.h"
#include "private/NXSpinBoxPrivate.h"
#include "NXTheme.h"

#include <QContextMenuEvent>
#include <QLineEdit>
#include <QPainter>
#include <QPropertyAnimation>
NXSpinBox::NXSpinBox(QWidget* parent)
    : QSpinBox(parent), d_ptr(new NXSpinBoxPrivate())
{
    Q_D(NXSpinBox);
    d->q_ptr = this;
    d->_pExpandMarkWidth = 0;
    setFixedSize(115, 35);
    d->_style = new NXSpinBoxStyle(style());
    setStyle(d->_style);
    lineEdit()->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    lineEdit()->setStyleSheet("background-color:transparent;padding-left:10px;padding-bottom:3px;");
    d->onThemeChanged(nxTheme->getThemeMode());
    QObject::connect(nxTheme, &NXTheme::themeModeChanged, d, &NXSpinBoxPrivate::onThemeChanged);
}

NXSpinBox::~NXSpinBox()
{
}

void NXSpinBox::setButtonMode(NXSpinBoxType::ButtonMode buttonMode)
{
    Q_D(NXSpinBox);
    if (minimumWidth() < 90)
    {
        setMinimumWidth(90);
    }
    d->_style->setButtonMode(buttonMode);
    switch (buttonMode)
    {
    case NXSpinBoxType::Inline:
    {
        lineEdit()->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        lineEdit()->setStyleSheet("background-color:transparent;padding-left:10px;padding-bottom:3px;");
        break;
    }
    case NXSpinBoxType::Compact:
    case NXSpinBoxType::Side:
    case NXSpinBoxType::PMSide:
    {
        lineEdit()->setAlignment(Qt::AlignCenter);
        lineEdit()->setStyleSheet("background-color:transparent;padding-bottom:3px;");
        break;
    }
    }
    d->onThemeChanged(nxTheme->getThemeMode());
    setFrame(hasFrame());
    Q_EMIT pButtonModeChanged();
}

NXSpinBoxType::ButtonMode NXSpinBox::getButtonMode() const
{
    Q_D(const NXSpinBox);
    return d->_style->getButtonMode();
}

void NXSpinBox::focusInEvent(QFocusEvent* event)
{
    Q_D(NXSpinBox);
    if (event->reason() == Qt::MouseFocusReason)
    {
        QPropertyAnimation* markAnimation = new QPropertyAnimation(d, "pExpandMarkWidth");
        QObject::connect(markAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            update();
        });
        markAnimation->setDuration(300);
        markAnimation->setEasingCurve(QEasingCurve::InOutSine);
        markAnimation->setStartValue(d->_pExpandMarkWidth);
        markAnimation->setEndValue(width() / 2 - 3);
        markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    QSpinBox::focusInEvent(event);
}

void NXSpinBox::focusOutEvent(QFocusEvent* event)
{
    Q_D(NXSpinBox);
    if (event->reason() != Qt::PopupFocusReason)
    {
        QPropertyAnimation* markAnimation = new QPropertyAnimation(d, "pExpandMarkWidth");
        QObject::connect(markAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            update();
        });
        markAnimation->setDuration(300);
        markAnimation->setEasingCurve(QEasingCurve::InOutSine);
        markAnimation->setStartValue(d->_pExpandMarkWidth);
        markAnimation->setEndValue(0);
        markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    QSpinBox::focusOutEvent(event);
}

void NXSpinBox::paintEvent(QPaintEvent* event)
{
    Q_D(NXSpinBox);
    QSpinBox::paintEvent(event);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(NXThemeColor(d->_themeMode, PrimaryNormal));
    painter.drawRoundedRect(QRectF(width() / 2 - d->_pExpandMarkWidth, height() - 2.5, d->_pExpandMarkWidth * 2, 2.5), 2, 2);
    painter.restore();
}

void NXSpinBox::contextMenuEvent(QContextMenuEvent* event)
{
    Q_D(NXSpinBox);
    NXMenu* menu = d->_createStandardContextMenu();
    if (!menu)
    {
        return;
    }
    menu->addSeparator();
    const uint se = stepEnabled();
    QAction* up = menu->addNXIconAction(NXIconType::Plus, tr("增加"));
    up->setEnabled(se & StepUpEnabled);
    QAction* down = menu->addNXIconAction(NXIconType::Minus, tr("减少"));
    down->setEnabled(se & StepDownEnabled);
    menu->addSeparator();

    const QAbstractSpinBox* that = this;
    const QPoint pos = (event->reason() == QContextMenuEvent::Mouse)
        ? event->globalPos()
        : mapToGlobal(QPoint(event->pos().x(), 0)) + QPoint(width() / 2, height() / 2);
    const QAction* action = menu->exec(pos);
    delete menu;
    if (that && action)
    {
        if (action == up)
        {
            stepBy(1);
        }
        else if (action == down)
        {
            stepBy(-1);
        }
    }
    event->accept();
}
