#ifndef NXCONTENTDIALOG_H
#define NXCONTENTDIALOG_H
#include <QAbstractNativeEventFilter>
#include <QDialog>

#include "stdafx.h"

class NXContentDialogPrivate;
class NX_EXPORT NXContentDialog : public QDialog
{
    Q_OBJECT
    Q_Q_CREATE(NXContentDialog)
public:
    explicit NXContentDialog(QWidget* parent);
    ~NXContentDialog() override;
    Q_SLOT virtual void onLeftButtonClicked();
    Q_SLOT virtual void onMiddleButtonClicked();
    Q_SLOT virtual void onRightButtonClicked();
    void setCentralWidget(QWidget* centralWidget);

    void setLeftButtonText(const QString& text);
    void setMiddleButtonText(const QString& text);
    void setRightButtonText(const QString& text);

    void close();
Q_SIGNALS:
    Q_SIGNAL void leftButtonClicked();
    Q_SIGNAL void middleButtonClicked();
    Q_SIGNAL void rightButtonClicked();

protected:
    virtual void showEvent(QShowEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void keyPressEvent(QKeyEvent* event) override;
#ifdef Q_OS_WIN
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    virtual bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override;
#else
    virtual bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
#endif
#endif
};

#endif // NXCONTENTDIALOG_H
