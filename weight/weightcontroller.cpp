#include "weightcontroller.h"
#include <QDebug>

WeightController::WeightController(QObject *parent) : QObject{parent}
{
    m_dataTableA.resize(6);
    m_dataTableA.fill(0);

    m_dataTableB.resize(6);
    m_dataTableB.fill(0);

}

WeightController::~WeightController() {

}

void WeightController::assignDb(DatabaseController *db)
{
    _db = db;
}

void WeightController::readDb(QString exitFeature, QString speed) {
    _db->readGenerals(exitFeature);
    _db->readGeneral();

    _db->readLna(speed);
    _db->readLnb(speed);

    _calibrationRounds = _db->gen()[1].toInt();
    _distanceFromLC = _db->gen()[2].toInt();
}

void WeightController::pushToWeights(const int _lineID, const double &weight)
{
    if(_lines.size() == 2)
        _lineID == 0 ? m_weightLeft.push_back(weight / _lines[_lineID]->kFactor()) : m_weightRight.push_back(weight / _lines[_lineID]->kFactor());
}

void WeightController::start()
{
    _pixId = 0;



    QDir _homeFolder(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
    QString _rootPath = _homeFolder.absolutePath() + "/Density/";

    if(!QDir(_rootPath).exists())
        QDir().mkdir(_rootPath);

    file.setFileName(_rootPath + QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss") + ".txt");

    //qDebug() << QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Could not open file for writing";
    }


    for(int i=0;i<SharedData::_numberOfLines;i++)
    {
        LineWeightHandler *_line = new LineWeightHandler();
        _line->setLineID(i);
        i == 0 ? _line->setVariables(_db->lnaSetting()) : _line->setVariables(_db->lnbSetting());
        _lines.push_back(_line);
    }
}

int WeightController::encoder() const
{
    return _encoder;
}

void WeightController::mainLoop(int _en, int &_cup) {

    if(_lines.size() == 2)
    {
        if (_en < 80 && flg_cup)
        {
            if (_cup == 45 && flg_start == false)
            {
                _cup = 0;
                flg_start = true;
            }
            flg_cup = false;

            if (flg_start)
            {
                if(m_posLeft.size() > 0)
                {
                    m_posLeft.pop_back();
                }
                if(m_weightLeft.size() > 0)
                {
                    m_weightLeft.pop_back();
                }
                if(m_weightRight.size() > 0)
                {
                    m_weightRight.pop_back();
                }
                if(m_posRight.size() > 0)
                {
                    m_posRight.pop_back();
                }

                //qDebug() << "cup" << _cup % 283 << m_posRight << m_weightRight;

                _lines[0]->getCupAverage(m_posLeft, m_weightLeft, 0, _cup);
                _lines[1]->getCupAverage(m_posRight, m_weightRight, 1, _cup);

                calibration(_cup);

                if (_cup >= _calRounds * SharedData::_totalCupNumber)
                {
                    //m_graderStatus = GraderStatus::Grading;

                    int _cupID = (_cup + SharedData::_totalCupNumber - _distanceFromLC) % (SharedData::_totalCupNumber);

                    _output->lines[0].cups[_cupID].weight = _lines[0]->getFinalWeight(_cup);
                    _output->lines[1].cups[_cupID].weight = _lines[1]->getFinalWeight(_cup);

                    QtConcurrent::run([=](){
                        if(_pixId < 1000)
                        {
                            if(_output->lines[0].cups[_cupID].weight > 20)
                            {
                                _pixId++;
                                QTextStream out(&file);
                                out << _lines[0]->getFinalWeight(_cup) << "\t" << _output->lines[0].cups[_cupID].pixCount << Qt::endl;
                            }

                            if(_output->lines[1].cups[_cupID].weight > 20)
                            {
                                _pixId++;
                                QTextStream out(&file);
                                out << _lines[1]->getFinalWeight(_cup) << "\t" << _output->lines[1].cups[_cupID].pixCount << Qt::endl;
                            }
                        }
                    });

                    // if(_lines[1]->getFinalWeight() > 30)
                    //     qDebug() << "_________" << _cup % 283 <<  _lines[1]->getFinalWeight();
                }

                Q_EMIT weightingFinished();


                if(m_flagChartA) updateChartA();
                if(m_flagChartB) updateChartB();

                if(m_dataTableA.size() > 0)
                {
                    if(m_flagTableA){
                        updateTableA();
                    }
                }
                if(m_dataTableB.size() > 0)
                {
                    if(m_flagTableB){
                        updateTableB();
                    }
                }
            }

            //qDebug() << "_____________" << _cup;

            m_posLeft.clear();
            m_weightLeft.clear();
            m_posRight.clear();
            m_weightRight.clear();
        }
        else if (_en > 80) {
            flg_cup = true;
        }
    }

}

void WeightController::calibration(int _cup) {
    if(m_flagTableA) {
        m_dataTableA[0] = QString::number(_cup % SharedData::_totalCupNumber);
        m_dataTableA[1] = QString::number(_lines[0]->averageWeight() * _lines[0]->kFactor());
        m_dataTableA[2] = QString::number(_lines[0]->averageWeight());
    }

    if(m_flagTableB) {
        m_dataTableB[0] = QString::number(_cup % SharedData::_totalCupNumber);
        m_dataTableB[1] = QString::number(_lines[1]->averageWeight() * _lines[1]->kFactor());
        m_dataTableB[2] = QString::number(_lines[1]->averageWeight());
    }

    for(const auto &line:_lines)
        line->calibration(_cup);

    if(m_flagTableA) {
        m_dataTableA[3] = QString::number(_lines[0]->prevTare(),'f',1);
        m_dataTableA[4] = QString::number(_lines[0]->averageWeight() - _lines[0]->tare()[_cup % SharedData::_totalCupNumber],'f',1);
        m_dataTableA[5] = QString::number(_lines[0]->tare()[_cup % SharedData::_totalCupNumber],'f',1);
    }

    if(m_flagTableB) {
        m_dataTableB[3] = QString::number(_lines[1]->prevTare(),'f',1);
        m_dataTableB[4] = QString::number(_lines[1]->averageWeight() - _lines[1]->tare()[_cup % SharedData::_totalCupNumber],'f',1);
        m_dataTableB[5] = QString::number(_lines[1]->tare()[_cup % SharedData::_totalCupNumber],'f',1);
    }
}

void WeightController::updateChartA() {
    double min, max;

    if(m_weightLeft.empty()) {
        min = max = 0;
    } else {
        min = max = m_weightLeft[0];

        for(auto a:m_weightLeft){
            if(a < min)
                min = a;
        }

        for(auto a:m_weightLeft){
            if(a > max)
                max = a;
        }
    }

    m_posLui = m_posLeft;
    m_weightLui = m_weightLeft;

    setMinAxisA(min);
    setMaxAxisA(max);

    Q_EMIT posLeftChanged();
    Q_EMIT weightLeftChanged();
}

void WeightController::updateChartB() {
    double min, max;

    if(m_weightRight.empty()) {
        min = max = 0;
    } else {
        min = max = m_weightRight[0];

        for(auto a:m_weightRight){
            if(a < min)
                min = a;
        }

        for(auto a:m_weightRight){
            if(a > max)
                max = a;
        }
    }

    m_posRui = m_posRight;
    m_weightRui = m_weightRight;

    setMinAxisB(min);
    setMaxAxisB(max);

    Q_EMIT posRightChanged();
    Q_EMIT weightRightChanged();
}

void WeightController::updateTableA() {
    Q_EMIT dataTableAChanged();
}

void WeightController::updateTableB() {
    Q_EMIT dataTableBChanged();
}

void WeightController::stop() {
    qDebug() << "---" << Q_FUNC_INFO;

    flg_cup = false;
    flg_start = false;
    flg_calibration = false;

    _lines.clear();

    m_posLeft.clear();
    m_weightLeft.clear();
    m_posRight.clear();
    m_weightRight.clear();
}

void WeightController::applySetting(int lineNum, QVector<QString> setting)
{
    if(_lines.size() > 0)
        lineNum == 0 ? _lines[0]->applySetting(setting) : _lines[1]->applySetting(setting);
}

const QVector<int> &WeightController::getPosRight() const
{
    return m_posRight;
}

void WeightController::setPosRight(const QVector<int> &newPosRight)
{
    if (m_posRight == newPosRight)
        return;
    m_posRight = newPosRight;
    Q_EMIT posRightChanged();
}

const QVector<double> &WeightController::getWeightRight() const
{
    return m_weightRight;
}

void WeightController::setWeightRight(const QVector<double> &newWeightRight)
{
    if (m_weightRight == newWeightRight)
        return;
    m_weightRight = newWeightRight;
    Q_EMIT weightRightChanged();
}

const QVector<int> &WeightController::getPosLeft() const
{
    return m_posLeft;
}

void WeightController::setPosLeft(const QVector<int> &newPosLeft)
{
    if (m_posLeft == newPosLeft)
        return;
    m_posLeft = newPosLeft;
    Q_EMIT posLeftChanged();
}

const QVector<double> &WeightController::getWeightLeft() const
{
    return m_weightLeft;
}

void WeightController::setWeightLeft(const QVector<double> &newWeightLeft)
{
    if (m_weightLeft == newWeightLeft)
        return;
    m_weightLeft = newWeightLeft;
    Q_EMIT weightLeftChanged();
}

double WeightController::getMinAxisA() const
{
    return m_minAxisA;
}

void WeightController::setMinAxisA(double newMinAxisA)
{
    if (qFuzzyCompare(m_minAxisA, newMinAxisA))
        return;
    m_minAxisA = newMinAxisA;
    Q_EMIT minAxisAChanged();
}

double WeightController::getMaxAxisA() const
{
    return m_maxAxisA;
}

void WeightController::setMaxAxisA(double newMaxAxisA)
{
    if (qFuzzyCompare(m_maxAxisA, newMaxAxisA))
        return;
    m_maxAxisA = newMaxAxisA;
    Q_EMIT maxAxisAChanged();
}

double WeightController::getMinAxisB() const
{
    return m_minAxisB;
}

void WeightController::setMinAxisB(double newMinAxisB)
{
    if (qFuzzyCompare(m_minAxisB, newMinAxisB))
        return;
    m_minAxisB = newMinAxisB;
    Q_EMIT minAxisBChanged();
}

double WeightController::getMaxAxisB() const
{
    return m_maxAxisB;
}

void WeightController::setMaxAxisB(double newMaxAxisB)
{
    if (qFuzzyCompare(m_maxAxisB, newMaxAxisB))
        return;
    m_maxAxisB = newMaxAxisB;
    Q_EMIT maxAxisBChanged();
}

const QVector<QString> &WeightController::getDataTableA() const
{
    return m_dataTableA;
}

void WeightController::setDataTableA(const QVector<QString> &newDataTableA)
{
    if (m_dataTableA == newDataTableA)
        return;
    m_dataTableA = newDataTableA;
    Q_EMIT dataTableAChanged();
}

const QVector<QString> &WeightController::getDataTableB() const
{
    return m_dataTableB;
}

void WeightController::setDataTableB(const QVector<QString> &newDataTableB)
{
    if (m_dataTableB == newDataTableB)
        return;
    m_dataTableB = newDataTableB;
    Q_EMIT dataTableBChanged();
}

bool WeightController::flagTableA() const
{
    return m_flagTableA;
}

void WeightController::setFlagTableA(bool newFlagTableA)
{
    if (m_flagTableA == newFlagTableA)
        return;
    m_flagTableA = newFlagTableA;
    Q_EMIT flagTableAChanged();
}

bool WeightController::flagTableB() const
{
    return m_flagTableB;
}

void WeightController::setFlagTableB(bool newFlagTableB)
{
    if (m_flagTableB == newFlagTableB)
        return;
    m_flagTableB = newFlagTableB;
    Q_EMIT flagTableBChanged();
}

bool WeightController::flagChartA() const
{
    return m_flagChartA;
}

void WeightController::setFlagChartA(bool newFlagChartA)
{
    if (m_flagChartA == newFlagChartA)
        return;
    m_flagChartA = newFlagChartA;
    Q_EMIT flagChartAChanged();
}

bool WeightController::flagChartB() const
{
    return m_flagChartB;
}

void WeightController::setFlagChartB(bool newFlagChartB)
{
    if (m_flagChartB == newFlagChartB)
        return;
    m_flagChartB = newFlagChartB;
    Q_EMIT flagChartBChanged();
}

bool WeightController::errorCupA() const
{
    return m_errorCupA;
}

void WeightController::setErrorCupA(bool newErrorCupA)
{
    if (m_errorCupA == newErrorCupA)
        return;
    m_errorCupA = newErrorCupA;
    Q_EMIT errorCupAChanged();
}

bool WeightController::errorCupB() const
{
    return m_errorCupB;
}

void WeightController::setErrorCupB(bool newErrorCupB)
{
    if (m_errorCupB == newErrorCupB)
        return;
    m_errorCupB = newErrorCupB;
    Q_EMIT errorCupBChanged();
}

int WeightController::getErrorCupValueA() const
{
    return m_errorCupValueA;
}

void WeightController::setErrorCupValueA(int newErrorCupValueA)
{
    if (m_errorCupValueA == newErrorCupValueA)
        return;
    m_errorCupValueA = newErrorCupValueA;
    Q_EMIT errorCupValueAChanged();
}

int WeightController::getErrorCupValueB() const
{
    return m_errorCupValueB;
}

void WeightController::setErrorCupValueB(int newErrorCupValueB)
{
    if (m_errorCupValueB == newErrorCupValueB)
        return;
    m_errorCupValueB = newErrorCupValueB;
    Q_EMIT errorCupValueBChanged();
}

QVector<int> WeightController::posLui() const
{
    return m_posLui;
}

void WeightController::setPosLui(const QVector<int> &newPosLui)
{
    if (m_posLui == newPosLui)
        return;
    m_posLui = newPosLui;
    Q_EMIT posLuiChanged();
}

QVector<double> WeightController::weightLui() const
{
    return m_weightLui;
}

void WeightController::setWeightLui(const QVector<double> &newWeightLui)
{
    if (m_weightLui == newWeightLui)
        return;
    m_weightLui = newWeightLui;
    Q_EMIT weightLuiChanged();
}

QVector<int> WeightController::posRui() const
{
    return m_posRui;
}

void WeightController::setPosRui(const QVector<int> &newPosRui)
{
    if (m_posRui == newPosRui)
        return;
    m_posRui = newPosRui;
    Q_EMIT posRuiChanged();
}

QVector<double> WeightController::weightRui() const
{
    return m_weightRui;
}

void WeightController::setWeightRui(const QVector<double> &newWeightRui)
{
    if (m_weightRui == newWeightRui)
        return;
    m_weightRui = newWeightRui;
    Q_EMIT weightRuiChanged();
}
