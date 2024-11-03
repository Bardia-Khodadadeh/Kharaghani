#ifndef LINEWEIGHTHANDLER_H
#define LINEWEIGHTHANDLER_H

#include "qvector.h"
#include "shareddata.h"

class LineWeightHandler:public SharedData
{
private:
    int    _lineID;
    int    _minPos;
    int    _maxPos;

    double _kFactor;

    int    _minWeight;
    int    _maxWeight;
    int    _defaultWeight;

    double _averageWeight;

    int    _errorRange;

    double _finalWeight;

    double _prevTare = 0;

    QVector<int> _positions;
public:
    LineWeightHandler();

    QVector<double> _tare;

    double getCupAverage(const QVector<int> &pos, QVector<double> &weight, int lineID, int _cup = 0);
    void calibration(const int &cup);
    void setVariables(const QVector<QString> &vars);

    double getFinalWeight(int _cup);
    int minPos() const;
    void setMinPos(int newMinPos);
    int maxPos() const;
    void setMaxPos(int newMaxPos);
    int kFactor() const;
    void setKFactor(int newKFactor);
    int minWeight() const;
    void setMinWeight(int newMinWeight);
    int maxWeight() const;
    void setMaxWeight(int newMaxWeight);
    int defaultWeight() const;
    void setDefaultWeight(int newDefaultWeight);
    double averageWeight() const;
    void setAverageWeight(double newAverageWeight);
    int errorRange() const;
    void setErrorRange(int newErrorRange);
    void setFinalWeight(int newFinalWeight);
    QVector<double> tare() const;
    void setTare(const QVector<double> &newTare);
    int lineID() const;
    void setLineID(int newLineID);
    double prevTare() const;
    void setPrevTare(double newPrevTare);
    void applySetting(const QVector<QString> &newSetting);
};

#endif // LINEWEIGHTHANDLER_H
