﻿#include "NXPlainTextEdit.h"

#include <QClipboard>
#include <QGuiApplication>
#include <QMimeData>
#include <QPainter>
#include <QPropertyAnimation>

#include "NXEventBus.h"
#include "NXMenu.h"
#include "private/NXPlainTextEditPrivate.h"
#include "DeveloperComponents/NXPlainTextEditStyle.h"
#include "NXScrollBar.h"
#include "NXTheme.h"
NXPlainTextEdit::NXPlainTextEdit(QWidget* parent)
    : QPlainTextEdit(parent), d_ptr(new NXPlainTextEditPrivate())
{
    Q_D(NXPlainTextEdit);
    d->q_ptr = this;
    setObjectName("NXPlainTextEdit");
    setStyleSheet("#NXPlainTextEdit{background-color:transparent;}");
    setHorizontalScrollBar(new NXScrollBar(this));
    setVerticalScrollBar(new NXScrollBar(this));
    setMouseTracking(true);
    // 事件总线
    d->_focusEvent = new NXEvent("WMWindowClicked", "onWMWindowClickedEvent", d);
    d->_focusEvent->registerAndInit();

    d->_style = new NXPlainTextEditStyle(style());
    setStyle(d->_style);
    d->onThemeChanged(nxTheme->getThemeMode());
    QObject::connect(nxTheme, &NXTheme::themeModeChanged, d, &NXPlainTextEditPrivate::onThemeChanged);
}

NXPlainTextEdit::NXPlainTextEdit(const QString& text, QWidget* parent)
    : NXPlainTextEdit(parent)
{
    setPlainText(text);
}

NXPlainTextEdit::~NXPlainTextEdit()
{
}

void NXPlainTextEdit::focusInEvent(QFocusEvent* event)
{
    Q_D(NXPlainTextEdit);
    if (event->reason() == Qt::MouseFocusReason)
    {
        QPropertyAnimation* markAnimation = new QPropertyAnimation(d->_style, "pExpandMarkWidth");
        QObject::connect(markAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            update();
        });
        markAnimation->setDuration(300);
        markAnimation->setEasingCurve(QEasingCurve::InOutSine);
        markAnimation->setStartValue(d->_style->getExpandMarkWidth());
        markAnimation->setEndValue(width() / 2 - 3);
        markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    QPlainTextEdit::focusInEvent(event);
}

void NXPlainTextEdit::focusOutEvent(QFocusEvent* event)
{
    Q_D(NXPlainTextEdit);
    if (event->reason() != Qt::PopupFocusReason)
    {
        QPropertyAnimation* markAnimation = new QPropertyAnimation(d->_style, "pExpandMarkWidth");
        QObject::connect(markAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            update();
        });
        markAnimation->setDuration(300);
        markAnimation->setEasingCurve(QEasingCurve::InOutSine);
        markAnimation->setStartValue(d->_style->getExpandMarkWidth());
        markAnimation->setEndValue(0);
        markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    QPlainTextEdit::focusOutEvent(event);
}

void NXPlainTextEdit::contextMenuEvent(QContextMenuEvent* event)
{
    NXMenu* menu = new NXMenu(this);
    menu->setMenuItemHeight(27);
    menu->setAttribute(Qt::WA_DeleteOnClose);
    QAction* action = nullptr;
    if (!isReadOnly())
    {
        action = menu->addNXIconAction(NXIconType::ArrowRotateLeft, "撤销", QKeySequence::Undo);
        action->setEnabled(isUndoRedoEnabled() ? document()->isUndoAvailable() : false);
        QObject::connect(action, &QAction::triggered, this, &NXPlainTextEdit::undo);

        action = menu->addNXIconAction(NXIconType::ArrowRotateRight, "恢复", QKeySequence::Redo);
        action->setEnabled(isUndoRedoEnabled() ? document()->isRedoAvailable() : false);
        QObject::connect(action, &QAction::triggered, this, &NXPlainTextEdit::redo);
        menu->addSeparator();
    }
#ifndef QT_NO_CLIPBOARD
    if (!isReadOnly())
    {
        action = menu->addNXIconAction(NXIconType::KnifeKitchen, "剪切", QKeySequence::Cut);
        action->setEnabled(!isReadOnly() && !textCursor().selectedText().isEmpty());
        QObject::connect(action, &QAction::triggered, this, &NXPlainTextEdit::cut);
    }

    action = menu->addNXIconAction(NXIconType::Copy, "复制", QKeySequence::Copy);
    action->setEnabled(!textCursor().selectedText().isEmpty());
    QObject::connect(action, &QAction::triggered, this, &NXPlainTextEdit::copy);

    if (!isReadOnly())
    {
        action = menu->addNXIconAction(NXIconType::Paste, "粘贴", QKeySequence::Paste);
        action->setEnabled(!isReadOnly() && !QGuiApplication::clipboard()->text().isEmpty());
        QObject::connect(action, &QAction::triggered, this, &NXPlainTextEdit::paste);
    }
#endif
    if (!isReadOnly())
    {
        action = menu->addNXIconAction(NXIconType::DeleteLeft, "删除");
        action->setEnabled(!isReadOnly() && !toPlainText().isEmpty() && !textCursor().selectedText().isEmpty());
        QObject::connect(action, &QAction::triggered, this, [=](bool checked) {
            if (!textCursor().selectedText().isEmpty())
            {
                textCursor().deleteChar();
            }
        });
    }
    if (!menu->isEmpty())
    {
        menu->addSeparator();
    }
    action = menu->addAction("全选");
    action->setShortcut(QKeySequence::SelectAll);
    action->setEnabled(!toPlainText().isEmpty() && !(textCursor().selectedText() == toPlainText()));
    QObject::connect(action, &QAction::triggered, this, &NXPlainTextEdit::selectAll);
    menu->popup(event->globalPos());
    this->setFocus();
}
