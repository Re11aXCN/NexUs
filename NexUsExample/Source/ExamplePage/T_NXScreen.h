//#ifndef T_NXSCREEN_H
//#define T_NXSCREEN_H
//#include <QWidget>
//
//#ifdef Q_OS_WIN
//#include "T_BasePage.h"
//
//class NXDxgiScreen;
//class NXComboBox;
//class T_NXPacketIO;
//class T_RecvScreen;
//class T_NXScreen : public T_BasePage
//{
//    Q_OBJECT
//public:
//    Q_INVOKABLE explicit T_NXScreen(QWidget* parent = nullptr);
//    ~T_NXScreen() override;
//
//    void initGrabImageHandleThread();
//
//private:
//    T_NXPacketIO* _packetSendIO{nullptr};
//    T_RecvScreen* _recvScreen{nullptr};
//    QThread* _packetIOSendThread{nullptr};
//    T_NXPacketIO* _packetRecvIO{nullptr};
//    QThread* _packetIORecvThread{nullptr};
//    QPixmap _recvPix;
//    NXDxgiScreen* _dxgiScreen{nullptr};
//    NXComboBox* _dxComboBox{nullptr};
//    NXComboBox* _outputComboBox{nullptr};
//
//    void _initSendThread(QString interfaceIP);
//    void _initRecvThread(QString interfaceIP);
//    void _unInitThread(bool isSend);
//};
//#endif
//#endif // T_NXSCREEN_H
