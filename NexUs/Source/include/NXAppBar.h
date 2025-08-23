﻿#ifndef NXAPPBAR_H
#define NXAPPBAR_H

#include <QWidget>

#include "NXDef.h"

#ifdef Q_OS_WIN
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#define Q_TAKEOVER_NATIVEEVENT_H virtual bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override;
#else
#define Q_TAKEOVER_NATIVEEVENT_H virtual bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;
#endif
#else
#define Q_TAKEOVER_NATIVEEVENT_H
#endif

#ifdef Q_OS_WIN
#define NXAPPBAR_HANDLE(NXAppBar)                                           \
    if (NXAppBar)                                                            \
    {                                                                         \
        int ret = NXAppBar->takeOverNativeEvent(eventType, message, result); \
        if (ret == -1)                                                        \
        {                                                                     \
            return QWidget::nativeEvent(eventType, message, result);          \
        }                                                                     \
        return (bool)ret;                                                     \
    }                                                                         \
    return QWidget::nativeEvent(eventType, message, result);
#endif

#ifdef Q_OS_WIN
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#define Q_TAKEOVER_NATIVEEVENT_CPP(CLASS, NXAppBar)                                     \
    bool CLASS::nativeEvent(const QByteArray& eventType, void* message, qintptr* result) \
    {                                                                                    \
        NXAPPBAR_HANDLE(NXAppBar)                                                      \
    }
#else
#define Q_TAKEOVER_NATIVEEVENT_CPP(CLASS, NXAppBar)                                  \
    bool CLASS::nativeEvent(const QByteArray& eventType, void* message, long* result) \
    {                                                                                 \
        NXAPPBAR_HANDLE(NXAppBar)                                                   \
    }
#endif
#else
#define Q_TAKEOVER_NATIVEEVENT_CPP(CLASS, NXAppBar)
#endif
class QMenu;
class NXAppBarPrivate;
class NX_EXPORT NXAppBar : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(NXAppBar)
    Q_PROPERTY_CREATE_Q_H(bool, IsStayTop)
    Q_PROPERTY_CREATE_Q_H(bool, IsFixedSize)
    Q_PROPERTY_CREATE_Q_H(bool, IsDefaultClosed)
    Q_PROPERTY_CREATE_Q_H(bool, IsOnlyAllowMinAndClose)
    Q_PROPERTY_CREATE_Q_H(int, AppBarHeight)
    Q_PROPERTY_CREATE_Q_H(int, CustomWidgetMaximumWidth)
public:
    explicit NXAppBar(QWidget* parent = nullptr);
    ~NXAppBar() override;

    void setCustomWidget(NXAppBarType::CustomArea customArea, QWidget* customWidget);
    QWidget* getCustomWidget() const;
    void setCustomMenu(QMenu* customMenu);
    QMenu* getCustomMenu() const;

    void setWindowButtonFlag(NXAppBarType::ButtonType buttonFlag, bool isEnable = true);
    void setWindowButtonFlags(NXAppBarType::ButtonFlags buttonFlags);
    NXAppBarType::ButtonFlags getWindowButtonFlags() const;

    void setRouteBackButtonEnable(bool isEnable);

    void closeWindow();
#ifdef Q_OS_WIN
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    int takeOverNativeEvent(const QByteArray& eventType, void* message, qintptr* result);
#else
    int takeOverNativeEvent(const QByteArray& eventType, void* message, long* result);
#endif
#endif
Q_SIGNALS:
    Q_SIGNAL void routeBackButtonClicked();
    Q_SIGNAL void navigationButtonClicked();
    Q_SIGNAL void themeChangeButtonClicked();
    Q_SIGNAL void closeButtonClicked();
    Q_SIGNAL void customWidgetChanged();
    Q_SIGNAL void customMenuChanged();

protected:
    virtual bool eventFilter(QObject* obj, QEvent* event) override;
#ifdef Q_OS_WIN
    virtual void paintEvent(QPaintEvent* event) override;
#endif
};

#endif // NXAPPBAR_H
