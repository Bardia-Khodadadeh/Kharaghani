#include "ejectorboard.h"

EjectorBoard::EjectorBoard()
{

}

EjectorBoard::~EjectorBoard()
{
    sendString = "Z:000000000000";
}


void EjectorBoard::setupConnections()
{
    _lnCup.resize(8);
    _lnCup.fill(0);

    for(int i=0;i<283;i++)
        _cupEjectorNum[i] = -1;


    for(int i=0;i<8;i++) {
        _test.push_back(false);
    }

    connect(_testConnectionTimer, &QTimer::timeout, [this](){
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
    _testConnectionTimer->start(1);


    QObject::connect(_clientSocket, &QTcpSocket::readyRead, this, &EjectorBoard::readDataEthernet);
}

void EjectorBoard::readDataEthernet()
{
    _connectionCounter = 0;
}

void EjectorBoard::setBytesToSend()
{
    sendString = "Z:0000000000000";

    if (_posFinal > _energize && flg_eng){

        for(int i=0;i<8;i++)
        {
            int idx = _cupEjectorNum[(_cup % SharedData::_totalCupNumber + 283 - _distanceFromLC - _lnCup[i]) % 283];
            if(i == idx)
            {
                if(_cupEjectorNum[idx] != -1)
                {
                    sendString[_outputs[idx] + 2] = '1';
                }
            }
            if(_test[i])
            {
                if(_cup % 2 == 0)
                    sendString[_outputs[idx] + 2] = '1';
            }
        }

        flg_ejc = true;
        flg_eng = false;
    }
    if (_posFinal < _energize && !flg_eng)
        flg_eng= true;
}

void EjectorBoard::sendEjectCommand(QString cmnd)
{
    if(_posFinal > _energize)
    {
        writeDataEthernet(cmnd.toUtf8(), TCP);
    }
}

void EjectorBoard::testEachEjector(int ejectorID)
{
    sendString = "Zabcdefghijkl";

    sendString[ejectorID + 1] = sendString[ejectorID + 1].toUpper();

    writeDataEthernet(sendString.toUtf8(), TCP);
    QTimer::singleShot(500, [=](){
        sendString[ejectorID + 1] = sendString[ejectorID + 1].toLower();

        writeDataEthernet(sendString.toUtf8(), TCP);
    });
}

