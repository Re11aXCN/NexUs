#include "T_UpdateWidget.h"

#include <QVBoxLayout>

#include "NXText.h"
T_UpdateWidget::T_UpdateWidget(QWidget* parent)
    : QWidget{parent}
{
    setMinimumSize(200, 260);
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSizeConstraint(QLayout::SetMaximumSize);
    mainLayout->setContentsMargins(5, 10, 5, 5);
    mainLayout->setSpacing(4);
    NXText* updateTitle = new NXText("2025-10-9更新", 15, this);
    NXText* update1 = new NXText("1、NXTabWidget功能重做", 13, this);
    NXText* update2 = new NXText("2、新增双向路由跳转功能", 13, this);
    NXText* update3 = new NXText("3、新增主要堆栈自定义窗口支持", 13, this);
    NXText* update4 = new NXText("4、Linux部分组件体验优化", 13, this);
    NXText* update5 = new NXText("5、QQ交流群: 850243692", 13, this);
    update1->setIsWrapAnywhere(true);
    update2->setIsWrapAnywhere(true);
    update3->setIsWrapAnywhere(true);
    update4->setIsWrapAnywhere(true);

    mainLayout->addWidget(updateTitle);
    mainLayout->addWidget(update1);
    mainLayout->addWidget(update2);
    mainLayout->addWidget(update3);
    mainLayout->addWidget(update4);
    mainLayout->addWidget(update5);
    mainLayout->addStretch();
}

T_UpdateWidget::~T_UpdateWidget()
{
}
