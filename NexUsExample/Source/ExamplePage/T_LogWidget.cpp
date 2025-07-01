#include "T_LogWidget.h"

#include <NXListView.h>

#include <QVBoxLayout>

#include "NXLog.h"
#include "T_LogModel.h"
T_LogWidget::T_LogWidget(QWidget* parent)
    : QWidget{parent}
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 5, 5, 0);
    NXListView* logView = new NXListView(this);
    logView->setIsTransparent(true);
    _logModel = new T_LogModel(this);
    logView->setModel(_logModel);
    mainLayout->addWidget(logView);
    QObject::connect(NXLog::getInstance(), &NXLog::logMessage, this, [=](QString log) {
        _logModel->appendLogList(log);
    });
    _logModel->appendLogList("测试条例11223344556677889900");
    _logModel->appendLogList("测试条例11223344556677889900");
    _logModel->appendLogList("测试条例11223344556677889900");
    _logModel->appendLogList("测试条例11223344556677889900");
}

T_LogWidget::~T_LogWidget()
{
}
