#include "NXRollerPrivate.h"
#include <QPropertyAnimation>

NXRollerPrivate::NXRollerPrivate(QObject* parent)
    : QObject(parent)
{
}

NXRollerPrivate::~NXRollerPrivate()
{
}

void NXRollerPrivate::_scroll(int delta)
{
    int steps = delta / 120;
    _targetScrollOffset -= steps * _pItemHeight;
    _targetScrollOffset = qRound(_targetScrollOffset / _pItemHeight) * _pItemHeight;
    _scrollAnimation->stop();
    _scrollAnimation->setStartValue(_pScrollOffset);
    _scrollAnimation->setEndValue(_targetScrollOffset);
    _scrollAnimation->start();
}
