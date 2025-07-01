#ifndef NXCALENDARPRIVATE_H
#define NXCALENDARPRIVATE_H

#include <QDate>
#include <QObject>
#include <QPixmap>

#include "NXDef.h"
class NXCalendar;
class NXBaseListView;
class NXCalendarModel;
class NXCalendarDelegate;
class NXToolButton;
class NXCalendarPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXCalendar)
    Q_PROPERTY_CREATE_D(int, BorderRaiuds)
    Q_PROPERTY_CREATE_D(QDate, SelectedDate)
    Q_PROPERTY_CREATE(qreal, ZoomRatio)
    Q_PROPERTY_CREATE(qreal, PixOpacity)
public:
    explicit NXCalendarPrivate(QObject* parent = nullptr);
    ~NXCalendarPrivate();
    Q_SLOT void onSwitchButtonClicked();
    Q_SLOT void onCalendarViewClicked(const QModelIndex& index);
    Q_SLOT void onUpButtonClicked();
    Q_SLOT void onDownButtonClicked();

private:
    QPixmap _oldCalendarViewPix;
    QPixmap _newCalendarViewPix;
    int _lastSelectedYear{0};
    int _lastSelectedMonth{1};
    qreal _borderWidth{1.5};
    NXThemeType::ThemeMode _themeMode;
    NXBaseListView* _calendarView{nullptr};
    NXCalendarModel* _calendarModel{nullptr};
    NXCalendarDelegate* _calendarDelegate{nullptr};
    NXBaseListView* _calendarTitleView{nullptr};
    NXToolButton* _modeSwitchButton{nullptr};
    NXToolButton* _upButton{nullptr};
    NXToolButton* _downButton{nullptr};
    bool _isSwitchAnimationFinished{true};
    bool _isDrawNewPix{false};
    void _scrollToDate(QDate date);
    void _doSwitchAnimation(bool isZoomIn);
    void _updateSwitchButtonText();
};

#endif // NXCALENDARPRIVATE_H
