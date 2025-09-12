﻿#include "NXKeyBinder.h"
#include "NXContentDialog.h"
#include "DeveloperComponents/NXKeyBinderContainer.h"
#include "private/NXKeyBinderPrivate.h"
#include "NXTheme.h"
#include <QMouseEvent>
#include <QPainter>
#include <utility>
Q_PROPERTY_CREATE_Q_CPP(NXKeyBinder, int, BorderRadius)
NXKeyBinder::NXKeyBinder(QWidget* parent)
    : QLabel(parent), d_ptr(new NXKeyBinderPrivate())
{
    Q_D(NXKeyBinder);
    d->q_ptr = this;
    d->_pBorderRadius = 5;
    setFixedHeight(35);
    setMouseTracking(true);
    setStyleSheet("#NXKeyBinder{background-color:transparent;}");
    QFont textFont = font();
    textFont.setLetterSpacing(QFont::AbsoluteSpacing, 0.5);
    textFont.setPixelSize(15);
    setFont(textFont);
    d->_binderContainer = new NXKeyBinderContainer(this);
    setText(u8"  按键: " + QString(u8"未绑定") + "      ");
    d->_binderDialog = new NXContentDialog(window());
    d->_binderDialog->setCentralWidget(d->_binderContainer);
    d->_binderDialog->setLeftButtonText(u8"取消");
    d->_binderDialog->setMiddleButtonText(u8"重置");
    d->_binderDialog->setRightButtonText(u8"确认");
    QObject::connect(d->_binderDialog, &NXContentDialog::leftButtonClicked, &NXContentDialog::close);
    QObject::connect(d->_binderDialog, &NXContentDialog::middleButtonClicked, this, [=]() {
        d->_binderContainer->logOrResetHistoryData(false);
    });
    QObject::connect(d->_binderDialog, &NXContentDialog::rightButtonClicked, this, [=]() {
        d->_binderContainer->saveBinderChanged();
    });
    d->onThemeChanged(nxTheme->getThemeMode());
    QObject::connect(nxTheme, &NXTheme::themeModeChanged, d, &NXKeyBinderPrivate::onThemeChanged);
}

NXKeyBinder::~NXKeyBinder()
{
}

void NXKeyBinder::setBinderKeyText(const QString& binderKeyText)
{
    Q_D(NXKeyBinder);
    d->_binderContainer->setBinderKeyText(binderKeyText);
    setText(u8"  按键: " + binderKeyText + "      ");
}

QString NXKeyBinder::getBinderKeyText() const
{
    Q_D(const NXKeyBinder);
    return d->_binderContainer->getBinderKeyText();
}

void NXKeyBinder::setNativeVirtualBinderKey(quint32 binderKey)
{
    Q_D(NXKeyBinder);
    d->_binderContainer->setNativeVirtualBinderKey(binderKey);
}

quint32 NXKeyBinder::getNativeVirtualBinderKey() const
{
    Q_D(const NXKeyBinder);
    return d->_binderContainer->getNativeVirtualBinderKey();
}

bool NXKeyBinder::event(QEvent* event)
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
    return QLabel::event(event);
}

void NXKeyBinder::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(NXKeyBinder);
    if (event->button() == Qt::LeftButton)
    {
        d->_binderDialog->show();
        d->_binderContainer->setFocus();
        d->_binderContainer->logOrResetHistoryData(true);
    }
    QLabel::mouseReleaseEvent(event);
}

void NXKeyBinder::paintEvent(QPaintEvent* event)
{
    Q_D(NXKeyBinder);
    if (palette().color(QPalette::WindowText) != NXThemeColor(d->_themeMode, BasicText))
    {
        d->onThemeChanged(d->_themeMode);
    }
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(NXThemeColor(d->_themeMode, BasicBorder));
    painter.setBrush(underMouse() ? NXThemeColor(d->_themeMode, BasicHover) : Qt::transparent);
    QRect borderRect = rect();
    borderRect.adjust(1, 1, -1, -1);
    painter.drawRoundedRect(borderRect, d->_pBorderRadius, d->_pBorderRadius);
    // 图标绘制
    QFont iconFont = QFont("NXAwesome");
    iconFont.setPixelSize(16);
    painter.setFont(iconFont);
    painter.setPen(NXThemeColor(d->_themeMode, BasicText));
    QRect iconRect = rect();
    iconRect.adjust(0, 0, -10, 0);
    painter.drawText(iconRect, Qt::AlignVCenter | Qt::AlignRight, QChar((unsigned short)NXIconType::Pencil));
    painter.restore();
    QLabel::paintEvent(event);
}