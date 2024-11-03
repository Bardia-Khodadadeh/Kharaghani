#ifndef ABSTRACTCONTROLLER_H
#define ABSTRACTCONTROLLER_H

#include <QThread>
#include <QObject>
#include <QtConcurrent/QtConcurrent>
#include <shareddata.h>
#include <QtNetwork>

class AbstractController : public QThread, public SharedData
{
public:
    AbstractController() = default;
    AbstractController(int receiverPort, int senderPort, QString ipAddress);
    AbstractController(int port);

    QString     _ipAddress;
    QUdpSocket *_udpSocketSend;
    QUdpSocket *_udpSocketReceive;
    int         _receiverPort;
    int         _senderPort;

    QTcpSocket *_clientSocket{nullptr};
    QTcpServer *_server{nullptr};
    int         _port;

    QString sendString = "Zabcdefghijkl";

    void setupEthernetConnection();
    void writeDataEthernet(QByteArray txDatagram, CommunicationType comType, int count = 1, bool flush = false);
    void closeEthernetConnection();
};

#endif // ABSTRACTCONTROLLER_H
