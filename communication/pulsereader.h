#ifndef PULSEREADER_H
#define PULSEREADER_H

#include <QObject>
#include <QThread>
#include "abstractcontroller.h"

class PulseReader : public AbstractController
{
    Q_OBJECT
private:


    bool _flagPrevEncoder = false;


    int  calcWeightLeft(QByteArray data);
    int  calcWeightRight(QByteArray data);
    int  calcFinalPos(QByteArray data);
    void setEncoder(int newEncoder);

public:
    PulseReader();

    int    _finalLeft = 0;
    int    _finalRight = 0;
    int    _shift = 92;
    int    _posFinal = 0;
    int    _prevPosFinal = 0;
    int    _energize = 70;
    int    _connectionCounter = 1;
    int    _cup = 0;

    void run() override;

    int getFinalPos() const;
    int getFinalWeightLeft() const;
    int getFinalWeightRight() const;
public Q_SLOTS:
    void read_encoder_and_weights();

Q_SIGNALS:
    void weightChanged();
    void encoderChanged();
};

#endif // PULSEREADER_H
