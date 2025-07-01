#ifndef NXPLAINTEXTEDIT_H
#define NXPLAINTEXTEDIT_H

#include <QPlainTextEdit>

#include "stdafx.h"

class NXPlainTextEditPrivate;
class NX_EXPORT NXPlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT
    Q_Q_CREATE(NXPlainTextEdit)
public:
    explicit NXPlainTextEdit(QWidget* parent = nullptr);
    explicit NXPlainTextEdit(const QString& text, QWidget* parent = nullptr);
    virtual ~NXPlainTextEdit();

protected:
    virtual void focusInEvent(QFocusEvent* event) override;
    virtual void focusOutEvent(QFocusEvent* event) override;
    virtual void contextMenuEvent(QContextMenuEvent* event) override;
};

#endif // NXPLAINTEXTEDIT_H
