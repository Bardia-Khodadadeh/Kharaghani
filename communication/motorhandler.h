#ifndef MOTORHANDLER_H
#define MOTORHANDLER_H

#include "communication/abstractcontroller.h"
#include "qmodbusdataunit.h"
class MotorHandler: public AbstractController
{
public:
    MotorHandler(int receiverPort, int senderPort, QString ipAddress);
    QString sendStr = "Z:000000000000";
    QString _readStr = "";

    QTimer *_sendTimer = new QTimer(this);
    bool sendFlags[3] = {false, false, false};

    int _connectionCounter = 0;

    QString _engineStatus[9];
    QVector<QString> _b1Status;
    bool startAuto = false;
    bool startFeeding = false;
    bool tenFinished = false;

    bool _calibratingFinished = false;

    QString _speed = "";
    MotorHandler(int port);
    ~MotorHandler();
    void setupConnections();

    int t1Bytes = 0;
    int t2Bytes = 0;

    quint16 calculateCRC(const QByteArray &data);
    QByteArray createModbusFrame(quint8 slaveId, quint8 functionCode, const QModbusDataUnit &unit);
public Q_SLOTS:
    void readDataEthernet();

};

#endif // MOTORHANDLER_H
