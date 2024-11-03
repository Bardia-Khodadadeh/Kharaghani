#include "motorboard.h"

MotorBoard::MotorBoard()
{
    for(int i=0;i<9;i++)
    {
        _engineStatus[i] = "OFF";
    }
}

MotorBoard::~MotorBoard()
{
    sendStr = "Z:000000000000";
}

quint16 MotorBoard::calculateCRC(const QByteArray &data) {
    quint16 crc = 0xFFFF;
    for (int i = 0; i < data.size(); ++i) {
        crc ^= static_cast<quint8>(data.at(i));
        for (int j = 0; j < 8; ++j) {
            if (crc & 0x0001) {
                crc >>= 1;
                crc ^= 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}

QByteArray MotorBoard::createModbusFrame(quint8 slaveId, quint8 functionCode, const QModbusDataUnit &unit) {
    QByteArray frame;
    frame.append(slaveId);                                  // Slave ID
    frame.append(functionCode);                             // Function code
    frame.append(static_cast<char>(unit.startAddress() >> 8)); // High byte of start address
    frame.append(static_cast<char>(unit.startAddress() & 0xFF)); // Low byte of start address

    for (size_t i = 0; i < unit.valueCount(); ++i) {
        quint16 value = unit.value(i);
        frame.append(static_cast<char>(value >> 8));       // High byte of value
        frame.append(static_cast<char>(value & 0xFF));     // Low byte of value
    }

    quint16 crc = calculateCRC(frame);
    frame.append(static_cast<char>(crc & 0xFF));           // CRC low byte
    frame.append(static_cast<char>((crc >> 8) & 0xFF));    // CRC high byte

    return frame;
}

void MotorBoard::setupConnections()
{
    connect(_sendTimer, &QTimer::timeout, [=](){
        int a = 0 , b = 0;

        if(_speed == "2")       a = 2;
        else if(_speed == "6")  a = 4;
        else if(_speed == "8")  a = 8;

        //qDebug() << a;

        if(startAuto)
        {
            a = 2;
            b= 16;

            if(tenFinished)
            {
                if(_speed == "2")       a = 2;
                else if(_speed == "6")  a = 4;
                else if(_speed == "8")  a = 8;
            }
        }
        if(startFeeding)
        {
            if(_calibratingFinished)
            {
                for(int i=0;i<_b1Status.size();i++)
                    if(_b1Status[i] == "ON")
                        sendStr[i+2] = '1';

                for(int i=0;i<=7;i++)
                    if(_engineStatus[i] == "ON")
                        if(i != 4)
                            b += pow(2, i);

                if(_engineStatus[8] == "ON") a += 1;
            }
        }
        if(!startFeeding && !startAuto)
        {
            if(_engineStatus[8] == "ON") a += 1;


            for(int i=0;i<=7;i++)
                if(_engineStatus[i] == "ON") b += pow(2, i);
        }

        t1Bytes = b;
        t2Bytes = a;

        _connectionCounter++;
        QTime currentTime = QTime::currentTime();
        int milliseconds = currentTime.msecsSinceStartOfDay() % 1000;
        //qDebug() << sendStr << t1Bytes << t2Bytes;

        int ms2 =  milliseconds % 500;

        QString str = "R8S:";

        if (ms2 < 100){
            sendFlags[0] = true;
            sendFlags[1] = true;
            sendFlags[2] = true;
        }

        if (ms2 > 200 && sendFlags[0]){
            writeDataEthernet(sendStr.toUtf8(), TCP);
            sendFlags[0] = false;
        }

        if (ms2 > 300 && sendFlags[1]){
            QModbusDataUnit writeUnit1(QModbusDataUnit::HoldingRegisters, 232, 1);
            writeUnit1.setValue(0, t1Bytes); // Example value to write
            QByteArray frame1 = createModbusFrame(11, 6, writeUnit1);
            frame1.insert(0, str.toUtf8());
            frame1.insert(frame1.length(), ";");
            writeDataEthernet(frame1, TCP);
            sendFlags[1] = false;
        }

        if (ms2 > 400 && sendFlags[2]){
            QModbusDataUnit writeUnit2(QModbusDataUnit::HoldingRegisters, 232, 1);
            writeUnit2.setValue(0, t2Bytes); // Example value to write
            QByteArray frame2 = createModbusFrame(12, 6, writeUnit2);
            frame2.insert(0, str.toUtf8());
            frame2.insert(frame2.length(), ";");
            writeDataEthernet(frame2, TCP);
            sendFlags[2] = false;
        }
    });
    _sendTimer->start(5);

    QObject::connect(_clientSocket, &QTcpSocket::readyRead, this, &MotorBoard::readDataEthernet);
}

void MotorBoard::readDataEthernet()
{
    _connectionCounter = 0;
}
