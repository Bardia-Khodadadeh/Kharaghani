#include "ejectorhandler.h"

EjectorHandler::EjectorHandler(int receiverPort, int senderPort, QString ipAddress)
    :AbstractController(receiverPort, senderPort, ipAddress)
{
    _udpSocketReceive = new QUdpSocket(this);
    _udpSocketSend = new QUdpSocket(this);

    _udpSocketReceive->bind(_receiverPort, QUdpSocket::ShareAddress);
    _udpSocketSend->bind(_senderPort,QUdpSocket::ShareAddress);

        for(int j=0;j<283;j++)
            for(int k=0;k<8;k++)
                _res[j][k] = false;
}

EjectorHandler::EjectorHandler(int port)
    :AbstractController(port)
{
    for(int i=0;i<283;i++)
        _cupEjectorNum[i] = -1;

}

EjectorHandler::~EjectorHandler()
{
    sendString = "Z:000000000000";
}

void EjectorHandler::setupConnections()
{
    _lnCup.resize(8);
    _lnCup.fill(0);

    for(int i=0;i<8;i++) {
        _test.push_back(false);
    }

    connect(_arduinoTimer, &QTimer::timeout, [this](){
        _connectionCounter++;
        setBytesToSend();
        if(flg_ejc)
        {
            writeDataEthernet(sendString.toUtf8(), TCP);
            flg_ejc = false;
        }
        else if(flagEject)
        {
            writeDataEthernet(sendStr.toUtf8(), TCP);
            flagEject = false;
        }
        else
        {
            writeDataEthernet("T", TCP);
        }
    });
    _arduinoTimer->start(1);

    QObject::connect(_clientSocket, &QTcpSocket::readyRead, this, &EjectorHandler::readDataEthernet);
}

void EjectorHandler::readDataEthernet()
{
   _connectionCounter = 0;
    //qDebug() << _data;
}

void EjectorHandler::setBytesToSend()
{
    sendString = "Z:000000000000";

    if (_posFinal > _energize && flg_eng){
        for(int i=0;i<8;i++)
        {
            int idx = _cupEjectorNum[(_cup % SharedData::_totalCupNumber + 283 - _distanceFromLC - _lnCup[i]) % 283];
            if(i == idx)
            {
                if(_cupEjectorNum[(_cup % SharedData::_totalCupNumber + 283 - _distanceFromLC - _lnCup[i]) % 283] != -1)
                {
                    sendString[_cupEjectorNum[(_cup % SharedData::_totalCupNumber + 283 - _distanceFromLC - _lnCup[i]) % 283] + 2] = '1';
                }
            }
            if(_test[i])
            {
                if(_cup % 2 == 0)
                    sendString[i + 2] = '1';
            }
        }

        flg_ejc = true;
        flg_eng = false;
    }
    if (_posFinal < _energize && !flg_eng)
        flg_eng= true;
}

void EjectorHandler::testEachEjector(int ejectorID)
{
    sendString = "Zabcdefghijkl";

    sendString[ejectorID + 1] = sendString[ejectorID + 1].toUpper();

    writeDataEthernet(sendString.toUtf8(), TCP);
    QTimer::singleShot(500, [=](){
        sendString[ejectorID + 1] = sendString[ejectorID + 1].toLower();

        writeDataEthernet(sendString.toUtf8(), TCP);
    });
}

