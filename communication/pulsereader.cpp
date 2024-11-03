#include "pulsereader.h"
#include <QObject>

PulseReader::PulseReader()
{
}

void PulseReader::run()
{
    _udpSocketReceive = new QUdpSocket();
    _udpSocketSend = new QUdpSocket();

    _udpSocketReceive->bind(_receiverPort, QUdpSocket::ShareAddress);
    _udpSocketSend->bind(_senderPort,QUdpSocket::ShareAddress);

    QObject::connect(_udpSocketReceive, SIGNAL(readyRead()), this, SLOT(read_encoder_and_weights()));

    QTimer *_arduinoTimer = new QTimer();

    QObject::connect(_arduinoTimer, &QTimer::timeout, this, [=](){
        QString snd = "T";
        writeDataEthernet(snd.toUtf8(), UDP, 1);
    });
    _arduinoTimer->start(1);

    exec();
}

void PulseReader::read_encoder_and_weights()
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
            _prevPosFinal = _posFinal;

            _posFinal = calcFinalPos(datagram);
            setEncoder(_posFinal);

            _connectionCounter = 0;

            if(_flagPrevEncoder) {
                _prevPosFinal = _posFinal;
                setEncoder(_posFinal);
                _flagPrevEncoder = false;
            }

            if (_prevPosFinal > 80 && _posFinal < 20)
            {
                _cup++;
            }

            _finalLeft = calcWeightLeft(datagram);

            _finalRight= calcWeightRight(datagram);
        }
    }
    while(_udpSocketReceive->hasPendingDatagrams());
}


void PulseReader::setEncoder(int newEncoder)
{
    if(_encoder == newEncoder)
        return;
    _encoder = newEncoder;

    Q_EMIT encoderChanged();
}


int PulseReader::calcFinalPos(QByteArray data)
{
    int pos[2] = {0,0};
    for (int i=0; i<2; i++)
        pos[i]=QByteArray::number(data[i+1]).toInt()+128;
    long posFinal = 0;
    posFinal  = pos[0]*128+pos[1];
    posFinal = 399 - posFinal;

    return ((posFinal * 8 * 100 / 400 ) + _shift ) % 100;
}


int PulseReader::calcWeightLeft(QByteArray data)
{
    int wl[4];

    for (int i=3; i<7; i++)
        wl[i - 3]=QByteArray::number(data[i]).toInt()+128;

    return wl[0]*262144+wl[1]*4096+wl[2]*64+wl[3];
}

int PulseReader::calcWeightRight(QByteArray data)
{
    int wr[4];

    for (int i=7; i<11; i++)
        wr[i - 7]=QByteArray::number(data[i]).toInt()+128;

    return wr[0]*262144+wr[1]*4096+wr[2]*64+wr[3];
}

int PulseReader::getFinalPos() const
{
    return _posFinal;
}

int PulseReader::getFinalWeightLeft() const
{
    return _finalLeft;
}

int PulseReader::getFinalWeightRight() const
{
    return _finalRight;
}

