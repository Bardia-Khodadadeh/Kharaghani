#ifndef ETHERNETCONTROLLER_H
#define ETHERNETCONTROLLER_H

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
#include "camera/cameracontroller.h"
#include "weight/weightcontroller.h"
#include <output.h>
#include <enums.h>
#include "communication/pulsereader.h"
#include "communication/tcp_server.h"

class EthernetController : public QObject, public SharedData
{
    Q_OBJECT
    Q_PROPERTY(int encoder READ getEncoder WRITE setEncoder NOTIFY encoderChanged)
    Q_PROPERTY(QString speed READ getSpeed WRITE setSpeed NOTIFY speedChanged)
    Q_PROPERTY(int cup READ cup WRITE setCup NOTIFY cupChanged)
    Q_PROPERTY(QVector<bool> testLeft READ testLeft WRITE setTestLeft NOTIFY testLeftChanged)
    Q_PROPERTY(QVector<bool> testRight READ testRight WRITE setTestRight NOTIFY testRightChanged)
    Q_PROPERTY(QVector<bool> boardStatus READ boardStatus WRITE setBoardStatus NOTIFY boardStatusChanged)
    Q_PROPERTY(QVector<QString> ruleID READ ruleID WRITE setRuleID NOTIFY ruleIDChanged)
    Q_PROPERTY(QVector<QString> wLow READ wLow WRITE setWLow NOTIFY wLowChanged)
    Q_PROPERTY(QVector<QString> wHigh READ wHigh WRITE setWHigh NOTIFY wHighChanged)
    Q_PROPERTY(QVector<QString> enables READ enables WRITE setEnables NOTIFY enablesChanged)
    Q_PROPERTY(QVector<QString> outputsUsed READ outputsUsed WRITE setOutputsUsed NOTIFY outputsUsedChanged)
    Q_PROPERTY(QVector<QString> sizeDensity READ sizeDensity WRITE setSizeDensity NOTIFY sizeDensityChanged)
    Q_PROPERTY(QVector<int> totalCountRight READ getTotalCountRight WRITE setTotalCountRight NOTIFY totalCountRightChanged)
    Q_PROPERTY(QVector<int> totalCountLeft READ getTotalCountLeft WRITE setTotalCountLeft NOTIFY totalCountLeftChanged)
    Q_PROPERTY(QVector<QString> totalWeightOutput READ getTotalWeightOutput WRITE setTotalWeightOutput NOTIFY totalWeightOutputChanged)
    Q_PROPERTY(QString totalWeight READ getTotalWeight WRITE setTotalWeight NOTIFY totalWeightChanged)
    Q_PROPERTY(GraderStatus graderStatus READ graderStatus WRITE setGraderStatus NOTIFY graderStatusChanged)
    Q_PROPERTY(ConnectionStatus connectionStatus READ connectionStatus WRITE setConnectionStatus NOTIFY connectionStatusChanged)
    Q_PROPERTY(QString elapsedTime READ elapsedTime WRITE setElapsedTime NOTIFY elapsedTimeChanged)
    Q_PROPERTY(bool autoGrading READ autoGrading WRITE setAutoGrading NOTIFY autoGradingChanged)
    Q_PROPERTY(bool autoFeeding READ autoFeeding WRITE setAutoFeeding NOTIFY autoFeedingChanged)
    Q_PROPERTY(int errorCupValueA READ getErrorCupValueA WRITE setErrorCupValueA NOTIFY errorCupValueAChanged)
    Q_PROPERTY(int errorCupValueB READ getErrorCupValueB WRITE setErrorCupValueB NOTIFY errorCupValueBChanged)
    Q_PROPERTY(bool errorCupA READ errorCupA WRITE setErrorCupA NOTIFY errorCupAChanged)
    Q_PROPERTY(bool errorCupB READ errorCupB WRITE setErrorCupB NOTIFY errorCupBChanged)

private:
    QString _speed;

    QString sendToBoard1 = "";
    QString sendToBoard2 = "";

    QVector<QString> motorSymbols = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l"};

    QVector<QString> b1Status;

    QVector<QString> t1Status;
    QVector<QString> t2Status;

    QVector<QString> _ejectorA_Ips;
    QVector<int> _ejectorA_Outputs;

    QVector<QString> _ejectorB_Ips;
    QVector<int> _ejectorB_Outputs;

    QVector<int> _boardOutputs[2][283];

    QVector<QString> engineStatus;

    int _currentCup = 0;

    bool flagAutoFeeding = true;

    Tcp_Server *tcpServer;

    QVector<double> _totalWeights;


    int val_res=-1  , cnt_res=-1 , ind_res=-1;
    int val_res2=-1  , cnt_res2=-1 , ind_res2=-1;

    int _calRounds = 0;

    QTcpServer *_server;
    PulseReader *_pulseReader;

    QVector<QString> ejectSymbols = {"a", "b", "c", "d", "e", "f", "g", "h"};
    QString sendStringA = "";
    QString sendStringB = "";

    bool flg_eng = false;
    bool flg_ejc = true;

    bool programStarted = false;

    QByteArray txtData;

    QByteArray txt;

    int kk = 0;
    int wt = 0;

    bool ejcFlag = false;


    bool _flagPrevEncoder = true;

    QVector<QString> _report;

    QByteArray _data;
    int ccc= 0;

    int c1 = 0;
    int a = 0;
    int b = 0;

    int bytesToSend[4] = {0, 0, 0, 0};

    ConnectionStatus m_connectionStatus = ConnectionStatus::Disconnected;

    WeightController *_weightController;
    CameraController *_cameraController;

    double _wLeft = 0;
    double _wRight = 0;

    Output *_output = new Output();

    DatabaseController *_db = nullptr;

    bool _flagCup = false;

    bool _weightConditions[2][8] = {{false}};
    bool _densityConditions[2][8] = {{false}};
    bool _sizeConditions[2][8] = {{false}};
    bool _colorConditions[2][8] = {{false}};
    bool _defectConditions[2][8] = {{false}};
    ////////////////////////////////////////////////////////////////////
    int _res_L[283][8] = {{0}};
    int _res_R[283][8] = {{0}};

    QString _ejectCommand_L = "z:000000000000";
    QString _ejectCommand_R = "z:000000000000";

    int _maxStringLength = 12;


    /////////////////////////////////////////////////////////////////////

    int _cupID;
    int _ejectCup = 0;

    QVector<int> _lnaCup;
    QVector<int> _lnbCup;

    bool _res[2][283][8] = {{{false}}};

    int _distanceFromLC = 0;

    int m_encoder;
    int _prevPosFinal = 0;
    int _posFinal = 0;
    int _connectionTryCounter = 0;

    int _shift = 0;
    int _energize = 0;

    QTimer *_reportTimer = new QTimer(this);

    QTimer *c1Timer = new QTimer(this);
    QTimer *c2Timer = new QTimer(this);

    QModbusRtuSerialMaster *_modbusDevice;

    QTimer *_connectionTimer = new QTimer(this);
    QTimer *_motorTimer = new QTimer(this);
    QTimer *_ejcectTimer = new QTimer(this);
    QTimer *_encoderTimer = new QTimer(this);

    QTimer *_sendTimer = new QTimer(this);

    QVector<QString> m_wLow;
    QVector<QString> m_wHigh;
    QVector<QString> m_enables;
    QVector<QString> _densityCondition;
    QVector<QString> _sizeCondition;
    QVector<QString> _defectEnables;
    QVector<QString> _colorCondition;

    int m_cup = 0;

    QString exf;

    QString m_elapsedTime = "00:00:00";
    QTime Start_time;
    QString start_report;

    QVector<bool> m_testLeft;
    QVector<bool> m_testRight;

    QVector<int> m_totalCountRight;
    QVector<double> _totalWeightRight;
    QVector<double> _totalWeightLeft;
    QVector<int> m_totalCountLeft;

    QVector<QString> m_outputsUsed;

    QVector<QString> m_sizeDensity;

    QVector<QString> m_ruleID;

    QString m_totalWeight = "0";

    QVector<QString> m_totalWeightOutput;

    bool m_autoGrading;

    bool m_autoFeeding;

    QVector<bool> m_boardStatus;

    int m_errorCupValueA;

    int m_errorCupValueB;

    bool m_errorCupA = false;

    bool m_errorCupB = false;

public:

    long sec;
    long sec_old;

    int _en;

    long sec2;
    long sec_old2;

    explicit EthernetController(QObject *parent = nullptr);
    ~EthernetController();

    Q_ENUM(ConnectionStatus);


    QVector<QString> wLow() const;
    void setWLow(const QVector<QString> &newWLow);

    QVector<QString> wHigh() const;
    void setWHigh(const QVector<QString> &newWHigh);

    void clearArrays();

    QVector<QString> enables() const;
    void setEnables(const QVector<QString> &newEnables);

    int getEncoder() const;

    double wLeft() const;

    double wRight() const;

    void mainLoop();
    bool evaluateColorCondition(const QString &condition, std::vector<int> colorPercents);
    void setupEthernetConnection();
    GraderStatus graderStatus() const;
    void setGraderStatus(GraderStatus newGraderStatus);

    ConnectionStatus connectionStatus() const;
    void setConnectionStatus(ConnectionStatus newConnectionStatus);

    void stepCup();
    void setSizeAndDensity();
    void setColor(int idx, int outputID, QVector<bool> _colorIsEnabled);
    void setWeight(int idx, int outputID);
    void setDefect(int idx, int outputID, QVector<bool> _defectIsEnabled);
    int cup() const;
    void setCup(int newCup);

    QString elapsedTime() const;
    void setElapsedTime(const QString &newElapsedTime);

    void calcElapsedTime();
    void eject();
    QVector<bool> testLeft() const;
    void setTestLeft(const QVector<bool> &newTestLeft);

    QVector<bool> testRight() const;
    void setTestRight(const QVector<bool> &newTestRight);

    QVector<int> getTotalCountRight() const;
    void setTotalCountRight(const QVector<int> &newTotalCountRight);

    QVector<int> getTotalCountLeft() const;
    void setTotalCountLeft(const QVector<int> &newTotalCountLeft);

    QVector<QString> outputsUsed() const;
    void setOutputsUsed(const QVector<QString> &newOutputsUsed);

    QVector<QString> sizeDensity() const;
    void setSizeDensity(const QVector<QString> &newSizeDensity);

    QVector<QString> ruleID() const;
    void setRuleID(const QVector<QString> &newRuleID);

    QString getTotalWeight() const;
    void setTotalWeight(const QString &newTotalWeight);

    void insertReport(QString exf);
    QVector<QString> getTotalWeightOutput() const;
    void setTotalWeightOutput(const QVector<QString> &newTotalWeightOutput);

    void setBytesToSend();
    bool autoGrading() const;
    void setAutoGrading(bool newAutoGrading);

    bool autoFeeding() const;
    void setAutoFeeding(bool newAutoFeeding);

    QString getSpeed() const;
    void setSpeed(const QString &newSpeed);

    QVector<bool> boardStatus() const;
    void setBoardStatus(const QVector<bool> &newBoardStatus);

    int getErrorCupValueA() const;
    void setErrorCupValueA(int newErrorCupValueA);

    int getErrorCupValueB() const;
    void setErrorCupValueB(int newErrorCupValueB);

    bool errorCupA() const;
    void setErrorCupA(bool newErrorCupA);

    bool errorCupB() const;
    void setErrorCupB(bool newErrorCupB);

    void filligArraysAndVars();
    void makeEjectCommand();
public Q_SLOTS:
    void startGrader(QString speed);
    void startManual(int boadrID, int motorID, bool checked);
    void start(QString speed);
    void readDataEthernet(int pos, int prevPos);
    void assignDb(DatabaseController *db);
    void checkConnection();

    void readDb(QString exf);

    void setEncoder(int newEncoder);
    void assignControllers(WeightController *wc, CameraController *cc);
    void stop();
    void applyGeneral(const QVector<QString> &vars);
    void setIdxTestLeft(int idx, bool checked);
    void setIdxTestRight(int idx, bool checked);
    void applyLineA(const QVector<QString> &vars);
    void applyLineB(const QVector<QString> &vars);
    void updateReport();
    void writeToExcel();
    void testEachEjector(int lineID, int ejectorID);

    void checkMotorsStatus();
    void startManualTika(int boadrID, int motorID, bool checked);
    void stopFeeding();
    void stopGrader();
    void checkBoards();
    void setManualStatus(int idx, bool value);
    void setTikaSpeed(QString speed);
    void startFeeding();
    void updateUI();
    void sendEject();
    void encoderPulseChanged();
Q_SIGNALS:
    void wLowChanged();
    void wHighChanged();
    void enablesChanged();
    void encoderChanged();
    void wLeftChanged();
    void wRightChanged();
    void graderStatusChanged();
    void connectionStatusChanged();
    void cupChanged();
    void ethStatusChanged();
    void elapsedTimeChanged();
    void testLeftChanged();
    void testRightChanged();
    void totalCountRightChanged();
    void totalCountLeftChanged();
    void outputsUsedChanged();
    void sizeDensityChanged();
    void ruleIDChanged();
    void totalWeightChanged();
    void totalWeightOutputChanged();
    void textChanged();
    void autoGradingChanged();
    void autoFeedingChanged();
    void speedChanged();
    void boardStatusChanged();
    void errorCupValueAChanged();
    void errorCupValueBChanged();
    void errorCupAChanged();
    void errorCupBChanged();
    void stopChartA();
    void stopChartB();
    void ejectDecisionFinished();

};

#endif // ETHERNETCONTROLLER_H
