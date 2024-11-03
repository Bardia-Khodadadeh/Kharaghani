#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "qtcpserver.h"
#include "ejectorboard.h"
#include "motorboard.h"
#include <QThread>
#include <opencv2/opencv.hpp>
#include <QMutex>
#include <QObject>
#include <QtConcurrent/QtConcurrent>
#include <QSize>
#include <QImage>
#include <QPainter>
#include <gst/gst.h>
#include <shareddata.h>

class Tcp_Server : public QThread
{
    Q_OBJECT
public:
    Tcp_Server();

    QTcpServer *_server;

    QVector<EjectorBoard*> _ejectorBoards;

    QVector<QString> _clientsIpList = {"::ffff:192.168.0.201", "::ffff:192.168.0.202", "::ffff:192.168.0.203"};

    MotorBoard *_motorBoard{nullptr};

    bool boards[3] = {false, false, false};

    QVector<int> _lnaCup;
    QVector<int> _lnbCup;

    int _distanceFromLC = 0;

    int _energize = 0;

    QVector<bool> _testLeft;
    QVector<bool> _testRight;

    bool flg_ejc = true;

    void setVars();
    void stop();
    void findClients();

    void run() override;
public Q_SLOTS:

Q_SIGNALS:
    void boardStatusChanged();
};

#endif // TCP_SERVER_H
