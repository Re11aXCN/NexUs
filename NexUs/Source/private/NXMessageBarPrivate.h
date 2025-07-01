#ifndef NXMESSAGEBARPRIVATE_H
#define NXMESSAGEBARPRIVATE_H

#include <QMap>
#include <QObject>
#include <QQueue>
#include <QVariantMap>

#include "NXDef.h"
#include "singleton.h"
#include "stdafx.h"

enum WorkStatus
{
    Idle = 0x0000,
    CreateAnimation = 0x0001,
    OtherEventAnimation = 0x0002,
};

class NXMessageBar;
class NXMessageBarManager : public QObject
{
    Q_OBJECT
    Q_SINGLETON_CREATE_H(NXMessageBarManager)
private:
    explicit NXMessageBarManager(QObject* parent = nullptr);
    ~NXMessageBarManager();

public:
    //请求事件堆栈调用
    void requestMessageBarEvent(NXMessageBar* messageBar);
    //发布创建事件
    void postMessageBarCreateEvent(NXMessageBar* messageBar);
    //发布终止事件
    void postMessageBarEndEvent(NXMessageBar* messageBar);
    //强制发布终止事件
    void forcePostMessageBarEndEvent(NXMessageBar* messageBar);
    //获取当前事件数量
    int getMessageBarEventCount(NXMessageBar* messageBar);
    //更新活动序列
    void updateActiveMap(NXMessageBar* messageBar, bool isActive);

private:
    QMap<NXMessageBar*, QList<QVariantMap>> _messageBarEventMap;
};

class NXIconButton;
class QPainter;
class NXMessageBarPrivate : public QObject
{
    Q_OBJECT
    Q_D_CREATE(NXMessageBar)
    Q_PROPERTY_CREATE(qreal, Opacity)
public:
    explicit NXMessageBarPrivate(QObject* parent = nullptr);
    ~NXMessageBarPrivate();
    void tryToRequestMessageBarEvent();
    WorkStatus getWorkMode() const;
    Q_INVOKABLE void onOtherMessageBarEnd(QVariantMap eventData);
    Q_INVOKABLE void messageBarEnd(QVariantMap eventData);
    Q_SLOT void onCloseButtonClicked();

private:
    friend class NXMessageBarManager;
    NXThemeType::ThemeMode _themeMode;
    int _borderRadius{6};
    QString _title{""};
    QString _text{""};
    NXMessageBarType::PositionPolicy _policy;
    NXMessageBarType::MessageMode _messageMode;
    qreal _createTime{0};

    // 位置数据
    int _leftPadding{20};                // 左边框到图标中心
    int _titleLeftSpacing{30};           // 图标中心到Title左侧
    int _textLeftSpacing{15};            // Title右侧到Text左侧
    int _closeButtonLeftRightMargin{20}; // closeButton左右总Margin
    int _closeButtonWidth{30};
    int _messageBarHorizontalMargin{20};
    int _messageBarVerticalBottomMargin{20};
    int _messageBarVerticalTopMargin{50};
    int _messageBarSpacing{15};
    int _shadowBorderWidth{6};

    // 逻辑数据
    bool _isMessageBarCreateAnimationFinished{false};
    bool _isReadyToEnd{false};
    bool _isNormalDisplay{false};
    bool _isMessageBarEventAnimationStart{false};
    NXIconButton* _closeButton{nullptr};
    Q_INVOKABLE void _messageBarCreate(int displayMsec);

    // 初始坐标计算
    void _calculateInitialPos(int& startX, int& startY, int& endX, int& endY);
    //获取总高度和次序信息
    QList<int> _getOtherMessageBarTotalData(bool isJudgeCreateOrder = false);
    //计算目标坐标
    qreal _calculateTargetPosY();

    //创建次序判断
    bool _judgeCreateOrder(NXMessageBar* otherMessageBar);

    // 绘制函数
    void _drawSuccess(QPainter* painter);
    void _drawWarning(QPainter* painter);
    void _drawInformation(QPainter* painter);
    void _drawError(QPainter* painter);
};

#endif // NXMESSAGEBARPRIVATE_H
