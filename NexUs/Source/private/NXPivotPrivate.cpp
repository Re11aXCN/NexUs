#include "NXPivotPrivate.h"

#include <QModelIndex>
#include <QPropertyAnimation>

#include "NXPivot.h"
#include "DeveloperComponents/NXPivotStyle.h"
#include "DeveloperComponents/NXPivotView.h"
NXPivotPrivate::NXPivotPrivate(QObject* parent)
    : QObject{parent}
{
}

NXPivotPrivate::~NXPivotPrivate()
{
}

void NXPivotPrivate::_checkCurrentIndex()
{
    Q_Q(NXPivot);
    QModelIndex currentIndex = _listView->currentIndex();
    if (currentIndex.row() != _listStyle->getCurrentIndex())
    {
        _listView->doCurrentIndexChangedAnimation(currentIndex);
        _listStyle->setCurrentIndex(currentIndex.row());
        Q_EMIT q->pCurrentIndexChanged();
    }
}
