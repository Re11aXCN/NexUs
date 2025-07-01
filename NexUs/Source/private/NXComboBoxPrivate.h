#ifndef NXCOMBOBOXPRIVATE_H
#define NXCOMBOBOXPRIVATE_H

#include <QObject>

#include "NXDef.h"
class NXComboBox;
class NXComboBoxStyle;
class NXComboBoxPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXComboBox);
    Q_PROPERTY_CREATE_D(int, BorderRadius)

public:
    explicit NXComboBoxPrivate(QObject* parent = nullptr);
    ~NXComboBoxPrivate();

private:
    bool _isAllowHidePopup{false};
    NXThemeType::ThemeMode _themeMode;

    std::shared_ptr<NXComboBoxStyle> _comboBoxStyle{ nullptr };
};

#endif // NXCOMBOBOXPRIVATE_H
