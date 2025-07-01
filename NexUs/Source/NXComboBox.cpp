#include "NXComboBox.h"

#include <QAbstractItemView>
#include <QApplication>
#include <QLayout>
#include <QListView>
#include <QMouseEvent>
#include <QPropertyAnimation>

#include "DeveloperComponents/NXComboBoxStyle.h"
#include "NXScrollBar.h"
#include "NXTheme.h"
#include "private/NXComboBoxPrivate.h"
Q_PROPERTY_CREATE_Q_CPP(NXComboBox, int, BorderRadius)
NXComboBox::NXComboBox(QWidget* parent)
    : QComboBox(parent), d_ptr(new NXComboBoxPrivate())
{
    Q_D(NXComboBox);
    setMouseTracking(true);
    d->q_ptr = this;
    d->_pBorderRadius = 3;
    d->_themeMode = nxTheme->getThemeMode();
    setObjectName("NXComboBox");
    setFixedHeight(35);
    d->_comboBoxStyle = std::make_shared<NXComboBoxStyle>(style());
    setStyle(d->_comboBoxStyle.get());

    setView(new QListView(this));
    QAbstractItemView* comboBoxView = this->view();
    comboBoxView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    NXScrollBar* scrollBar = new NXScrollBar(this);
    comboBoxView->setVerticalScrollBar(scrollBar);
    NXScrollBar* floatVScrollBar = new NXScrollBar(scrollBar, comboBoxView);
    floatVScrollBar->setIsAnimation(true);
    comboBoxView->setAutoScroll(false);
    comboBoxView->setSelectionMode(QAbstractItemView::NoSelection);
    comboBoxView->setObjectName("NXComboBoxView");
    comboBoxView->setStyleSheet("#NXComboBoxView{background-color:transparent;}");
    comboBoxView->setStyle(d->_comboBoxStyle.get());
    QWidget* container = this->findChild<QFrame*>();
    if (container)
    {
        container->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
        container->setAttribute(Qt::WA_TranslucentBackground);
        container->setObjectName("NXComboBoxContainer");
        container->setStyle(d->_comboBoxStyle.get());
        QLayout* layout = container->layout();
        while (layout->count())
        {
            layout->takeAt(0);
        }
        layout->addWidget(view());
        layout->setContentsMargins(6, 0, 6, 6);
#ifndef Q_OS_WIN
        container->setStyleSheet("background-color:transparent;");
#endif
    }
    QComboBox::setMaxVisibleItems(5);
    QObject::connect(nxTheme, &NXTheme::themeModeChanged, this, [=](NXThemeType::ThemeMode themeMode) { d->_themeMode = themeMode; });
}

NXComboBox::~NXComboBox()
{
}

void NXComboBox::showPopup()
{
    Q_D(NXComboBox);
    bool oldAnimationEffects = qApp->isEffectEnabled(Qt::UI_AnimateCombo);
    qApp->setEffectEnabled(Qt::UI_AnimateCombo, false);
    QComboBox::showPopup();
    qApp->setEffectEnabled(Qt::UI_AnimateCombo, oldAnimationEffects);
    if (count() > 0)
    {
        QWidget* container = this->findChild<QFrame*>();
        if (container)
        {
            int containerHeight = 0;
            if (count() >= maxVisibleItems())
            {
                containerHeight = maxVisibleItems() * 35 + 8;
            }
            else
            {
                containerHeight = count() * 35 + 8;
            }
            view()->resize(view()->width(), containerHeight - 8);
            container->move(container->x(), container->y() + 3);
            QLayout* layout = container->layout();
            while (layout->count())
            {
                layout->takeAt(0);
            }
            QPropertyAnimation* fixedSizeAnimation = new QPropertyAnimation(container, "maximumHeight");
            QObject::connect(fixedSizeAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
                container->setFixedHeight(value.toUInt());
            });
            fixedSizeAnimation->setStartValue(1);
            fixedSizeAnimation->setEndValue(containerHeight);
            fixedSizeAnimation->setEasingCurve(QEasingCurve::OutCubic);
            fixedSizeAnimation->setDuration(400);
            fixedSizeAnimation->start(QAbstractAnimation::DeleteWhenStopped);

            QPropertyAnimation* viewPosAnimation = new QPropertyAnimation(view(), "pos");
            QObject::connect(viewPosAnimation, &QPropertyAnimation::finished, this, [=]() {
                d->_isAllowHidePopup = true;
                layout->addWidget(view());
            });
            QPoint viewPos = view()->pos();
            viewPosAnimation->setStartValue(QPoint(viewPos.x(), viewPos.y() - view()->height()));
            viewPosAnimation->setEndValue(viewPos);
            viewPosAnimation->setEasingCurve(QEasingCurve::OutCubic);
            viewPosAnimation->setDuration(400);
            viewPosAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        }
        //鎸囩ず鍣ㄥ姩鐢?
        QPropertyAnimation* rotateAnimation = new QPropertyAnimation(d->_comboBoxStyle.get(), "pExpandIconRotate");
        QObject::connect(rotateAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            update();
        });
        rotateAnimation->setDuration(300);
        rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
        rotateAnimation->setStartValue(d->_comboBoxStyle->getExpandIconRotate());
        rotateAnimation->setEndValue(-180);
        rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        QPropertyAnimation* markAnimation = new QPropertyAnimation(d->_comboBoxStyle.get(), "pExpandMarkWidth");
        markAnimation->setDuration(300);
        markAnimation->setEasingCurve(QEasingCurve::InOutSine);
        markAnimation->setStartValue(d->_comboBoxStyle->getExpandMarkWidth());
        markAnimation->setEndValue(width() / 2 - d->_pBorderRadius - 6);
        markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void NXComboBox::hidePopup()
{
    Q_D(NXComboBox);
    if (d->_isAllowHidePopup)
    {
        QWidget* container = this->findChild<QFrame*>();
        int containerHeight = container->height();
        if (container)
        {
            QLayout* layout = container->layout();
            while (layout->count())
            {
                layout->takeAt(0);
            }
            QPropertyAnimation* viewPosAnimation = new QPropertyAnimation(view(), "pos");
            QObject::connect(viewPosAnimation, &QPropertyAnimation::finished, this, [=]() {
                layout->addWidget(view());
                QMouseEvent focusEvent(QEvent::MouseButtonPress, QPoint(-1, -1), QPoint(-1, -1), Qt::NoButton, Qt::NoButton, Qt::NoModifier);
                QApplication::sendEvent(parentWidget(), &focusEvent);
                QComboBox::hidePopup();
                container->setFixedHeight(containerHeight);
            });
            QPoint viewPos = view()->pos();
            QObject::connect(viewPosAnimation, &QPropertyAnimation::finished, this, [=]() { view()->move(viewPos); });
            viewPosAnimation->setStartValue(viewPos);
            viewPosAnimation->setEndValue(QPoint(viewPos.x(), viewPos.y() - view()->height()));
            viewPosAnimation->setEasingCurve(QEasingCurve::InCubic);
            viewPosAnimation->start(QAbstractAnimation::DeleteWhenStopped);

            QPropertyAnimation* fixedSizeAnimation = new QPropertyAnimation(container, "maximumHeight");
            QObject::connect(fixedSizeAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
                container->setFixedHeight(value.toUInt());
            });
            fixedSizeAnimation->setStartValue(container->height());
            fixedSizeAnimation->setEndValue(1);
            fixedSizeAnimation->setEasingCurve(QEasingCurve::InCubic);
            fixedSizeAnimation->start(QAbstractAnimation::DeleteWhenStopped);
            d->_isAllowHidePopup = false;
        }
        //鎸囩ず鍣ㄥ姩鐢?
        QPropertyAnimation* rotateAnimation = new QPropertyAnimation(d->_comboBoxStyle.get(), "pExpandIconRotate");
        QObject::connect(rotateAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            update();
        });
        rotateAnimation->setDuration(300);
        rotateAnimation->setEasingCurve(QEasingCurve::InOutSine);
        rotateAnimation->setStartValue(d->_comboBoxStyle->getExpandIconRotate());
        rotateAnimation->setEndValue(0);
        rotateAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        QPropertyAnimation* markAnimation = new QPropertyAnimation(d->_comboBoxStyle.get(), "pExpandMarkWidth");
        markAnimation->setDuration(300);
        markAnimation->setEasingCurve(QEasingCurve::InOutSine);
        markAnimation->setStartValue(d->_comboBoxStyle->getExpandMarkWidth());
        markAnimation->setEndValue(0);
        markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}
