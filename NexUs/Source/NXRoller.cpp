#include "NXRoller.h"
#include "private/NXRollerPrivate.h"
#include "NXTheme.h"
#include <QPainter>
#include <QPropertyAnimation>
#include <QWheelEvent>
#include <cmath>

Q_PROPERTY_CREATE_Q_CPP(NXRoller, int, BorderRadius)
NXRoller::NXRoller(QWidget* parent)
    : QWidget{parent}, d_ptr(new NXRollerPrivate())
{
    Q_D(NXRoller);
    d->q_ptr = this;
    d->_pScrollOffset = 0;
    d->_pItemHeight = 35;
    d->_pBorderRadius = 3;
    d->_pMaxVisibleItems = 5;
    d->_pCurrentIndex = 0;
    setFixedSize(90, 175);
    setMouseTracking(true);
    setObjectName("NXRoller");
    setStyleSheet("#NXRoller{background-color:transparent;}");
    QFont font = this->font();
    font.setPixelSize(16);
    setFont(font);

    d->_scrollAnimation = new QPropertyAnimation(d, "pScrollOffset");
    QObject::connect(d->_scrollAnimation, &QPropertyAnimation::valueChanged, this, [=]() {
        update();
    });
    QObject::connect(d->_scrollAnimation, &QPropertyAnimation::finished, this, [=]() {
        while (d->_pScrollOffset < -d->_pItemList.size() * d->_pItemHeight)
        {
            d->_pScrollOffset += d->_pItemList.size() * d->_pItemHeight;
        }
        while (d->_pScrollOffset > d->_pItemList.size() * d->_pItemHeight)
        {
            d->_pScrollOffset -= d->_pItemList.size() * d->_pItemHeight;
        }
        d->_targetScrollOffset = d->_pScrollOffset;
        int currentIndex = 0;
        if (d->_pScrollOffset >= 0)
        {
            currentIndex = d->_pScrollOffset / d->_pItemHeight;
        }
        else if (d->_pScrollOffset <= -d->_pItemHeight)
        {
            currentIndex = d->_pItemList.count() + d->_pScrollOffset / d->_pItemHeight;
        }
        if (currentIndex != d->_pCurrentIndex)
        {
            d->_pCurrentIndex = currentIndex;
            Q_EMIT pCurrentIndexChanged();
        }
        update();
    });

    d->_scrollAnimation->setDuration(300);
    d->_scrollAnimation->setEasingCurve(QEasingCurve::OutCubic);

    d->_themeMode = nxTheme->getThemeMode();
    QObject::connect(nxTheme, &NXTheme::themeModeChanged, this, [=](NXThemeType::ThemeMode themeMode) {
        d->_themeMode = themeMode;
    });
}

NXRoller::~NXRoller()
{
}

QString NXRoller::getCurrentData() const
{
    Q_D(const NXRoller);
    return d->_pItemList[d->_pCurrentIndex];
}

void NXRoller::setItemList(QStringList itemList)
{
    Q_D(NXRoller);
    d->_pItemList = std::move(itemList);
    update();
    Q_EMIT pItemListChanged();
}

QStringList NXRoller::getItemList() const
{
    Q_D(const NXRoller);
    return d->_pItemList;
}

void NXRoller::setItemHeight(int itemHeight)
{
    Q_D(NXRoller);
    d->_pItemHeight = itemHeight;
    setFixedHeight(d->_pMaxVisibleItems * d->_pItemHeight);
    update();
    Q_EMIT pItemHeightChanged();
}

int NXRoller::getItemHeight() const
{
    Q_D(const NXRoller);
    return d->_pItemHeight;
}

void NXRoller::setMaxVisibleItems(int maxVisibleItems)
{
    Q_D(NXRoller);
    d->_pMaxVisibleItems = maxVisibleItems;
    setFixedHeight(d->_pMaxVisibleItems * d->_pItemHeight);
    update();
    Q_EMIT pMaxVisibleItemsChanged();
}

int NXRoller::getMaxVisibleItems() const
{
    Q_D(const NXRoller);
    return d->_pMaxVisibleItems;
}

void NXRoller::setCurrentIndex(int currentIndex)
{
    Q_D(NXRoller);
    if (currentIndex >= d->_pItemList.count())
    {
        return;
    }
    d->_pCurrentIndex = currentIndex;
    d->_pScrollOffset = d->_pItemHeight * currentIndex;
    d->_targetScrollOffset = d->_pScrollOffset;
    update();
    Q_EMIT pCurrentIndexChanged();
}

int NXRoller::getCurrentIndex() const
{
    Q_D(const NXRoller);
    return d->_pCurrentIndex;
}

void NXRoller::wheelEvent(QWheelEvent* event)
{
    Q_D(NXRoller);
    d->_scroll(event->angleDelta().y());
    event->accept();
}

void NXRoller::mouseReleaseEvent(QMouseEvent* event)
{
    Q_D(NXRoller);
    int centerIndex = d->_pMaxVisibleItems / 2;
    int clickIndex = event->pos().y() / d->_pItemHeight;
    int jumpCount = abs(d->_pMaxVisibleItems / 2 - clickIndex);
    if (clickIndex > centerIndex)
    {
        for (int i = 0; i < jumpCount; i++)
        {
            d->_scroll(-120);
        }
    }
    else if (clickIndex < centerIndex)
    {
        for (int i = 0; i < jumpCount; i++)
        {
            d->_scroll(120);
        }
    }
}

void NXRoller::mouseMoveEvent(QMouseEvent* event)
{
    Q_D(NXRoller);
    d->_mousePoint = event->pos();
    update();
    QWidget::mouseMoveEvent(event);
}

void NXRoller::leaveEvent(QEvent* event)
{
    Q_D(NXRoller);
    d->_mousePoint = QPoint();
    update();
    QWidget::leaveEvent(event);
}

void NXRoller::paintEvent(QPaintEvent* event)
{
    Q_D(NXRoller);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    // 背景绘制
    QRect foregroundRect(d->_penBorderWidth, d->_penBorderWidth, width() - 2 * d->_penBorderWidth, height() - 2 * d->_penBorderWidth);
    painter.setPen(QPen(NXThemeColor(d->_themeMode, BasicBorder), d->_penBorderWidth));
    painter.setBrush(isEnabled() ? NXThemeColor(d->_themeMode, BasicBase) : NXThemeColor(d->_themeMode, BasicDisable));
    painter.drawRoundedRect(foregroundRect, d->_pBorderRadius, d->_pBorderRadius);

    // 中心指示区域绘制
    painter.setPen(Qt::NoPen);
    painter.setBrush(NXThemeColor(d->_themeMode, PrimaryNormal));
    bool isEven = d->_pMaxVisibleItems % 2 == 0;
    if (isEven)
    {
        painter.drawRoundedRect(QRect(d->_indicatorMargin, d->_pItemHeight * d->_pMaxVisibleItems / 2 + d->_indicatorMargin, width() - 2 * d->_indicatorMargin, d->_pItemHeight - 2 * d->_indicatorMargin), d->_pBorderRadius, d->_pBorderRadius);
    }
    else
    {
        painter.drawRoundedRect(QRect(d->_indicatorMargin, (height() - d->_pItemHeight) / 2 + d->_indicatorMargin, width() - 2 * d->_indicatorMargin, d->_pItemHeight - 2 * d->_indicatorMargin), d->_pBorderRadius, d->_pBorderRadius);
    }

    int yStart = -d->_pItemHeight;
    int yEnd = height();
    int centerYStart = isEven ? d->_pItemHeight * d->_pMaxVisibleItems / 2 : height() / 2 - d->_pItemHeight;
    int centerYEnd = isEven ? d->_pItemHeight * d->_pMaxVisibleItems / 2 + d->_pItemHeight / 2 : height() / 2;
    int startIndexOffset = d->_pMaxVisibleItems / 2;
    for (int i = 0; i < d->_pItemList.size(); i++)
    {
        qreal y = (i + startIndexOffset) * d->_pItemHeight - d->_pScrollOffset;
        // 范围控制
        while (y <= yStart)
        {
            y += d->_pItemList.size() * d->_pItemHeight;
        }
        while (y >= yEnd)
        {
            y -= d->_pItemList.size() * d->_pItemHeight;
        }
        // 可见区域绘制
        if (y >= yStart && y <= yEnd)
        {
            painter.save();
            painter.translate(0, y);
            // 文字绘制
            if (y >= centerYStart && y <= centerYEnd)
            {
                painter.setPen(NXThemeColor(d->_themeMode, BasicTextInvert));
            }
            else
            {
                // 覆盖效果绘制
                if (!d->_mousePoint.isNull() && d->_scrollAnimation->state() == QPropertyAnimation::State::Stopped)
                {
                    QRectF itemRect = QRectF(0, y, width(), d->_pItemHeight);
                    if (itemRect.contains(d->_mousePoint))
                    {
                        painter.setBrush(NXThemeColor(d->_themeMode, BasicHover));
                        painter.drawRoundedRect(QRect(d->_indicatorMargin, d->_indicatorMargin, width() - 2 * d->_indicatorMargin, d->_pItemHeight - 2 * d->_indicatorMargin), d->_pBorderRadius, d->_pBorderRadius);
                    }
                }
                painter.setPen(NXThemeColor(d->_themeMode, BasicText));
            }
            painter.drawText(QRect(0, 0, width(), d->_pItemHeight), Qt::AlignCenter | Qt::TextSingleLine, d->_pItemList[i]);
            painter.restore();
        }
    }
    painter.restore();
}
