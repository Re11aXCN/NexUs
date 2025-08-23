#ifndef NXCONTENTDIALOGPRIVATE_H
#define NXCONTENTDIALOGPRIVATE_H

#include <QObject>

#include "NXDef.h"
class QVBoxLayout;
class NXPushButton;
class NXContentDialog;
class NXMaskWidget;
class NXAppBar;
class NXContentDialogPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXContentDialog)
public:
    explicit NXContentDialogPrivate(QObject* parent = nullptr);
    ~NXContentDialogPrivate() override;

private:
 	qint64 _currentWinID{0};
    NXThemeType::ThemeMode _themeMode;
    QString _leftButtonText{ "cancel" };
    QString _middleButtonText{ "minimum" };
    QString _rightButtonText{ "exit" };
    NXAppBar* _appBar{ nullptr };
    NXMaskWidget* _maskWidget{nullptr};
    QWidget* _centralWidget{nullptr};
    QWidget* _buttonWidget{nullptr};
    QVBoxLayout* _mainLayout{nullptr};
    NXPushButton* _leftButton{nullptr};
    NXPushButton* _middleButton{nullptr};
    NXPushButton* _rightButton{nullptr};
    void _doCloseAnimation(bool isAccept);
};

#endif // NXCONTENTDIALOGPRIVATE_H
