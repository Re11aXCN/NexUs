#include "NXSpinBox.h"

#include <QContextMenuEvent>
#include <QLineEdit>
#include <QPainter>

#include "DeveloperComponents/NXSpinBoxStyle.h"
#include "NXMenu.h"
#include "private/NXSpinBoxPrivate.h"
#include "NXTheme.h"
#include <QTimer>

NXSpinBox::NXSpinBox(QWidget* parent)
    : QSpinBox(parent), d_ptr(new NXSpinBoxPrivate())
{
    Q_D(NXSpinBox);
    d->q_ptr = this;
    setFixedSize(120, 30);
    d->_spinBoxStyle = std::make_shared<NXSpinBoxStyle>(style());
    setStyle(d->_spinBoxStyle.get());
    lineEdit()->setAlignment(Qt::AlignCenter);
    lineEdit()->setStyleSheet("background-color:transparent");
    connect(nxTheme, &NXTheme::themeModeChanged, this, [=](NXThemeType::ThemeMode themeMode) {
        if (isVisible())
        {
            QPalette palette;
            palette.setColor(QPalette::Base, Qt::transparent);
            palette.setColor(QPalette::Text, NXThemeColor(themeMode, BasicText));
            lineEdit()->setPalette(palette);
        }
        else
        {
            QTimer::singleShot(1, this, [=] {
                QPalette palette;
                palette.setColor(QPalette::Base, Qt::transparent);
                palette.setColor(QPalette::Text, NXThemeColor(themeMode, BasicText));
                lineEdit()->setPalette(palette);
                });
        }
    });
}

NXSpinBox::~NXSpinBox()
{
}

void NXSpinBox::contextMenuEvent(QContextMenuEvent* event)
{
    Q_D(NXSpinBox);
    NXMenu* menu = d->_createStandardContextMenu();
    if (!menu)
    {
        return;
    }
    menu->addSeparator();
    const uint se = stepEnabled();
    QAction* up = menu->addNXIconAction(NXIconType::Plus, tr("增加"));
    up->setEnabled(se & StepUpEnabled);
    QAction* down = menu->addNXIconAction(NXIconType::Minus, tr("減少"));
    down->setEnabled(se & StepDownEnabled);
    menu->addSeparator();

    const QAbstractSpinBox* that = this;
    const QPoint pos = (event->reason() == QContextMenuEvent::Mouse)
        ? event->globalPos()
        : mapToGlobal(QPoint(event->pos().x(), 0)) + QPoint(width() / 2, height() / 2);
    const QAction* action = menu->exec(pos);
    delete menu;
    if (that && action)
    {
        if (action == up)
        {
            stepBy(1);
        }
        else if (action == down)
        {
            stepBy(-1);
        }
    }
    event->accept();
}
