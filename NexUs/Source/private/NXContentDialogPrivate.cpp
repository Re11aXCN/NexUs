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

void NXContentDialogPrivate::_doCloseAnimation()
{
    Q_Q(NXContentDialog);
    _maskWidget->doMaskAnimation(0);
    q->QDialog::close();
}
