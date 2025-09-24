#ifndef T_NXSCREEN_H
#define T_NXSCREEN_H
#include <QWidget>

#ifdef Q_OS_WIN
#include "T_BasePage.h"

class NXDxgiScreen;
class NXComboBox;
#if defined(Q_OS_WIN) && defined(NXPACKETIO_LIBRARY_ALREADY_BUILT)
class T_NXPacketIO;
#endif
class T_RecvScreen;
class T_NXScreen : public T_BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit T_NXScreen(QWidget* parent = nullptr);
    ~T_NXScreen() override;

private:
    NXDxgiScreen* _dxgiScreen{nullptr};
    NXComboBox* _dxComboBox{nullptr};
    NXComboBox* _outputComboBox{nullptr};
#if defined(Q_OS_WIN) && defined(NXPACKETIO_LIBRARY_ALREADY_BUILT)
    T_NXPacketIO* _packetSendIO{nullptr};
    T_NXPacketIO* _packetRecvIO{nullptr};
    T_RecvScreen* _recvScreen{nullptr};
    QThread* _packetIOSendThread{nullptr};
    QThread* _packetIORecvThread{nullptr};
    void _initSendThread(QString interfaceIP);
    void _initRecvThread(QString interfaceIP);
    void _unInitThread(bool isSend);
#endif
};
#endif
#endif // T_NXSCREEN_H
