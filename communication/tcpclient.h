// tcpclient.h
#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>

class TcpClient : public QObject
{
    Q_OBJECT
public:
    TcpClient(QObject *parent = nullptr);

    void sendAndRecieve(const QString &message);
    void connectTcp(const QString &serverIp, quint16 port, const QString &message);
private:
    QTcpSocket *socket;
};

#endif // TCPCLIENT_H
