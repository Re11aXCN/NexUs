#include "T_BasePage.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "NXMenu.h"
#include "NXText.h"
#include "NXTheme.h"
#include "NXToolButton.h"
T_BasePage::T_BasePage(QWidget* parent)
    : NXScrollPage(parent)
{
    connect(nxTheme, &NXTheme::themeModeChanged, this, [=]() {
        if (!parent)
        {
            update();
        }
    });
}

T_BasePage::~T_BasePage()
{
}

void T_BasePage::createCustomWidget(QString desText)
{
    // 顶部元素
    QWidget* customWidget = new QWidget(this);
    NXText* subTitleText = new NXText(this);
    subTitleText->setText("https://github.com/Liniyous/NXWidgetTools");
    subTitleText->setTextInteractionFlags(Qt::TextSelectableByMouse);
    subTitleText->setTextPixelSize(11);

    NXToolButton* documentationButton = new NXToolButton(this);
    documentationButton->setFixedHeight(35);
    documentationButton->setIsTransparent(false);
    documentationButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    //_toolButton->setPopupMode(QToolButton::MenuButtonPopup);
    documentationButton->setText("Documentation");
    documentationButton->setNXIcon(NXIconType::FileDoc);
    NXMenu* documentationMenu = new NXMenu(this);
    documentationMenu->addNXIconAction(NXIconType::CardsBlank, "CardsBlank");
    documentationMenu->addNXIconAction(NXIconType::EarthAmericas, "EarthAmericas");
    documentationButton->setMenu(documentationMenu);

    NXToolButton* sourceButton = new NXToolButton(this);
    sourceButton->setFixedHeight(35);
    sourceButton->setIsTransparent(false);
    sourceButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    sourceButton->setText("Source");
    sourceButton->setNXIcon(NXIconType::NfcSymbol);
    NXMenu* sourceMenu = new NXMenu(this);
    sourceMenu->addNXIconAction(NXIconType::FireBurner, "FireBurner");
    sourceMenu->addNXIconAction(NXIconType::Galaxy, "Galaxy~~~~");
    sourceButton->setMenu(sourceMenu);

    NXToolButton* themeButton = new NXToolButton(this);
    themeButton->setFixedSize(35, 35);
    themeButton->setIsTransparent(false);
    themeButton->setNXIcon(NXIconType::MoonStars);
    connect(themeButton, &NXToolButton::clicked, this, [=]() {
        nxTheme->setThemeMode(nxTheme->getThemeMode() == NXThemeType::Light ? NXThemeType::Dark : NXThemeType::Light);
    });

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(documentationButton);
    buttonLayout->addSpacing(5);
    buttonLayout->addWidget(sourceButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(themeButton);
    buttonLayout->addSpacing(15);

    NXText* descText = new NXText(this);
    descText->setText(desText);
    descText->setTextPixelSize(13);

    QVBoxLayout* topLayout = new QVBoxLayout(customWidget);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->addWidget(subTitleText);
    topLayout->addSpacing(5);
    topLayout->addLayout(buttonLayout);
    topLayout->addSpacing(5);
    topLayout->addWidget(descText);
    setCustomWidget(customWidget);
}
