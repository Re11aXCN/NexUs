#ifndef T_POPUP_H
#define T_POPUP_H

#include "T_BasePage.h"

class NXToolButton;
class NXColorDialog;
class NXCalendar;
class NXCalendarPicker;
class NXKeyBinder;
class NXRoller;
class NXDrawerArea;
class T_Popup : public T_BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit T_Popup(QWidget* parent = nullptr);
    ~T_Popup();

private:
    NXToolButton* _toolButton{nullptr};
    NXColorDialog* _colorDialog{nullptr};
    NXCalendar* _calendar{nullptr};
    NXCalendarPicker* _calendarPicker{nullptr};
    NXKeyBinder* _keyBinder{nullptr};
    NXRoller* _roller{nullptr};
    NXDrawerArea* _drawer{nullptr};
};

#endif // T_POPUP_H
