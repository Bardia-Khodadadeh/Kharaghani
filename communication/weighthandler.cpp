#include "weighthandler.h"

WeightHandler::WeightHandler(int receiverPort, int senderPort, QString ipAddress)
    :EthernetReader(receiverPort, senderPort, ipAddress)
{
    _udpSocketReceive = new QUdpSocket(this);
    _udpSocketSend = new QUdpSocket(this);

    _udpSocketReceive->bind(_receiverPort, QUdpSocket::ShareAddress);
    _udpSocketSend->bind(_senderPort,QUdpSocket::ShareAddress);

    connect(_udpSocketReceive, SIGNAL(readyRead()), this, SLOT(readDataEthernet()));

    connect(_arduinoTimer, &QTimer::timeout, [this](){
        QString snd = "T";
        writeDataEthernet(snd.toUtf8(), UDP, 1);
    });
    _arduinoTimer->start(1);
}

WeightHandler::WeightHandler(int port)
    :EthernetReader(port)
{
    _server = new QTcpServer();

    QObject::connect(_server, &QTcpServer::newConnection, [this]() {
        _clientSocket = _server->nextPendingConnection();

        qDebug() << "New connection from:" << _clientSocket->peerAddress().toString() << "Port:" << _clientSocket->peerPort();

        QObject::connect(_clientSocket, &QTcpSocket::readyRead, this, &WeightHandler::readDataEthernet);
    });

    connect(_arduinoTimer, &QTimer::timeout, [this](){
        QString snd = "OK";
        writeDataEthernet(snd.toUtf8(), TCP);
    });
    _arduinoTimer->start(1000);
}

void WeightHandler::readDataEthernet()
{

    QHostAddress senderAddress;
    quint16 senderPort;

    QByteArray datagram;
    do
    {
        datagram.resize(_udpSocketReceive->pendingDatagramSize());
        _udpSocketReceive->readDatagram(datagram.data(), datagram.size(), &senderAddress, &senderPort);

        switch (datagram[0]) {

        case 'P':

            pos4 = pos3;
            pos3 = pos2;
            pos2 = pos1;
            pos1 = _posFinal;
            _prevPosFinal = _posFinal;

            _posFinal = getFinalPos(datagram);
            setEncoder(_posFinal);

            _connectionCounter = 0;
            //qDebug() << "_______" << _posFinal;

            if(_flagPrevEncoder) {
                _prevPosFinal = _posFinal;
                setEncoder(_posFinal);
                _flagPrevEncoder = false;
            }

            if (_prevPosFinal > 80 && _posFinal < 20)
            {
                _cup++;
                //qDebug() << _cup;
            }

            _finalLeft = getFinalWeightLeft(datagram);

            _finalRight= getFinalWeightRight(datagram);

            //Q_EMIT weightChanged();
        }
    }
    while(_udpSocketReceive->hasPendingDatagrams());

}

void WeightHandler::setEncoder(int newEncoder)
{
    if(_encoder == newEncoder)
        return;
    _encoder = newEncoder;

    //qDebug() << "boa\t" << _encoder;

    Q_EMIT encoderChanged();
}


int WeightHandler::getFinalPos(QByteArray data)
{
    int pos[2] = {0,0};
    for (int i=0; i<2; i++)
        pos[i]=QByteArray::number(data[i+1]).toInt()+128;
    long posFinal = 0;
    posFinal  = pos[0]*128+pos[1];
    posFinal = 399 - posFinal;

    return ((posFinal * 8 * 100 / 400 ) + _shift ) % 100;
}


int WeightHandler::getFinalWeightLeft(QByteArray data)
{
    int wl[4];

    for (int i=3; i<7; i++)
        wl[i - 3]=QByteArray::number(data[i]).toInt()+128;

    return wl[0]*262144+wl[1]*4096+wl[2]*64+wl[3];
}

int WeightHandler::getFinalWeightRight(QByteArray data)
{
    int wr[4];

    for (int i=7; i<11; i++)
        wr[i - 7]=QByteArray::number(data[i]).toInt()+128;

    return wr[0]*262144+wr[1]*4096+wr[2]*64+wr[3];
}
