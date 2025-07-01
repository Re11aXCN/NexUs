#include "NXContentDialogPrivate.h"

#include "NXContentDialog.h"
#include "DeveloperComponents/NXMaskWidget.h"
NXContentDialogPrivate::NXContentDialogPrivate(QObject* parent)
    : QObject{parent}
{
}

NXContentDialogPrivate::~NXContentDialogPrivate()
{
}

void NXContentDialogPrivate::_doCloseAnimation(bool isAccept)
{
    Q_Q(NXContentDialog);
    _maskWidget->doMaskAnimation(0);
    isAccept ? q->accept() : q->reject();
}
