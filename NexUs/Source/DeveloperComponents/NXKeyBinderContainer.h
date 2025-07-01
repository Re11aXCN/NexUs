#ifndef NXWIDGETTOOLS_SRC_DEVELOPERCOMPONENTS_NXKEYBINDERCONTAINER_H_
#define NXWIDGETTOOLS_SRC_DEVELOPERCOMPONENTS_NXKEYBINDERCONTAINER_H_

#include "NXDef.h"
#include <QWidget>
class NXKeyBinder;
class NXKeyBinderContainer : public QWidget
{
    Q_OBJECT
    Q_PRIVATE_CREATE(QString, BinderKeyText)
    Q_PRIVATE_CREATE(quint32, NativeVirtualBinderKey)
public:
    explicit NXKeyBinderContainer(QWidget* parent = nullptr);
    ~NXKeyBinderContainer();
    void logOrResetHistoryData(bool isLog);
    void saveBinderChanged();

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void focusOutEvent(QFocusEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;

private:
    QString _historyBinderKeyText;
    quint32 _historyNativeVirtualBinderKey{0};
    NXThemeType::ThemeMode _themeMode;
    NXKeyBinder* _keyBinder{nullptr};
};

#endif//NXWIDGETTOOLS_SRC_DEVELOPERCOMPONENTS_NXKEYBINDERCONTAINER_H_
