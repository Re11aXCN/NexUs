#include "NXCentralStackedWidget.h"

#include "NXTheme.h"
#include <QApplication>
#include <QGraphicsBlurEffect>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QTimer>
#include <cmath>
NXCentralStackedWidget::NXCentralStackedWidget(QWidget* parent)
    : QStackedWidget(parent)
{
    _pPopupAnimationYOffset = 0;
    _pScaleAnimationRatio = 1;
    _pScaleAnimationPixOpacity = 1;
    _pFlipAnimationRatio = 1;
    _pBlurAnimationRadius = 0;

    _blurEffect = new QGraphicsBlurEffect(this);
    _blurEffect->setBlurHints(QGraphicsBlurEffect::BlurHint::QualityHint);
    _blurEffect->setBlurRadius(0);
    _blurEffect->setEnabled(false);
    setGraphicsEffect(_blurEffect);

    setObjectName("NXCentralStackedWidget");
    setStyleSheet("#NXCentralStackedWidget{background-color:transparent;}");
    _themeMode = nxTheme->getThemeMode();
    QObject::connect(nxTheme, &NXTheme::themeModeChanged, this, &NXCentralStackedWidget::onThemeModeChanged);
}

NXCentralStackedWidget::~NXCentralStackedWidget()
{
}

void NXCentralStackedWidget::onThemeModeChanged(NXThemeType::ThemeMode themeMode)
{
    _themeMode = themeMode;
}

void NXCentralStackedWidget::setIsTransparent(bool isTransparent)
{
    this->_isTransparent = isTransparent;
    update();
}

bool NXCentralStackedWidget::getIsTransparent() const
{
    return _isTransparent;
}

void NXCentralStackedWidget::setIsHasRadius(bool isHasRadius)
{
    this->_isHasRadius = isHasRadius;
    update();
}

void NXCentralStackedWidget::doWindowStackSwitch(NXWindowType::StackSwitchMode stackSwitchMode, int nodeIndex, bool isRouteBack)
{
    _stackSwitchMode = stackSwitchMode;
    switch (stackSwitchMode)
    {
    case NXWindowType::None:
    {
        this->setCurrentIndex(nodeIndex);
        break;
    }
    case NXWindowType::Popup:
    {
        QTimer::singleShot(180, this, [=]() {
            QWidget* targetWidget = this->widget(nodeIndex);
            this->setCurrentIndex(nodeIndex);
            _getTargetStackPix();
            targetWidget->setVisible(false);
            QPropertyAnimation* popupAnimation = new QPropertyAnimation(this, "pPopupAnimationYOffset");
            QObject::connect(popupAnimation, &QPropertyAnimation::valueChanged, this, [=]() {
                update();
            });
            QObject::connect(popupAnimation, &QPropertyAnimation::finished, this, [=]() {
                _targetStackPix = QPixmap();
                targetWidget->setVisible(true);
            });
            popupAnimation->setEasingCurve(QEasingCurve::OutCubic);
            popupAnimation->setDuration(300);
            int targetWidgetY = targetWidget->y();
            popupAnimation->setEndValue(targetWidgetY);
            targetWidgetY += 80;
            popupAnimation->setStartValue(targetWidgetY);
            popupAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        });
        break;
    }
    case NXWindowType::Scale:
    {
        QWidget* targetWidget = this->widget(nodeIndex);
        _getCurrentStackPix();
        this->setCurrentIndex(nodeIndex);
        _getTargetStackPix();
        targetWidget->setVisible(false);
        _isDrawNewPix = false;
        QPropertyAnimation* currentPixZoomAnimation = new QPropertyAnimation(this, "pScaleAnimationRatio");
        QObject::connect(currentPixZoomAnimation, &QPropertyAnimation::valueChanged, this, [=]() {
            update();
        });
        QObject::connect(currentPixZoomAnimation, &QPropertyAnimation::finished, this, [=]() {
            _isDrawNewPix = true;
            QPropertyAnimation* targetPixZoomAnimation = new QPropertyAnimation(this, "pScaleAnimationRatio");
            QObject::connect(targetPixZoomAnimation, &QPropertyAnimation::valueChanged, this, [=]() {
                update();
            });
            QObject::connect(targetPixZoomAnimation, &QPropertyAnimation::finished, this, [=]() {
                _targetStackPix = QPixmap();
                _currentStackPix = QPixmap();
                targetWidget->setVisible(true);
            });
            if (isRouteBack)
            {
                targetPixZoomAnimation->setStartValue(1.5);
                targetPixZoomAnimation->setEndValue(1);
            }
            else
            {
                // 放大
                targetPixZoomAnimation->setStartValue(0.85);
                targetPixZoomAnimation->setEndValue(1);
            }
            targetPixZoomAnimation->setDuration(300);
            targetPixZoomAnimation->setEasingCurve(QEasingCurve::OutCubic);
            targetPixZoomAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        });
        if (isRouteBack)
        {
            // 缩小
            currentPixZoomAnimation->setStartValue(1);
            currentPixZoomAnimation->setEndValue(0.85);
        }
        else
        {
            // 放大
            currentPixZoomAnimation->setStartValue(1);
            currentPixZoomAnimation->setEndValue(1.15);
        }
        currentPixZoomAnimation->setDuration(150);
        currentPixZoomAnimation->start(QAbstractAnimation::DeleteWhenStopped);

        QPropertyAnimation* currentPixOpacityAnimation = new QPropertyAnimation(this, "pScaleAnimationPixOpacity");
        QObject::connect(currentPixZoomAnimation, &QPropertyAnimation::finished, this, [=]() {
            QPropertyAnimation* targetPixOpacityAnimation = new QPropertyAnimation(this, "pScaleAnimationPixOpacity");
            targetPixOpacityAnimation->setStartValue(0);
            targetPixOpacityAnimation->setEndValue(1);
            targetPixOpacityAnimation->setDuration(300);
            targetPixOpacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        });
        currentPixOpacityAnimation->setStartValue(1);
        currentPixOpacityAnimation->setEndValue(0);
        currentPixOpacityAnimation->setDuration(150);
        currentPixOpacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        break;
    }
    case NXWindowType::Flip:
    {
        QWidget* targetWidget = this->widget(nodeIndex);
        _getCurrentStackPix();
        this->setCurrentIndex(nodeIndex);
        _getTargetStackPix();
        targetWidget->setVisible(false);
        QPropertyAnimation* flipAnimation = new QPropertyAnimation(this, "pFlipAnimationRatio");
        QObject::connect(flipAnimation, &QPropertyAnimation::valueChanged, this, [=]() {
            update();
        });
        QObject::connect(flipAnimation, &QPropertyAnimation::finished, this, [=]() {
            _targetStackPix = QPixmap();
            _currentStackPix = QPixmap();
            targetWidget->setVisible(true);
        });
        flipAnimation->setEasingCurve(QEasingCurve::InOutSine);
        flipAnimation->setDuration(650);
        flipAnimation->setStartValue(0);
        flipAnimation->setEndValue(isRouteBack ? -180 : 180);
        flipAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        break;
    }
    case NXWindowType::Blur:
    {
        _targetStackPix = QPixmap();
        _blurEffect->setEnabled(true);
        QPropertyAnimation* blurAnimation = new QPropertyAnimation(this, "pBlurAnimationRadius");
        QObject::connect(blurAnimation, &QPropertyAnimation::valueChanged, this, [=]() {
            _blurEffect->setBlurRadius(_pBlurAnimationRadius);
        });
        QObject::connect(blurAnimation, &QPropertyAnimation::finished, this, [=]() {
            _blurEffect->setEnabled(false);
        });
        blurAnimation->setEasingCurve(QEasingCurve::InOutSine);
        blurAnimation->setDuration(350);
        blurAnimation->setStartValue(40);
        blurAnimation->setEndValue(2);
        blurAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        QApplication::processEvents();
        this->setCurrentIndex(nodeIndex);
        break;
    }
    }
}

void NXCentralStackedWidget::paintEvent(QPaintEvent* event)
{
    QRect targetRect = this->rect();
    targetRect.adjust(1, 1, 10, 10);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing);
    if (!_isTransparent)
    {
        painter.setPen(QPen(NXThemeColor(_themeMode, BasicBaseLine), 1.5));
        painter.setBrush(NXThemeColor(_themeMode, WindowCentralStackBase));
        if (_isHasRadius)
        {
            painter.drawRoundedRect(targetRect, 10, 10);
        }
        else
        {
            painter.drawRect(targetRect);
        }
    }
    // 切换动画
    if (!_targetStackPix.isNull())
    {
        QPainterPath clipPath;
        clipPath.addRoundedRect(targetRect, 10, 10);
        painter.setClipPath(clipPath);
        switch (_stackSwitchMode)
        {
        case NXWindowType::None:
        {
            break;
        }
        case NXWindowType::Popup:
        {
            painter.drawPixmap(QRect(0, _pPopupAnimationYOffset, width(), height()), _targetStackPix);
            break;
        }
        case NXWindowType::Scale:
        {
            painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
            painter.setOpacity(_pScaleAnimationPixOpacity);
            painter.translate(rect().center());
            painter.scale(_pScaleAnimationRatio, _pScaleAnimationRatio);
            painter.translate(-rect().center());
            painter.drawPixmap(rect(), _isDrawNewPix ? _targetStackPix : _currentStackPix);
            break;
        }
        case NXWindowType::Flip:
        {
            painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
            QTransform transform;
            transform.translate(rect().center().x(), 0);
            if (abs(_pFlipAnimationRatio) >= 90)
            {
                transform.rotate(-180 + _pFlipAnimationRatio, Qt::YAxis);
            }
            else
            {
                transform.rotate(_pFlipAnimationRatio, Qt::YAxis);
            }
            transform.translate(-rect().center().x(), 0);
            painter.setTransform(transform);
            if (abs(_pFlipAnimationRatio) >= 90)
            {
                painter.drawPixmap(rect(), _targetStackPix);
            }
            else
            {
                painter.drawPixmap(rect(), _currentStackPix);
            }
            break;
        }
        case NXWindowType::Blur:
        {
            break;
        }
        }
    }
    painter.restore();
}

void NXCentralStackedWidget::_getCurrentStackPix()
{
    _targetStackPix = QPixmap();
    bool isTransparent = _isTransparent;
    _isTransparent = true;
    currentWidget()->setVisible(true);
    _currentStackPix = this->grab(rect());
    currentWidget()->setVisible(false);
    _isTransparent = isTransparent;
}

void NXCentralStackedWidget::_getTargetStackPix()
{
    _targetStackPix = QPixmap();
    bool isTransparent = _isTransparent;
    _isTransparent = true;
    _targetStackPix = this->grab(rect());
    _isTransparent = isTransparent;
}
