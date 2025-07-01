#include "NXPlainTextEditPrivate.h"
#include "NXApplication.h"
#include "NXPlainTextEdit.h"
#include <QTimer>
NXPlainTextEditPrivate::NXPlainTextEditPrivate(QObject* parent)
    : QObject{parent}
{
}

NXPlainTextEditPrivate::~NXPlainTextEditPrivate()
{
}

void NXPlainTextEditPrivate::onWMWindowClickedEvent(QVariantMap data)
{
    Q_Q(NXPlainTextEdit);
    NXAppBarType::WMMouseActionType actionType = data.value("WMClickType").value<NXAppBarType::WMMouseActionType>();
    if (actionType == NXAppBarType::WMLBUTTONDOWN)
    {
        if (!q->toPlainText().isEmpty() && q->hasFocus())
        {
            q->clearFocus();
        }
    }
    else if (actionType == NXAppBarType::WMLBUTTONUP || actionType == NXAppBarType::WMNCLBUTTONDOWN)
    {
        if (NXApplication::containsCursorToItem(q) || (actionType == NXAppBarType::WMLBUTTONUP && !q->toPlainText().isEmpty()))
        {
            return;
        }
        if (q->hasFocus())
        {
            q->clearFocus();
        }
    }
}

void NXPlainTextEditPrivate::onThemeChanged(NXThemeType::ThemeMode themeMode)
{
    Q_Q(NXPlainTextEdit);
    _themeMode = themeMode;
    if (q->isVisible())
    {
        _changeTheme();
    }
    else
    {
        QTimer::singleShot(1, this, [=] {
            _changeTheme();
        });
    }
}

void NXPlainTextEditPrivate::_changeTheme()
{
    Q_Q(NXPlainTextEdit);
    if (_themeMode == NXThemeType::Light)
    {
        QPalette palette;
        palette.setColor(QPalette::Text, Qt::black);
        palette.setColor(QPalette::PlaceholderText, QColor(0x00, 0x00, 0x00, 128));
        q->setPalette(palette);
    }
    else
    {
        QPalette palette;
        palette.setColor(QPalette::Text, Qt::white);
        palette.setColor(QPalette::PlaceholderText, QColor(0xBA, 0xBA, 0xBA));
        q->setPalette(palette);
    }
}
