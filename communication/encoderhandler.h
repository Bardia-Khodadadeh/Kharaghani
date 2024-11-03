#ifndef ENCODERHANDLER_H
#define ENCODERHANDLER_H

#include "abstractcontroller.h"

class EncoderHandler : public AbstractController
{
    Q_OBJECT
private:

    bool _flagPrevEncoder = false;
public:
    EncoderHandler(int receiverPort, int senderPort, QString ipAddress);
    EncoderHandler(int port);
    ~EncoderHandler();
    int    _energize= 70;

    int    _shift = 0;
    int    _finalLeft = 0;
    int    _finalRight = 0;
    int    _posFinal = 0;
    int    _prevPosFinal = 0;
    int    pos1{0},pos2{0},pos3{0},pos4{0};
    int _en = 0;
    int _cup = 0;
    int _connectionCounter = 0;

    int _cupEjectorNum[283] = {-1};

    QTimer *_arduinoTimer = new QTimer(this);
    QTimer *_ejectTimer = new QTimer(this);

    bool flg_ejc = true;
    bool flg_eng = false;

    bool flagEject = false;

    int _res[283][8] = {{0}};

    int _count[8] = {0};

    QVector<int> _lnCup;
    QVector<bool> _test;

    int _distanceFromLC;

    QString sendStr;

    void setupConnections();
    void setEncoder(int newEncoder);

    void setBytesToSend();

public Q_SLOTS:
    void readDataEthernet();
    void testEachEjector(int ejectorID);
    void write();
Q_SIGNALS:
    void encoderChanged();
    void read();
    void stringPassed();
    void writeToEjector();
};

#endif // ENCODERHANDLER_H
