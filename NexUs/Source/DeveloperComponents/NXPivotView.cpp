﻿#include "NXPivotView.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPropertyAnimation>

#include "NXPivotStyle.h"
#include "NXScrollBar.h"
NXPivotView::NXPivotView(QWidget* parent)
    : QListView(parent)
{
    _pMarkX = 0;
    _pMarkWidth = 40;
    _pMarkAnimationWidth = 0;
    _pCurrentIndex = QModelIndex();
    _pCurrentIndexRect = QRect();
    _pIsAnimationFinished = true;
    setObjectName("NXPivotView");
    setStyleSheet("#NXPivotView{background-color:transparent;}");
    setMouseTracking(true);
    setVerticalScrollBar(new NXScrollBar(this));
    setHorizontalScrollBar(new NXScrollBar(this));
    setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
}

NXPivotView::~NXPivotView()
{
}

void NXPivotView::doCurrentIndexChangedAnimation(const QModelIndex& index)
{
    int xOffset = visualRect(_pCurrentIndex).x() - _pCurrentIndexRect.x();
    if (index.row() >= 0)
    {
        _pCurrentIndexRect = visualRect(index);
        QPropertyAnimation* markAnimation = new QPropertyAnimation(this, "pMarkX");
        QObject::connect(markAnimation, &QPropertyAnimation::valueChanged, this, [=]() {
            update();
        });
        QObject::connect(markAnimation, &QPropertyAnimation::finished, this, [=]() {
            _pIsAnimationFinished = true;
            update();
            });
        markAnimation->setDuration(300);
        markAnimation->setEasingCurve(QEasingCurve::InOutSine);
        if (_pPivotStyle->getCurrentIndex() >= 0)
        {
            markAnimation->setStartValue(_pMarkX + xOffset);
            markAnimation->setEndValue(_pCurrentIndexRect.center().x() - _pMarkWidth / 2);
        }
        else
        {
            markAnimation->setStartValue(_pCurrentIndexRect.center().x());
            markAnimation->setEndValue(_pCurrentIndexRect.center().x() - _pMarkWidth / 2);

            QPropertyAnimation* markWidthAnimation = new QPropertyAnimation(this, "pMarkAnimationWidth");
            markWidthAnimation->setDuration(300);
            markWidthAnimation->setEasingCurve(QEasingCurve::InOutSine);
            markWidthAnimation->setStartValue(0);
            markWidthAnimation->setEndValue(_pMarkWidth);
            markWidthAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        }
        markAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else
    {
        _pCurrentIndexRect = visualRect(model()->index(_pPivotStyle->getCurrentIndex(), 0));
        QPropertyAnimation* markAnimation = new QPropertyAnimation(this, "pMarkX");
        QObject::connect(markAnimation, &QPropertyAnimation::valueChanged, this, [=]() {
            update();
        });
        QObject::connect(markAnimation, &QPropertyAnimation::finished, this, [=]() {
            _pIsAnimationFinished = true;
            update();
            });
        markAnimation->setDuration(300);
        markAnimation->setEasingCurve(QEasingCurve::InOutSine);
        markAnimation->setStartValue(_pCurrentIndexRect.center().x() - _pMarkWidth / 2);
        markAnimation->setEndValue(_pCurrentIndexRect.center().x());
        markAnimation->start(QAbstractAnimation::DeleteWhenStopped);

        QPropertyAnimation* markWidthAnimation = new QPropertyAnimation(this, "pMarkAnimationWidth");
        markWidthAnimation->setDuration(300);
        markWidthAnimation->setEasingCurve(QEasingCurve::InOutSine);
        markWidthAnimation->setStartValue(_pMarkAnimationWidth);
        markWidthAnimation->setEndValue(0);
        markWidthAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    _pCurrentIndex = index;
    _pIsAnimationFinished = false;
    setCurrentIndex(index);
}

void NXPivotView::mouseDoubleClickEvent(QMouseEvent* event)
{
    _pPivotStyle->setPressIndex(indexAt(event->pos()));
    viewport()->update();
    QListView::mouseDoubleClickEvent(event);
}

void NXPivotView::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        _pPivotStyle->setPressIndex(QModelIndex());
        viewport()->update();
    }
    QListView::mouseReleaseEvent(event);
}

void NXPivotView::paintEvent(QPaintEvent* event)
{
    QPainter painter(viewport());
    QRect viewPortRect = viewport()->rect();
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(_pPivotStyle->getMarkColor());
    QRect currentIndexRect = visualRect(_pCurrentIndex);
    if (_pIsAnimationFinished)
    {
        painter.drawRoundedRect(QRect(currentIndexRect.center().x() - _pMarkWidth / 2, viewPortRect.bottom() - 4, _pMarkAnimationWidth, 3), 3, 3);
    }
    else
    {
        int xOffset = currentIndexRect.x() - _pCurrentIndexRect.x();
        painter.drawRoundedRect(QRect(_pMarkX + xOffset, viewPortRect.bottom() - 4, _pMarkAnimationWidth, 3), 3, 3);
    }
    painter.restore();
    QListView::paintEvent(event);
}
