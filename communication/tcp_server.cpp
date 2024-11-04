#include "tcp_server.h"

Tcp_Server::Tcp_Server() {
    _lnaCup.resize(8);
    _lnaCup.fill(0);

    _lnbCup.resize(8);
    _lnbCup.fill(0);

    for(int i=0;i<8;i++) {
        _testLeft.push_back(false);
        _testRight.push_back(false);
    }

    _ipList.clear();
}

void Tcp_Server::run()
{
    findClients();
}

void Tcp_Server::findClients()
{
    _server = new QTcpServer();

    for(int i=0;i<2;i++)
    {
        EjectorBoard *eb = new EjectorBoard();
        eb->_ipAddress = _clientsIpList[i];

        _ejectorBoards.push_back(eb);
    }

    _motorBoard = new MotorBoard;

    _motorBoard->_ipAddress = _clientsIpList[2];


    if (!_server->listen(QHostAddress::Any, 1250)) {
        qDebug() << "Failed to start server!";
    }
    else
    {
        qDebug() << "Server started! Listening on port 1250.";
    }

    QObject::connect(_server, &QTcpServer::newConnection, [=]() {
        QTcpSocket* clientSocket = _server->nextPendingConnection();
        qDebug() << clientSocket->peerAddress().toString();

        if(clientSocket->peerAddress().toString() == "::ffff:192.168.0.201")
        {
            _ejectorBoards[0]->_clientSocket = clientSocket;
            _ejectorBoards[0]->_ip = "192.168.0.201";
            _ejectorBoards[0]->setupConnections();

            qDebug() << "Ejector Board 1 Connected" << _ejectorBoards[0]->_clientSocket->peerAddress().toString();

            boards[0] = true;
            Q_EMIT boardStatusChanged();
        }
        if(clientSocket->peerAddress().toString() == "::ffff:192.168.0.202")
        {
            _ejectorBoards[1]->_clientSocket = clientSocket;
            _ejectorBoards[1]->_ip = "192.168.0.202";
            _ejectorBoards[1]->setupConnections();

            qDebug() << "Ejector Board 2 Connected" << _ejectorBoards[1]->_clientSocket->peerAddress().toString();

            boards[1] = true;
            Q_EMIT boardStatusChanged();
        }
        if(clientSocket->peerAddress().toString() == "::ffff:192.168.0.203")
        {
            _motorBoard->_clientSocket = clientSocket;
            _motorBoard->setupConnections();

            qDebug() << "Motor Board Connected" << _motorBoard->_clientSocket->peerAddress().toString();

            boards[2] = true;
            Q_EMIT boardStatusChanged();
        }
    });
}

void Tcp_Server::setVars()
{
    _ejectorBoards[0]->_distanceFromLC = _distanceFromLC;
    _ejectorBoards[1]->_distanceFromLC = _distanceFromLC;

    _ejectorBoards[0]->_lnCup = _lnaCup;
    _ejectorBoards[1]->_lnCup = _lnbCup;

    // _encoderHandler->_energize = _energize;
    // _ejectorHandler->_energize = _energize;

    _ejectorBoards[0]->_test = _testLeft;
    _ejectorBoards[1]->_test = _testRight;
}

void Tcp_Server::stop()
{
    _server->close();
}
