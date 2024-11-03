#ifndef WEIGHTHANDLER_H
#define WEIGHTHANDLER_H

#include "ethernetreader.h"

class WeightHandler : public EthernetReader
{
    Q_OBJECT
private:

public:
    WeightHandler(int receiverPort, int senderPort, QString ipAddress);
    WeightHandler(int port);

    int    _finalLeft = 0;
    int    _finalRight = 0;

    int    _shift = 92;
    int    _posFinal = 0;
    int    _prevPosFinal = 0;
    int    pos1{0},pos2{0},pos3{0},pos4{0};

    int    _energize = 70;

    int    _connectionCounter = 1;

    bool _flagPrevEncoder = false;

    int _cup = 0;

    QTimer *_arduinoTimer = new QTimer(this);

    int  getFinalWeightLeft(QByteArray data);
    int  getFinalWeightRight(QByteArray data);
    int getFinalPos(QByteArray data);
    void setEncoder(int newEncoder);
public Q_SLOTS:
    void readDataEthernet();

Q_SIGNALS:
    void weightChanged();
    void encoderChanged();
};


#endif // WEIGHTHANDLER_H
