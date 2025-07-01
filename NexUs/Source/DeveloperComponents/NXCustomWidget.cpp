#include "NXCustomWidget.h"

#include <QPainter>
#include <QVBoxLayout>

#include "NXApplication.h"
#include "NXTheme.h"
Q_TAKEOVER_NATIVEEVENT_CPP(NXCustomWidget, _appBar);
NXCustomWidget::NXCustomWidget(QWidget* parent)
    : QDialog{parent}
{
    resize(500, 500);
    setObjectName("NXCustomWidget");
    _appBar = new NXAppBar(this);
    _appBar->setWindowButtonFlags(NXAppBarType::MinimizeButtonHint |
                                  NXAppBarType::MaximizeButtonHint |
                                  NXAppBarType::CloseButtonHint);

    _mainLayout = new QVBoxLayout(this);
    _mainLayout->setContentsMargins(0, 0, 0, 0);

    _themeMode = nxTheme->getThemeMode();
    QObject::connect(nxTheme, &NXTheme::themeModeChanged, this, [=](NXThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
        update();
    });
    _isEnableMica = nxApp->getIsEnableMica();
    QObject::connect(nxApp, &NXApplication::pIsEnableMicaChanged, this, [=]() {
        _isEnableMica = nxApp->getIsEnableMica();
        update();
        });
    nxApp->syncMica(this);
    setAttribute(Qt::WA_DeleteOnClose);
}

NXCustomWidget::~NXCustomWidget()
{
}

void NXCustomWidget::setCentralWidget(QWidget* widget)
{
    if (!widget)
    {
        return;
    }
    _centralWidget = widget;
    _mainLayout->addWidget(widget);
    widget->setVisible(true);
}

void NXCustomWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(NXThemeColor(_themeMode, WindowBase));
    painter.drawRect(rect());
    painter.restore();
}
