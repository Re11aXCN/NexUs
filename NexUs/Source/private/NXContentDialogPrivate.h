#ifndef NXCONTENTDIALOGPRIVATE_H
#define NXCONTENTDIALOGPRIVATE_H

#include <QObject>

#include "NXDef.h"
class QVBoxLayout;
class NXPushButton;
class NXContentDialog;
class NXMaskWidget;
class NXContentDialogPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXContentDialog)
public:
    explicit NXContentDialogPrivate(QObject* parent = nullptr);
    ~NXContentDialogPrivate();

private:
    qint64 _currentWinID{0};
    NXThemeType::ThemeMode _themeMode;
    NXMaskWidget* _maskWidget{nullptr};
    QWidget* _centralWidget{nullptr};
    QWidget* _buttonWidget{nullptr};
    QVBoxLayout* _mainLayout{nullptr};
    QString _leftButtonText{"cancel"};
    QString _middleButtonText{"minimum"};
    QString _rightButtonText{"exit"};
    NXPushButton* _leftButton{nullptr};
    NXPushButton* _middleButton{nullptr};
    NXPushButton* _rightButton{nullptr};
    void _doCloseAnimation();
};

#endif // NXCONTENTDIALOGPRIVATE_H
