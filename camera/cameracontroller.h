#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <QObject>
#include <QtConcurrent/QtConcurrent>
#include <opencv2/opencv.hpp>
#include "capturethread.h"
#include "processor/processor.h"
#include <databasecontroller.h>
#include "tcamcamera.h"
#include <vector>
#include <QMutex>

#include <QCoreApplication>
#include <QtNetwork>
#include <iostream>
#include <iomanip>
#include <QMainWindow>
#include <QTimer>

#include <databasecontroller.h>
#include <gst/video/video-info.h>
#include <gst/gst.h>
#include <tcam-property-1.0.h>
#include <opencv2/opencv.hpp>
#include <output.h>

using namespace std;

struct setupCamera //TODO
{
    const int numOfCamera = 2;//2
    const int numOfClasses = 9;
    const int batchSize = 14;//6
    const int width = 1440;
    const int height = 360; // we have a width and heigth in capturethread
    const QString systemPassword = "7410"; //MUST
    const QString modelPath = "/home/perticon/Application/best_.torchscript";
    const bool multiFrame = true;

    const int batchIDIgnogance = 30;

    // const QVector<QVector<int>> grabTimes =     {{92,98},{37,43}};
    // const QVector<QVector<int>> retrieveTimes = {{30,37},{75,83}};
    // const QVector<QVector<int>> processTime = {{84,91},{-1,-1}};
    const QVector<QVector<int>> grabTimes =     {{92,98},{42,48}};
    const QVector<QVector<int>> retrieveTimes = {{36,42},{86,92}};
    const QVector<QVector<int>> processTime = {{92,98},{-1,-1}};

    //    const QVector<QVector<int>> grabTimes =     {{10,16},{-1,-1}};
    //    const QVector<QVector<int>> retrieveTimes = {{26,32},{-1,-1}};
    //    const QVector<QVector<int>> processTime = {{33,39},{-1,-1}};

    const int receiverPort = 8569;
    const int senderPort = 8570;
    const int maxEjectorLine = 30;

};

struct camPack
{
    CaptureThread *capture;
    bool _frameDelivered = true;
    int frameNumber = -1;
    bool status = true;
    cv::Mat orgFrame = cv::Mat::ones(1, 1, CV_8UC3);
    cv::Mat lastFrameMat = cv::Mat::ones(1, 1, CV_8UC3);
};

class CameraController : public QObject, public SharedData
{
    Q_OBJECT
    Q_PROPERTY(QVector<QString> serials READ serials WRITE setSerials NOTIFY serialsChanged)
    Q_PROPERTY(bool analyseStatue READ analyseStatue WRITE setAnalyseStatue NOTIFY analyseStatueChanged)
    Q_PROPERTY(bool readyToProcess READ readyToProcess WRITE setReadyToProcess NOTIFY readyToProcessChanged)
    Q_PROPERTY(bool saveResult READ saveResult WRITE setSaveResult NOTIFY saveResultChanged)
    Q_PROPERTY(QVector<QString> classNamesList READ classNamesList WRITE setClassNamesList NOTIFY classNamesListChanged)
    Q_PROPERTY(QVector<bool> classEnableList READ classEnableList WRITE setClassEnableList NOTIFY classEnableListChanged)
    Q_PROPERTY(QVector<int> classConfList READ classConfList WRITE setClassConfList NOTIFY classConfListChanged)
    Q_PROPERTY(QVector<int> classesCount READ classesCount WRITE setClassesCount NOTIFY classesCountChanged)
    Q_PROPERTY(int ejectorPosition READ ejectorPosition WRITE setEjectorPosition NOTIFY ejectorPositionChanged)
    Q_PROPERTY(int energize READ energize WRITE setEnergize NOTIFY energizeChanged)
    Q_PROPERTY(int deEnergize READ deEnergize WRITE setDeEnergize NOTIFY deEnergizeChanged)
    Q_PROPERTY(int batchID READ batchID WRITE setBatchID NOTIFY batchIDChanged)
    Q_PROPERTY(bool ardStatus READ ardStatus WRITE setArdStatus NOTIFY ardStatusChanged)
    Q_PROPERTY(bool ethStatus READ ethStatus WRITE setEthStatus NOTIFY ethStatusChanged) //E
    Q_PROPERTY(bool quantityEnable READ quantityEnable WRITE setQuantityEnable NOTIFY quantityEnableChanged)
    Q_PROPERTY(QVector<bool> classQuantityEnabledList READ classQuantityEnabledList WRITE setclassQuantityEnabledList NOTIFY classQuantityEnabledListChanged)
    Q_PROPERTY(QVector<int> quantityBasedClassCountList READ quantityBasedClassCountList WRITE setQuantityBasedClassCountList NOTIFY quantityBasedClassCountListChanged)
    Q_PROPERTY(QVector<float> quantityBasedClassConfList READ quantityBasedClassConfList WRITE setQuantityBasedClassConfList NOTIFY quantityBasedClassConfListChanged)
    Q_PROPERTY(int total READ total WRITE setTotal NOTIFY totalChanged)
    Q_PROPERTY(QVector<bool> showChecks READ showChecks WRITE setShowChecks NOTIFY showChecksChanged)
    Q_PROPERTY(QVector<int> colorPercents READ colorPercents WRITE setColorPercents NOTIFY colorPercentsChanged)
    Q_PROPERTY(QVector<bool> showChecks READ showChecks WRITE setShowChecks NOTIFY showChecksChanged)
    Q_PROPERTY(QVector<bool> cameraIsOpened READ cameraIsOpened WRITE setCameraIsOpened NOTIFY cameraIsOpenedChanged)
    Q_PROPERTY(QVector<bool> cameraIsStarted READ cameraIsStarted WRITE setCameraIsStarted NOTIFY cameraIsStartedChanged)
    Q_PROPERTY(QVector<QString> serials READ serials WRITE setSerials NOTIFY serialsChanged)
    Q_PROPERTY(bool startCameras READ startCameras WRITE setStartCameras NOTIFY startCamerasChanged)


    Q_PROPERTY(QVector<QString> frameIdx READ frameIdx WRITE setFrameIdx NOTIFY frameIdxChanged)

    Q_PROPERTY(QVector<QString> cropHsv READ cropHsv WRITE setCropHsv NOTIFY cropHsvChanged)
    Q_PROPERTY(QVector<QString> rdHsv READ rdHsv WRITE setRdHsv NOTIFY cropHsvChanged)
    Q_PROPERTY(QVector<QString> orHsv READ orHsv WRITE setOrHsv NOTIFY orHsvChanged)
    Q_PROPERTY(QVector<QString> ylHsv READ ylHsv WRITE setYLHsv NOTIFY ylHsvChanged)
    Q_PROPERTY(QVector<QString> gnHsv READ gnHsv WRITE setGnHsv NOTIFY gnHsvChanged)

public:
    explicit CameraController(QObject *parent = nullptr);
    ~CameraController();
    int prevcup;
    bool _retrieveFlag = false;
    bool _grabFlag = false;

    bool m_startCameras;

    QVector<QString> m_serials;

    bool showFlag = false;

    QVector<bool> m_cameraIsStarted;
    int camerasStarted = false;
    int watcherCounter = 0;

    DatabaseController *_db = nullptr;

    setupCamera mySetup;

    void stepCup();

    const QVector<QString> &serials() const;
    void setSerials(const QVector<QString> &newSerials);

    bool analyseStatue() const;
    void setAnalyseStatue(bool newAnalyseStatue);
    bool m_analyseStatue{false};

    bool readyToProcess() const;
    void setReadyToProcess(bool newReadyToProcess);
    bool m_readyToProcess{false};

    bool saveResult() const;
    void setSaveResult(bool newSaveResult);
    bool m_saveResult{false};

    const QVector<QString> &classNamesList() const;
    void setClassNamesList(const QVector<QString> &newClassNamesList);
    //    QVector<QString> m_classNamesList = {"Sib","Mis_shape","Naghes","Sar","Tah","Zakhme_khoshk","Khale_siyah","Laki","Negini"};
    //    QVector<QString> m_classNamesList = {"Apple","Mis_shape","Nubbin","Top","Bottom","Blotch","Scab","Bruising","Cut"}; //MUST
    QVector<QString> m_classNamesList = {"Apple","Mis_shape","Nubbin","Top","Bottom","Blotch","Scab","Bruising","Cut"};


    // Enabled Classes
    QVector<bool> classEnableList() const;
    void setClassEnableList(QVector<bool> newClassEnableList);
    QVector<bool> m_classEnableList;

    // Enabled Classes setter, getter
    const QVector<int> &classConfList() const;
    void setClassConfList(const QVector<int> &newClassConfList);
    QVector<int> m_classConfList;

    // Defineing if defect counting be enabled, at all
    bool quantityEnable() const;
    void setQuantityEnable(bool newquantityEnable);
    bool m_quantityEnable {false};

    // Defineing for which classes quantity based counting be considered
    const QVector<bool> &classQuantityEnabledList() const;
    void setclassQuantityEnabledList(const QVector<bool> &newclassQuantityEnabledList);
    QVector<bool> m_classQuantityEnabledList = {false,false,true,true,false,false,false,false,false}; //MUST


    QVector<int> m_quantityBasedClassCountList;
    const QVector<int> &quantityBasedClassCountList() const;
    void setQuantityBasedClassCountList(const QVector<int> &newQuantityBasedClassCountList);

    // this values will not be accessible from ui
    QVector<float> m_quantityBasedClassConfList;
    const QVector<float> &quantityBasedClassConfList() const;
    void setQuantityBasedClassConfList(const QVector<float> &newQuantityBasedClassConfList);

    const QVector<int> &classesCount() const;
    void setClassesCount(const QVector<int> &newClassesCount);
    QVector<int> m_classesCount = {0,0,0,0,0,0,0,0,0};

    int ejectorPosition() const;
    void setEjectorPosition(int newEjectorPosition);
    int m_ejectorPosition;

    int energize() const;
    void setEnergize(int newEnergize);
    int m_energize;

    int deEnergize() const;
    void setDeEnergize(int newDeEnergize);
    int m_deEnergize;

    int batchID() const;
    void setBatchID(int newBatchID);
    void stepBatchID();
    int m_batchID{-1};


    int total() const;
    void setTotal(int newtotal);
    void increaseTotalFruits();
    int m_total{0};


    bool ardStatus() const;
    void setArdStatus(bool newArdStatus);
    bool m_ardStatus{false};

    //E
    bool ethStatus() const;
    void setEthStatus(bool newEthStatus);
    bool m_ethStatus{false};

    bool cameraStatus(const QString Serial);
    QString calcEjectorVectorRowString(QVector<int> _ejectorQueueRow);

    GError* err = NULL;
    GstElement* pipeline;
    GstElement* source;
    GstElement* pipeline_swtrg;
    QVector<GstElement*> source_list;
    QVector<GstElement*> pipeline_swtrg_list;

    static GstPadProbeReturn handle_new_sample(GstPad *pad, GstPadProbeInfo *info, gpointer user_data);

    void captureImage(GstElement *source);

    void set_Camera_Ethernet_Setting();

    void modelWarmUp();

    QVector<cv::Mat> separateMiddle(cv::Mat &frame);
    QVector<cv::Mat> separateLeftAndRight(cv::Mat &frame);

    void clearArrays();
    QVector<cv::Mat> separateFrames(cv::Mat &frame);
    QVector<bool> showChecks() const;
    void setShowChecks(const QVector<bool> &newShowChecks);

    QVector<QString> &getCorrespondingVec(QString text);
    bool isValidScalar(const cv::Scalar &scalar);
    QVector<QString> cropHsv() const;
    void setCropHsv(const QVector<QString> &newCropHsv);

    QVector<QString> rdHsv() const;
    void setRdHsv(const QVector<QString> &newRdHsv);

    QVector<QString> orHsv() const;
    void setOrHsv(const QVector<QString> &newOrHsv);

    QVector<QString> ylHsv() const;
    void setYLHsv(const QVector<QString> &newYlHsv);

    QVector<QString> gnHsv() const;
    void setGnHsv(const QVector<QString> &newGnHsv);

    QVector<int> colorPercents() const;
    void setColorPercents(const QVector<int> &newColorPercents);

    int encoder() const;
    void setEncoder(int newEncoder);

    QVector<bool> cameraIsOpened() const;
    void setCameraIsOpened(const QVector<bool> &newCameraIsOpened);
    QVector<bool> cameraIsStarted() const;
    void setCameraIsStarted(const QVector<bool> &newCameraIsStarted);

    void saveImgs(std::vector<cv::Mat> &imgs, int cup);
    QVector<QString> frameIdx() const;
    void setFrameIdx(const QVector<QString> &newFrameIdx);

    bool startCameras() const;
    void setStartCameras(bool newStartCameras);

public Q_SLOTS:
    void getCameraSerials();

    void mainLoop(int _en, int _cup);
    void startAnalise(int &_cup);
    void stopAnalise();
    void setClassesConf(int _id, int _conf);
    void setClassesEnabled(int _id, bool _enb);
    void setclassQuantity(int _id, int _qnt);
    void setclassEnabledQuantity(int _id, bool _qntEnable);
    void timerFunction();

    void readDb(QString exf);
    void setNewSetting(QString serial, int idx1);
    void showFrames();
    void appendViewPort(CaptureViewPort *newCapViewPort);
    void readColors(QString colorGroup);
    void assignDb(DatabaseController *db);
    void start();
    void process();
    void setIdxFrameIdx(int idx, QString value);
    void setIdxShowChecks(int idx, bool value);
    void processFrames(int _cup);
    void processInputs(int _cup);
private:
    cv::Mat s1 = cv::imread("/home/perticon/sib-pics/r_10.jpg");
    cv::Mat s2 = cv::imread("/home/perticon/sib-pics/4.jpg");

    QTimer *_showFrameTimer = new QTimer(this);

    QVector<QString> ipList;
    QVector<QString> gwList;

    QVector<QString> camerasAddedToDb;

    GstElement* _source;
    QVector<QString> camSerials;
    bool grabFirst = false;
    bool grabSecond = false;
    bool retrieveFirst = false;
    bool retrieveSecond = false;
    bool restartFlag = false;

    std::vector<CaptureViewPort*> capViewPortList;

    QVector <gsttcam::TcamCamera> tcamList;
    Processor _processor;
    QTimer *timer = new QTimer(this);
    QTimer *frameTimer = new QTimer(this);

    QFuture<std::vector<std::vector<Detection>>> _inferenceResult;
    QString _rootPath;
    QString _settingPath;
    QList<camPack> _camPackList;
    QVector<QVector<int>> _prevbatchRes;
    QVector<QVector<int>> _ejectorQueue;

    QVector<QVector<int>> _NumberOfDefectsLine1;
    QVector<QVector<int>> _NumberOfDefectsLine2;
    QVector<QVector<int>> _NumberOfDefectsInEachImageOfBatchPrev;
    QVector<QVector<int>> lineClassValidityTotalPrev;

    QMutex ejectorLocker;
    std::vector<cv::Mat> _framesBatch;
    void preparingTheCameras();
    void releaseTheCameras();
    void increaseClassesCountByID(int _id);
    void postProcessing(std::vector<std::vector<Detection>> _detList, int _cup);

    QVector<QVector<int>> calcEjectorVectorRow(std::vector<std::vector<Detection>> _detList, int _cup);

    QVector<bool> m_showChecks = {false, false, false};
    bool checks[3] = {false, false, false};

    QVector<QString> m_cropHsv;

    QVector<QString> m_rdHsv;

    QVector<QString> m_orHsv;

    QVector<QString> m_ylHsv;

    QVector<QString> m_gnHsv;

    QVector<int> m_colorPercents;

    bool grabFrameForSetting = false;

    QVector<bool> m_cameraIsOpened;

    QVector<QString> m_frameIdx;

    std::vector<cv::Mat> _framesBatchReadyForInference;


Q_SIGNALS:
    void serialsChanged();
    void saveResultChanged();
    void classNamesListChanged();
    void classConfListChanged();
    void classEnableListChanged();
    void classNumberEnableListChanged();
    void ejectorPositionChanged();
    void shiftChanged();
    void energizeChanged();
    void deEnergizeChanged();
    void analyseStatueChanged();
    void classesCountChanged();
    void readyToProcessChanged();
    void batchIDChanged();
    void ardStatusChanged();
    void ethStatusChanged();
    void quantityEnableChanged(); //exist
    void classQuantityEnabledListChanged(); // which class enabled
    void quantityBasedClassCountListChanged(); // changing quantity of each class
    void quantityBasedClassConfListChanged();
    void totalChanged();
    void showChanged();
    void encoderChanged();
    void showChecksChanged();
    void cropHsvChanged();
    void orHsvChanged();
    void ylHsvChanged();
    void gnHsvChanged();
    void colorPercentsChanged();
    void cameraIsOpenedChanged();
    void cameraIsStartedChanged();
    void frameIdxChanged();
    void startCamerasChanged();
    void goToProcess(int _cup);
};

#endif // CAMERACONTROLLER_H
