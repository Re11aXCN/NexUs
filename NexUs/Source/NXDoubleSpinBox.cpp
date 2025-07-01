#include "NXDoubleSpinBox.h"

#include "DeveloperComponents/NXSpinBoxStyle.h"
#include "private/NXDoubleSpinBoxPrivate.h"
#include "NXMenu.h"
#include "NXTheme.h"
#include <QContextMenuEvent>
#include <QLineEdit>
#include <QTimer>
NXDoubleSpinBox::NXDoubleSpinBox(QWidget* parent)
    : QDoubleSpinBox(parent), d_ptr(new NXDoubleSpinBoxPrivate())
{
    Q_D(NXDoubleSpinBox);
    d->q_ptr = this;
    setFixedSize(120, 30);
    setStyle(new NXSpinBoxStyle(style()));
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

NXDoubleSpinBox::~NXDoubleSpinBox()
{
}

void NXDoubleSpinBox::contextMenuEvent(QContextMenuEvent* event)
{
    Q_D(NXDoubleSpinBox);
    NXMenu* menu = d->_createStandardContextMenu();
    if (!menu)
    {
        return;
    }
    menu->addSeparator();
    const uint se = stepEnabled();
    QAction* up = menu->addNXIconAction(NXIconType::Plus, tr("增加"));
    up->setEnabled(se & StepUpEnabled);
    QAction* down = menu->addNXIconAction(NXIconType::Minus, tr("减少"));
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
