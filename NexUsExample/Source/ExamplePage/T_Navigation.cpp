#include "T_Navigation.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

#include "NXBreadcrumbBar.h"
#include "NXPivot.h"
#include "NXPushButton.h"
#include "NXScrollPageArea.h"
#include "NXTabWidget.h"
#include "NXText.h"
T_Navigation::T_Navigation(QWidget* parent)
    : T_BasePage(parent)
{
    // 预览窗口标题
    setWindowTitle("NXNavigation");

    // 顶部元素
    createCustomWidget("一些导航组件被放置于此，可在此界面体验其效果并按需添加进项目中");

    //NXBreadcrumbBar
    NXText* breadcrumbBarText = new NXText("NXBreadcrumbBar", this);
    breadcrumbBarText->setTextPixelSize(18);
    _breadcrumbBar = new NXBreadcrumbBar(this);
    QStringList breadcrumbBarList;
    for (int i = 0; i < 20; i++)
    {
        breadcrumbBarList << QString("Item%1").arg(i + 1);
    }
    _breadcrumbBar->setBreadcrumbList(breadcrumbBarList);

    NXPushButton* resetButton = new NXPushButton("还原", this);
    resetButton->setFixedSize(60, 32);
    QObject::connect(resetButton, &NXPushButton::clicked, this, [=]() {
        _breadcrumbBar->setBreadcrumbList(breadcrumbBarList);
    });

    QHBoxLayout* breadcrumbBarTextLayout = new QHBoxLayout();
    breadcrumbBarTextLayout->addWidget(breadcrumbBarText);
    breadcrumbBarTextLayout->addSpacing(15);
    breadcrumbBarTextLayout->addWidget(resetButton);
    breadcrumbBarTextLayout->addStretch();

    NXScrollPageArea* breadcrumbBarArea = new NXScrollPageArea(this);
    QVBoxLayout* breadcrumbBarLayout = new QVBoxLayout(breadcrumbBarArea);
    breadcrumbBarLayout->addWidget(_breadcrumbBar);

    // NXPivot
    NXText* pivotText = new NXText("NXPivot", this);
    pivotText->setTextPixelSize(18);
    _pivot = new NXPivot(this);
    _pivot->setPivotSpacing(8);
    _pivot->setMarkWidth(75);
    _pivot->appendPivot("本地歌曲");
    _pivot->appendPivot("下载歌曲");
    _pivot->appendPivot("下载视频");
    _pivot->appendPivot("正在下载");
    _pivot->setCurrentIndex(0);

    NXScrollPageArea* pivotArea = new NXScrollPageArea(this);
    QVBoxLayout* pivotLayout = new QVBoxLayout(pivotArea);
    pivotLayout->addWidget(_pivot);

    // NXTabWidget
    NXText* tabWidgetText = new NXText("NXTabWidget", this);
    tabWidgetText->setTextPixelSize(18);
    _tabWidget = new NXTabWidget(this);
    _tabWidget->setFixedHeight(500);
    QLabel* page1 = new QLabel("新标签页1", this);
    page1->setAlignment(Qt::AlignCenter);
    QFont font = page1->font();
    font.setPixelSize(32);
    page1->setFont(font);
    QLabel* page2 = new QLabel("新标签页2", this);
    page2->setFont(font);
    page2->setAlignment(Qt::AlignCenter);
    QLabel* page3 = new QLabel("新标签页3", this);
    page3->setFont(font);
    page3->setAlignment(Qt::AlignCenter);
    QLabel* page4 = new QLabel("新标签页4", this);
    page4->setFont(font);
    page4->setAlignment(Qt::AlignCenter);
    _tabWidget->addTab(page1, QIcon(":/Resource/Image/Cirno.jpg"), "新标签页1");
    _tabWidget->addTab(page2, "新标签页2");
    _tabWidget->addTab(page3, "新标签页3");
    _tabWidget->addTab(page4, "新标签页4");
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("NXNavigation");
    QVBoxLayout* centerVLayout = new QVBoxLayout(centralWidget);
    centerVLayout->setContentsMargins(0, 0, 0, 0);
    centerVLayout->addLayout(breadcrumbBarTextLayout);
    centerVLayout->addSpacing(10);
    centerVLayout->addWidget(breadcrumbBarArea);
    centerVLayout->addSpacing(15);
    centerVLayout->addWidget(pivotText);
    centerVLayout->addSpacing(10);
    centerVLayout->addWidget(pivotArea);
    centerVLayout->addSpacing(15);
    centerVLayout->addWidget(tabWidgetText);
    centerVLayout->addSpacing(10);
    centerVLayout->addWidget(_tabWidget);
    addCentralWidget(centralWidget, true, false, 0);
}

T_Navigation::~T_Navigation()
{
}
