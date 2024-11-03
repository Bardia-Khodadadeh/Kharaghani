 #include "lineweighthandler.h"
#include "algorithm"

LineWeightHandler::LineWeightHandler() {
    _positions.resize(283);
    _positions.fill(0);

}

void LineWeightHandler::setVariables(const QVector<QString> &vars)
{
    _kFactor = vars[1].toDouble();

    _minWeight = vars[2].toInt();
    _maxWeight = vars[3].toInt();
    _defaultWeight = vars[4].toInt();

    _minPos = vars[5].toInt();
    _maxPos = vars[6].toInt();

    _errorRange = vars[15].toInt();
}

void LineWeightHandler::applySetting(const QVector<QString> &newSetting)
{
    _kFactor = newSetting[0].toDouble();

    _minWeight = newSetting[1].toInt();
    _maxWeight = newSetting[2].toInt();
    _defaultWeight = newSetting[3].toInt();

    _minPos = newSetting[4].toInt();
    _maxPos = newSetting[5].toInt();

    _errorRange = newSetting[14].toInt();
}

double LineWeightHandler::getCupAverage(const QVector<int> &pos, QVector<double> &weight, int lineID, int _cup)
{
    double k = 0;
    double sum = 0;
    double avg = 0;

    QVector<double> variances;
    QVector<double> weights;

    QVector<double> weightsVariances;


    QVector<int> poses;

    //std::sort(weight.begin(), weight.end());

    // for(int j=0;j<steps;j++)
    // {
    //     // poses.clear();
    //     // weights.clear();
    //     // variances.clear();

    //     weightsVariances.clear();

    //     for(int i=0;i<pos.size();i++)
    //     {
    //         if (pos[i] >= minPos && pos[i] <= maxPos)
    //         {
    //             weightsVariances.push_back(weight[i] / _kFactor);
    //         }

    //     }

    //     variances.push_back(calculateVariance(weightsVariances));
    //     poses.push_back(minPos);


    //     minPos += 2;
    //     maxPos += 2;
    // }

    // auto minElement = std::min_element(variances.constBegin(), variances.constEnd());
    // int minIndex = std::distance(variances.constBegin(), minElement);



    // if(_cup < 283)
    // {
    //     _positions[_cup % 283] = poses[minIndex];
    // }

    // // qDebug() << "Weight Variances: " << variances;
    // // qDebug() << "Weight Weights: " << weight;
    // // qDebug() << "Weight Poses: " << poses;


    // qDebug() << "Weight Positions: " << poses[minIndex] << poses[minIndex] + distance << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // qDebug() << pos;
    // qDebug() << weight;

    // for (int j = 0; j < pos.size(); j++)
    //     if (pos[j] > poses[minIndex] && pos[j] < poses[minIndex] + distance)
    //     {
    //         k++;
    //         weights.push_back(weight[j] / _kFactor);
    //         sum += weight[j] / _kFactor;
    //     }


    for (int j = 0; j < pos.size(); j++)
        if (pos[j] > _minPos && pos[j] < _maxPos)
        {
            k++;
            weights.push_back(weight[j] / _kFactor);
            sum += weight[j] / _kFactor;
        }

    auto start = std::chrono::high_resolution_clock::now();


    QVector<double> output = weights;

    // output = processArrayKalman(output);

    auto end = std::chrono::high_resolution_clock::now();


    //qDebug() << "Weight Positions: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();


    //output = processArrayKalman(output);


    // qDebug() << "W" << weight;
    // qDebug() << "WS" << weights;

    double sum2 = 0;

    for(auto w:output)
        sum2+= w;

    if (k > 0) avg = sum2 / output.size();

    _averageWeight = avg;

    int precision = 2; // Set the desired precision

    // Multiply the double by 10 raised to the power of the precision, round it, and then divide it back
    //_averageWeight = round(_averageWeight * pow(10, precision)) / pow(10, precision);

    //qDebug() << "__________" << _minWeight;

    // if(lineID == 1)
    // {
    //     if(_cup % 283 == 21)
    //     {
    //         qDebug() << avg;
    //         qDebug() << weight;
    //         qDebug() << weights;
    //     }
    // }

    //lineID == 0 ? qDebug() << "L" << (int)_averageWeight : qDebug() << "R" << (int)_averageWeight;

    return _averageWeight;
}

void LineWeightHandler::calibration(const int &cup)
{
    //qDebug() << _errorRange;
    if (cup / SharedData::_totalCupNumber < 1){
        if (_averageWeight > _minWeight && _averageWeight < _maxWeight)
            _tare.push_back(_averageWeight);
        else _tare.push_back(_defaultWeight);
        _prevTare = _tare[cup % SharedData::_totalCupNumber];
    }
    else if (cup / SharedData::_totalCupNumber < 2){
        _prevTare = _tare[cup % SharedData::_totalCupNumber];
        if (abs(_averageWeight - _prevTare) < _errorRange)
            _tare[cup % SharedData::_totalCupNumber] = (_prevTare + _averageWeight) / 2;
    } else {
        _prevTare = _tare[cup % SharedData::_totalCupNumber];
        if (abs(_averageWeight - _prevTare) < _errorRange) {
            _tare[cup % SharedData::_totalCupNumber] = ((2 * _prevTare) + _averageWeight) / 3;
        }
    }
}



double LineWeightHandler::getFinalWeight(int _cup)
{
    //_finalWeight = abs(_averageWeight - _tare[cup]);
    _finalWeight = _averageWeight - _tare[_cup % 283];

    // qDebug() << _cup % 283 << _averageWeight - _prevTare;
    // qDebug() << _cup % 283 << _averageWeight - _tare[_cup % 283];

    return _finalWeight;
}

int LineWeightHandler::minPos() const
{
    return _minPos;
}

void LineWeightHandler::setMinPos(int newMinPos)
{
    _minPos = newMinPos;
}

int LineWeightHandler::maxPos() const
{
    return _maxPos;
}

void LineWeightHandler::setMaxPos(int newMaxPos)
{
    _maxPos = newMaxPos;
}

int LineWeightHandler::kFactor() const
{
    return _kFactor;
}

void LineWeightHandler::setKFactor(int newKFactor)
{
    _kFactor = newKFactor;
}

int LineWeightHandler::minWeight() const
{
    return _minWeight;
}

void LineWeightHandler::setMinWeight(int newMinWeight)
{
    _minWeight = newMinWeight;
}

int LineWeightHandler::maxWeight() const
{
    return _maxWeight;
}

void LineWeightHandler::setMaxWeight(int newMaxWeight)
{
    _maxWeight = newMaxWeight;
}

int LineWeightHandler::defaultWeight() const
{
    return _defaultWeight;
}

void LineWeightHandler::setDefaultWeight(int newDefaultWeight)
{
    _defaultWeight = newDefaultWeight;
}

double LineWeightHandler::averageWeight() const
{
    return _averageWeight;
}

void LineWeightHandler::setAverageWeight(double newAverageWeight)
{
    _averageWeight = newAverageWeight;
}

int LineWeightHandler::errorRange() const
{
    return _errorRange;
}

void LineWeightHandler::setErrorRange(int newErrorRange)
{
    _errorRange = newErrorRange;
}

void LineWeightHandler::setFinalWeight(int newFinalWeight)
{
    _finalWeight = newFinalWeight;
}

QVector<double> LineWeightHandler::tare() const
{
    return _tare;
}

void LineWeightHandler::setTare(const QVector<double> &newTare)
{
    _tare = newTare;
}

int LineWeightHandler::lineID() const
{
    return _lineID;
}

void LineWeightHandler::setLineID(int newLineID)
{
    _lineID = newLineID;
}

double LineWeightHandler::prevTare() const
{
    return _prevTare;
}

void LineWeightHandler::setPrevTare(double newPrevTare)
{
    _prevTare = newPrevTare;
}
