#include "NXLineEditPrivate.h"

#include "NXApplication.h"
#include "NXLineEdit.h"

NXLineEditPrivate::NXLineEditPrivate(QObject* parent)
    : QObject{parent}
{
}

NXLineEditPrivate::~NXLineEditPrivate()
{
}

void NXLineEditPrivate::onWMWindowClickedEvent(QVariantMap data)
{
    Q_Q(NXLineEdit);
    NXAppBarType::WMMouseActionType actionType = data.value("WMClickType").value<NXAppBarType::WMMouseActionType>();
    if (actionType == NXAppBarType::WMLBUTTONDOWN)
    {
        if (q->hasSelectedText() && q->hasFocus())
        {
            q->clearFocus();
        }
    }
    else if (actionType == NXAppBarType::WMLBUTTONUP || actionType == NXAppBarType::WMNCLBUTTONDOWN)
    {
        if (NXApplication::containsCursorToItem(q) || (actionType == NXAppBarType::WMLBUTTONUP && q->hasSelectedText()))
        {
            return;
        }
        if (q->hasFocus())
        {
            q->clearFocus();
        }
    }
}

void NXLineEditPrivate::onThemeChanged(NXThemeType::ThemeMode themeMode)
{
    Q_Q(NXLineEdit);
    _themeMode = themeMode;
    if (themeMode == NXThemeType::Light)
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