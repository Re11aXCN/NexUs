#ifndef NXMULTISELECTCOMBOBOXPRIVATE_H
#define NXMULTISELECTCOMBOBOXPRIVATE_H
#include <QObject>
#include <QVector>

#include "NXDef.h"
#include "stdafx.h"
class NXComboBoxStyle;
class NXComboBoxView;
class NXMultiSelectComboBox;
class NXMultiSelectComboBoxPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXMultiSelectComboBox)
    Q_PROPERTY_CREATE_D(int, BorderRadius)
    Q_PROPERTY_CREATE(qreal, ExpandIconRotate)
    Q_PROPERTY_CREATE(qreal, ExpandMarkWidth)
public:
    explicit NXMultiSelectComboBoxPrivate(QObject* parent = nullptr);
    ~NXMultiSelectComboBoxPrivate();
    Q_SLOT void onItemPressed(const QModelIndex& index);

private:
    NXComboBoxStyle* _comboBoxStyle{nullptr};
    NXComboBoxView* _comboView{nullptr};
    QVector<bool> _itemSelection;
    bool _isFirstPopup{false};
    bool _isAllowHidePopup{false};
    QString _currentText;
    QStringList _selectedTextList;
    void _refreshCurrentIndexs();
    void _adjustSelectedVector();
    NXThemeType::ThemeMode _themeMode;
};

#endif // NXMULTISELECTCOMBOBOXPRIVATE_H
