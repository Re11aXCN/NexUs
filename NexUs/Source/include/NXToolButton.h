#ifndef NXTOOLBUTTON_H
#define NXTOOLBUTTON_H

#include <NXAdvancedToolButton.h>

#include "NXDef.h"
class NXMenu;
class NXToolButtonPrivate;
class NX_EXPORT NXToolButton : public NXAdvancedToolButton
{
    Q_OBJECT
    Q_Q_CREATE(NXToolButton)
    Q_PROPERTY_CREATE_Q_H(int, BorderRadius);
    Q_PROPERTY_CREATE_Q_H(bool, IsSelected);

public:
    explicit NXToolButton(QWidget* parent = nullptr);
    ~NXToolButton();

    void setIsTransparent(bool isTransparent);
    bool getIsTransparent() const;

    void setMenu(NXMenu* menu);
    void setNXIcon(NXIconType::IconName icon);

protected:
    virtual bool eventFilter(QObject* watched, QEvent* event) override;
};

#endif // NXTOOLBUTTON_H
