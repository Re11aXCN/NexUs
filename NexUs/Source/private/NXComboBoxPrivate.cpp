#include "NXComboBoxPrivate.h"
#include "NXComboBox.h"
#include <QLineEdit>
#include <QTimer>
NXComboBoxPrivate::NXComboBoxPrivate(QObject* parent)
    : QObject{parent}
{
}

NXComboBoxPrivate::~NXComboBoxPrivate()
{
}

void NXComboBoxPrivate::onThemeChanged(NXThemeType::ThemeMode themeMode)
{
    Q_Q(NXComboBox);
    _themeMode = themeMode;
    auto lineEdit = q->lineEdit();
    if (lineEdit)
    {
        if (q->isVisible())
        {
            _changeTheme(lineEdit);
        }
        else
        {
            QTimer::singleShot(1, this, [=] {
                _changeTheme(lineEdit);
            });
        }
    }
}

void NXComboBoxPrivate::_changeTheme(QLineEdit* lineEdit)
{
    Q_Q(NXComboBox);
    QPalette palette = lineEdit->palette();
    if (_themeMode == NXThemeType::Light)
    {
        palette.setColor(QPalette::Text, Qt::black);
        palette.setColor(QPalette::PlaceholderText, QColor(0x00, 0x00, 0x00, 128));
    }
    else
    {
        palette.setColor(QPalette::Text, Qt::white);
        palette.setColor(QPalette::PlaceholderText, QColor(0xBA, 0xBA, 0xBA));
    }
    lineEdit->setPalette(palette);
}
