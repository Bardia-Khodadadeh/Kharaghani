#ifndef WEIGHTCONTROLLER_H
#define WEIGHTCONTROLLER_H

#include <QObject>
#include <QtConcurrent/QtConcurrent>
#include <QMutex>
#include <QModbusRtuSerialMaster>
#include <QModbusDataUnit>
#include <QModbusReply>

#include <QCoreApplication>
#include <QMainWindow>
#include <QTimer>
#include <xlsxwriter.h>

#include <QObject>
#include <QtConcurrent/QtConcurrent>
#include <QMutex>
#include <QModbusRtuSerialMaster>
#include <QModbusDataUnit>
#include <QModbusReply>
#include <QJSEngine>

#include <QCoreApplication>
#include <QtNetwork>
#include <QMainWindow>
#include <QTimer>
#include <databasecontroller.h>
#include <output.h>
#include <shareddata.h>

#include "lineweighthandler.h"

class WeightController : public QObject, public SharedData
{
    Q_OBJECT

    Q_PROPERTY(int errorCupValueA READ getErrorCupValueA WRITE setErrorCupValueA NOTIFY errorCupValueAChanged)
    Q_PROPERTY(int errorCupValueB READ getErrorCupValueB WRITE setErrorCupValueB NOTIFY errorCupValueBChanged)

    Q_PROPERTY(QVector<int> posRight READ getPosRight WRITE setPosRight NOTIFY posRightChanged)
    Q_PROPERTY(QVector<double> weightRight READ getWeightRight WRITE setWeightRight NOTIFY weightRightChanged)
    Q_PROPERTY(QVector<int> posLeft READ getPosLeft WRITE setPosLeft NOTIFY posLeftChanged)
    Q_PROPERTY(QVector<double> weightLeft READ getWeightLeft WRITE setWeightLeft NOTIFY weightLeftChanged)

    Q_PROPERTY(QVector<int> posLui READ posLui WRITE setPosLui NOTIFY posLuiChanged)
    Q_PROPERTY(QVector<double> weightLui READ weightLui WRITE setWeightLui NOTIFY weightLuiChanged)
    Q_PROPERTY(QVector<int> posRui READ posRui WRITE setPosRui NOTIFY posRuiChanged)
    Q_PROPERTY(QVector<double> weightRui READ weightRui WRITE setWeightRui NOTIFY weightRuiChanged)

    Q_PROPERTY(double minAxisA READ getMinAxisA WRITE setMinAxisA NOTIFY minAxisAChanged)
    Q_PROPERTY(double maxAxisA READ getMaxAxisA WRITE setMaxAxisA NOTIFY maxAxisAChanged)
    Q_PROPERTY(double minAxisB READ getMinAxisB WRITE setMinAxisB NOTIFY minAxisBChanged)
    Q_PROPERTY(double maxAxisB READ getMaxAxisB WRITE setMaxAxisB NOTIFY maxAxisBChanged)

    Q_PROPERTY(QVector<QString> dataTableA READ getDataTableA WRITE setDataTableA NOTIFY dataTableAChanged)
    Q_PROPERTY(QVector<QString> dataTableB READ getDataTableB WRITE setDataTableB NOTIFY dataTableBChanged)


    Q_PROPERTY(bool flagTableA READ flagTableA WRITE setFlagTableA NOTIFY flagTableAChanged)
    Q_PROPERTY(bool flagTableB READ flagTableB WRITE setFlagTableB NOTIFY flagTableBChanged)

    Q_PROPERTY(bool flagChartA READ flagChartA WRITE setFlagChartA NOTIFY flagChartAChanged)
    Q_PROPERTY(bool flagChartB READ flagChartB WRITE setFlagChartB NOTIFY flagChartBChanged)

    Q_PROPERTY(bool errorCupA READ errorCupA WRITE setErrorCupA NOTIFY errorCupAChanged)
    Q_PROPERTY(bool errorCupB READ errorCupB WRITE setErrorCupB NOTIFY errorCupBChanged)
public:
    explicit WeightController(QObject *parent = nullptr);
    WeightController(const WeightController &) = delete;
    WeightController(WeightController &&) = delete;
    WeightController &operator=(const WeightController &) = delete;
    WeightController &operator=(WeightController &&) = delete;
    ~WeightController();

    bool flg_start = false;

    QVector<LineWeightHandler*> _lines;

    int _pixId = 0;

    QFile file;

    int _calRounds = 0;

    DatabaseController *_db = nullptr;

    bool m_flagTableA = false;

    bool m_flagTableB = false;

    bool m_flagChartA = false;

    bool m_flagChartB = false;

    int _distanceFromLC;

    void calibration(int _cup);

    QVector<int> m_posRight;

    QVector<double> m_weightRight = {0, 0, 0, 0, 0, 0, 0, 0};

    QVector<int> m_posLeft;

    QVector<double> m_weightLeft = {0, 0, 0, 0, 0, 0, 0, 0};

    const QVector<int> &getPosRight() const;
    void setPosRight(const QVector<int> &newPosRight);

    const QVector<double> &getWeightRight() const;
    void setWeightRight(const QVector<double> &newWeightRight);

    const QVector<int> &getPosLeft() const;
    void setPosLeft(const QVector<int> &newPosLeft);

    const QVector<double> &getWeightLeft() const;
    void setWeightLeft(const QVector<double> &newWeightLeft);

    double getMinAxisA() const;
    void setMinAxisA(double newMinAxisA);

    double getMaxAxisA() const;
    void setMaxAxisA(double newMaxAxisA);

    double getMinAxisB() const;
    void setMinAxisB(double newMinAxisB);

    double getMaxAxisB() const;
    void setMaxAxisB(double newMaxAxisB);
    const QVector<QString> &getDataTableA() const;
    void setDataTableA(const QVector<QString> &newDataTableA);

    const QVector<QString> &getDataTableB() const;
    void setDataTableB(const QVector<QString> &newDataTableB);

    bool flagTableA() const;
    void setFlagTableA(bool newFlagTableA);

    bool flagTableB() const;
    void setFlagTableB(bool newFlagTableB);

    bool flagChartA() const;
    void setFlagChartA(bool newFlagChartA);

    bool flagChartB() const;
    void setFlagChartB(bool newFlagChartB);

    bool errorCupA() const;
    void setErrorCupA(bool newErrorCupA);

    bool errorCupB() const;
    void setErrorCupB(bool newErrorCupB);

    int getErrorCupValueA() const;
    void setErrorCupValueA(int newErrorCupValueA);

    int getErrorCupValueB() const;
    void setErrorCupValueB(int newErrorCupValueB);

    int encoder() const;

    void pushToWeights(const int _lineID, const double &weight);
    QVector<int> posLui() const;
    void setPosLui(const QVector<int> &newPosLui);

    QVector<double> weightLui() const;
    void setWeightLui(const QVector<double> &newWeightLui);

    QVector<int> posRui() const;
    void setPosRui(const QVector<int> &newPosRui);

    QVector<double> weightRui() const;
    void setWeightRui(const QVector<double> &newWeightRui);

public Q_SLOTS:
    void start();
    void readDb(QString exitFeature, QString speed);

    void updateChartA();
    void updateChartB();

    void updateTableA();
    void updateTableB();

    void mainLoop(int _en, int &_cup);

    void stop();

    void assignDb(DatabaseController *db);
    void applySetting(int lineNum, QVector<QString> setting);


private:

    int _calibrationRounds;

    bool flg_cup = false;
    bool flg_calibration = false;

    double m_minAxisA;

    double m_maxAxisA;

    double m_minAxisB;

    double m_maxAxisB;

    QVector<QString> m_dataTableA;

    QVector<QString> m_dataTableB;



    bool m_errorCupA = false;

    bool m_errorCupB = false;

    int m_errorCupValueA;

    int m_errorCupValueB;

    int m_light;
    int eth_count = 10;

    QVector<int> m_posLui;

    QVector<double> m_weightLui;

    QVector<int> m_posRui;

    QVector<double> m_weightRui;

Q_SIGNALS:
    void posRightChanged();
    void weightRightChanged();
    void posLeftChanged();
    void weightLeftChanged();
    void minAxisAChanged();
    void maxAxisAChanged();
    void minAxisBChanged();
    void maxAxisBChanged();
    void updateCharts();
    void dataTableAChanged();
    void dataTableBChanged();
    void encoderChanged();
    void reconnectChanged();
    void flagTableAChanged();
    void flagTableBChanged();
    void flagChartAChanged();
    void flagChartBChanged();
    void errorCupAChanged();
    void errorCupBChanged();
    void errorCupValueAChanged();
    void errorCupValueBChanged();
    void calibStatusChanged();
    void weightingFinished();
    void posLuiChanged();
    void weightLuiChanged();
    void posRuiChanged();
    void weightRuiChanged();
};

#endif // WEIGHTCONTROLLER_H
