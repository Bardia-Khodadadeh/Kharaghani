#ifndef EJECTORHANDLER_H
#define EJECTORHANDLER_H
#include "abstractcontroller.h"
#include "QTimer"

class EjectorHandler : public AbstractController
{
    Q_OBJECT
private:

public:
    EjectorHandler(int receiverPort, int senderPort, QString ipAddress);
    EjectorHandler(int port);
    ~EjectorHandler();

    int _cup = 0;

    QTimer *_arduinoTimer = new QTimer(this);
    QTimer *_ejectTimer = new QTimer(this);

    bool flg_ejc = true;
    bool flg_eng = false;

    int _connectionCounter = 0;

    int _posFinal = 0;

    int _count[8] = {0};

    bool flagEject = false;

    int _res[283][8] = {{0}};

    int _cupEjectorNum[283] = {-1};

    QVector<int> _lnCup;
    QVector<bool> _test;
    int    _energize = 70;

    int _distanceFromLC;

    QString sendStr;

    bool sendFlag = false;


    void setBytesToSend();
public Q_SLOTS:
    void readDataEthernet();
    void setupConnections();
    void testEachEjector(int ejectorID);
Q_SIGNALS:
    void stringPassed();
    void writeToEjector();

};
#endif // EJECTORHANDLER_H
