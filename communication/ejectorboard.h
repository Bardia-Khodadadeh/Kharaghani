#ifndef EJECTORBOARD_H
#define EJECTORBOARD_H

#include "abstractcontroller.h"
#include "qobjectdefs.h"
#include "qtimer.h"

class EjectorBoard : public AbstractController
{
    Q_OBJECT
private:

    QTimer *_ejectTimer = new QTimer(this);
    QTimer *_testConnectionTimer = new QTimer(this);

    bool flg_ejc = true;
    bool flg_eng = false;

    int _count[8] = {0};


public:
    EjectorBoard();
    ~EjectorBoard();

    int _energize= 70;
    int _posFinal = 0;
    int _cup = 0;
    int _connectionCounter = 0;
    int _shift = 0;


    bool flagEject = false;
    QString _ip{""};
    QVector<int> _outputs;


    int _cupEjectorNum[283] = {-1};

    std::vector<int> _boardOutputs[283];

    QVector<int> _lnCup;
    QVector<bool> _test;

    int _distanceFromLC;

    QString sendStr;

    void setupConnections();
    void setEncoder(int newEncoder);

    void setBytesToSend();

    void sendEjectCommand(QString cmnd);
public Q_SLOTS:
    void readDataEthernet();
    void testEachEjector(int ejectorID);

Q_SIGNALS:
    void sendEjectCommmand(QString cmnd);
};


#endif // EJECTORBOARD_H
