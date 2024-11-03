#include "cameracontroller.h"

CameraController::CameraController(QObject *parent) : QObject{parent}
{
    QDir _homeFolder(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
    _rootPath = _homeFolder.absolutePath() + "/Perticon/";

    if(!QDir(_rootPath).exists())
        QDir().mkdir(_rootPath);

    for(int i=0;i<10;i++)
        m_colorPercents.push_back(0);

    m_showChecks = {false, false, false};
    m_cameraIsOpened = {false, false};
    m_cameraIsStarted = {false, false};
    m_frameIdx = {"1", "1", "1"};

    _processor.cameraOpened = m_cameraIsOpened;

    _processor.setterMySetup(mySetup.multiFrame, SharedData::_numberOfLines);

}

CameraController::~CameraController()
{
    //stopAnalise();
}

void CameraController::stepCup()
{

    if(Q_UNLIKELY(restartFlag && analyseStatue() && m_batchID > mySetup.batchIDIgnogance)){
        setReadyToProcess(true);
        restartFlag = false;
    }
}

void CameraController::assignDb(DatabaseController *db)
{
    _db = db;
}

void CameraController::getCameraSerials()
{
    m_serials.clear();
    //Find the available cameras and push the camera serial into camList vector
    std::vector<gsttcam::CameraInfo> camList = gsttcam::get_device_list();


    for(auto &cam:camList)
        m_serials.push_back(QString::fromStdString(cam.serial));

    std::sort(m_serials.begin(), m_serials.end());

    camerasAddedToDb = _db->getCameras();

    std::sort(camerasAddedToDb.begin(), camerasAddedToDb.end());

    if(camerasAddedToDb.empty())
    {
        qDebug() << "--- No Cameras Were Added";
        return;
    }

    for(int i=0;i<m_serials.size();i++)
    {
        if(camerasAddedToDb.indexOf(m_serials[i]) != -1)
        {
            m_cameraIsOpened[camerasAddedToDb.indexOf(m_serials[i])] = true;
            Q_EMIT cameraIsOpenedChanged();
        }
    }
}

void CameraController::setNewSetting(QString serial, int idx1)
{
    //Pass the new settings to CaptureThread and show the new settings on camera
    if(!_camPackList.isEmpty()) {
        int idx = m_serials.indexOf(serial);
        QVector<QString> setting = _db->camSetting();
        _camPackList[idx].capture->setNewSetting(_camPackList[idx].capture->_source, setting[idx1], idx1);
    }
}

void CameraController::clearArrays()
{
    m_classNamesList.clear();
    m_classConfList.clear();
    m_quantityBasedClassCountList.clear();
    m_quantityBasedClassConfList.clear();
}

void CameraController::readDb(QString exf)
{
    QVector<QVector<QString>> res = _db->readConfidences(exf);

    clearArrays();

    for(const auto &r:res[0])
        m_classNamesList.push_back(r);

    for(const auto &r:res[1])
        m_classConfList.push_back(r.toInt());

    for(const auto &r:res[2])
        m_quantityBasedClassCountList.push_back(r.toInt());

    for(const auto &r:res[3])
        m_quantityBasedClassConfList.push_back(r.toFloat());
}

QVector<QString> &CameraController::getCorrespondingVec(QString text)
{
    if(text == "Red") return m_rdHsv;
    else if(text == "Orange") return m_orHsv;
    else if(text == "Yellow") return m_ylHsv;
    else if(text == "Green") return m_gnHsv;
    else if(text == "Crop")
    {
        return m_cropHsv;
    }
}

void CameraController::readColors(QString colorGroup)
{
    m_cropHsv.clear();
    m_rdHsv.clear();
    m_orHsv.clear();
    m_ylHsv.clear();
    m_gnHsv.clear();

    QVector<QString> colors = {"Red", "Orange", "Yellow", "Green", "Crop"};

    for(const auto &color:colors)
    {
        _db->readColor(colorGroup, color);
        QVector<QString> &vec = getCorrespondingVec(color);
        vec.push_back(_db->color()[2]);
        vec.push_back(_db->color()[3]);
        vec.push_back(_db->color()[4]);
        vec.push_back(_db->color()[5]);
        vec.push_back(_db->color()[6]);
        vec.push_back(_db->color()[7]);
        vec.push_back(_db->color()[8]);
    }

    _processor._cropHsv = m_cropHsv;
    _processor._rdHsv = m_rdHsv;
    _processor._orHsv = m_orHsv;
    _processor._ylHsv = m_ylHsv;
    _processor._gnHsv = m_gnHsv;
}

bool CameraController::cameraStatus(const QString Serial)
{
    return gsttcam::TcamCamera::SerialExists(Serial.toStdString());
}


void CameraController::set_Camera_Ethernet_Setting()
{
    qDebug() << "---" << Q_FUNC_INFO;
    QProcess process;
    const QString NetMask = "255.255.255.0";

    QVector<QString> ipList;
    QVector<QString> gwList;

    for(const auto &serial:m_serials)
    {
        ipList.push_back(_db->getIp(serial));
        gwList.push_back(_db->getGw(serial));
    }

    for(int i=0; i<ipList.length(); i++)
    {
        process.start("tcam-gigetool", QStringList() << "rescue" << "--ip" << ipList[i] << "--netmask" << NetMask << "--gateway" << gwList[i] << m_serials[i]);
        process.waitForFinished();
    }
}

void CameraController::start()
{
    //connect(this,SIGNAL(encoderChanged()), this, SLOT(mainLoop()));
    //startAnalise();
}

void CameraController::process()
{
    if(m_graderStatus == Grading)
        _showFrameTimer->stop();
}

void CameraController::startAnalise(int &_cup)
{
    // connect(_showFrameTimer, &QTimer::timeout, this, &CameraController::process);
    // _showFrameTimer->start(50);

    _processor.setModelPath(mySetup.modelPath);
    _processor.setClassesList(m_classNamesList);
    _processor.setOutputSize(cv::Size(384, 384));

    // _camPackList.clear();
    // preparingTheCameras();
    modelWarmUp();

    setBatchID(0);
    m_total = 0;
    setClassesCount({0,0,0,0,0,0,0,0,0}); //TODO

    _cup = 0;

    _NumberOfDefectsInEachImageOfBatchPrev.clear(); // Dim: numOfLines x numOfClasses
    lineClassValidityTotalPrev.clear();
    QVector<int> zeroVector(m_classNamesList.size(),0);
    for (int j = 0; j < mySetup.batchSize; j++) {
        _NumberOfDefectsInEachImageOfBatchPrev.append(zeroVector);
        lineClassValidityTotalPrev.append(zeroVector);
    }

    for (int j = 0; j < 2; j++){ // Dim: 2(new and old) x numOfLines
        QVector<int> zeroVector(SharedData::_numberOfLines,0); /// TODO
        _prevbatchRes.append(zeroVector);
    }

    restartFlag = true;
    setAnalyseStatue(true);

    QString _folderPath = _rootPath + QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss");

    if(!QDir(_folderPath).exists())
        QDir().mkdir(_folderPath);

    QString _picPath = _folderPath + "/Images/";
    if(!QDir(_picPath).exists())
        QDir().mkdir(_picPath);

    QString _reportPath = _folderPath + "/Reports/";
    if(!QDir(_reportPath).exists())
        QDir().mkdir(_reportPath);

    _processor._folderPath = _picPath;

    connect(this,&CameraController::goToProcess, this, &CameraController::processInputs);

    showFlag = true;

    QtConcurrent::run(this,&CameraController::showFrames);

    //process();

}

void CameraController::processInputs(int _cup)
{
    processFrames(_cup);
}

void CameraController::showFrames()
{
    while (showFlag)
    {
        if(m_graderStatus == GraderStatus::Stopped || m_graderStatus == GraderStatus::Calibrating)
        {
            if(!m_showChecks.empty())
            {
                if(m_showChecks[0] || m_showChecks[1] || m_showChecks[2])
                {
                    try {
                        for(auto &_pac : _camPackList){
                            if(true)
                            {
                                _pac.capture->grabFrame();
                                _grabFlag = true;
                            }
                        }
                    } catch (...) {
                        qWarning("Failed to grab first image!");
                    }

                    QThread::msleep(30);

                    _framesBatch.clear();
                    for(auto &_pac : _camPackList){
                        if(true)
                        {
                            auto f = _pac.capture->retrieveFrame();
                            const auto &separatedFrames = separateFrames(f);

                            if(Q_LIKELY(!f.empty())){
                                for(const auto &separatedFrame:separatedFrames)
                                    _framesBatch.push_back(separatedFrame.clone());
                                _grabFlag = false;
                                //_retrieveFlag = true;
                            }

                        }
                    }
                }

                if(m_showChecks[0]) //show cam setting
                {
                    for(const auto &pack:_camPackList)
                    {
                        if(_framesBatch.size() == 8)
                        {
                            pack.capture->_orgFrameMat[0] = _framesBatch[m_frameIdx[0].toInt() - 1];
                            pack.capture->_orgFrameMat[1] = _framesBatch[m_frameIdx[0].toInt() - 1 + 4];
                        }
                        // else
                        // {
                        //     if(m_cameraIsOpened[0])
                        //     {
                        //         pack.capture->_orgFrameMat[0] = _framesBatch[m_frameIdx[0].toInt() - 1];
                        //     }
                        //     else if(m_cameraIsOpened[1])
                        //     {
                        //         pack.capture->_orgFrameMat[1] = _framesBatch[m_frameIdx[0].toInt() - 1 + 4];
                        //     }
                        // }

                        Q_EMIT pack.capture->updateRequired();
                    }
                }

                if(m_showChecks[1])
                {
                    for(const auto &pack:_camPackList) //show colored frame
                    {
                        //TODO
                        // cv::Mat f1 = cv::imread("/home/perticon/sib-pics/3.jpg");
                        // cv::Mat f2 = cv::imread("/home/perticon/sib-pics/4.jpg");

                        if(_framesBatch.size() == 8)
                        {
                            std::vector<cv::Mat> cl1 = {_framesBatch[0], _framesBatch[1], _framesBatch[2], _framesBatch[3], _framesBatch[0], _framesBatch[1], _framesBatch[2]};
                            std::vector<cv::Mat> cl2 = {_framesBatch[4], _framesBatch[5], _framesBatch[6], _framesBatch[7], _framesBatch[4], _framesBatch[5], _framesBatch[6]};

                            _processor._frameIdx = m_frameIdx[0].toInt();

                            _processor.getColorPercents(cl1, 0 , 0);
                            _processor.getColorPercents(cl2, 0 , 1);

                            pack.capture->_orgFrameMat[0] = _framesBatch[m_frameIdx[0].toInt() - 1];
                            pack.capture->_orgFrameMat[1] = _framesBatch[m_frameIdx[0].toInt() - 1 + 4];

                            pack.capture->_colorFrameMat[0] = _processor._coloredFrames[m_frameIdx[0].toInt() - 1];
                            pack.capture->_colorFrameMat[1] = _processor._coloredFrames2[m_frameIdx[0].toInt() - 1];
                        }

                        m_colorPercents = _processor._colorsPer;
                        _processor._colorsPer.clear();
                        _processor._colorDetector->_colorsPer.clear();

                        Q_EMIT colorPercentsChanged();
                        Q_EMIT pack.capture->updateRequired();
                    }
                }

                std::vector<cv::Mat> _processBatch;

                if(m_showChecks[2])
                {
                    for(const auto &pack:_camPackList)//show defected frame
                    {
                        if(_framesBatch.size() == 8)
                        {
                            pack.capture->_orgFrameMat[0] = _framesBatch[m_frameIdx[0].toInt() - 1];
                            pack.capture->_orgFrameMat[1] = _framesBatch[m_frameIdx[0].toInt() - 1 + 4];
                        }
                        // else
                        // {
                        //     if(m_cameraIsOpened[0])
                        //     {
                        //         pack.capture->_orgFrameMat[0] = _framesBatch[0];
                        //     }
                        //     else if(m_cameraIsOpened[1])
                        //     {
                        //         pack.capture->_orgFrameMat[1] = _framesBatch[0];
                        //     }
                        // }

                        if(_framesBatch.size() == 8)
                        {
                            _processBatch = {
                                _framesBatch[0], _framesBatch[1], _framesBatch[2], _framesBatch[3], _framesBatch[4], _framesBatch[5], _framesBatch[6],
                                _framesBatch[4], _framesBatch[5], _framesBatch[6], _framesBatch[7], _framesBatch[4], _framesBatch[5], _framesBatch[6]
                            };

                            // for(int j=0;j<7;j++)
                            // {
                            //     _processBatch.push_back(_framesBatch[0]);
                            // }
                            // for(int j=0;j<7;j++)
                            // {
                            //     _processBatch.push_back(_framesBatch[4]);
                            // }

                            //qDebug() << _processor._labeledFrame.size();


                            std::vector<cv::Mat> _outputFrames(_processBatch.size());
                            _processor._saveResult = true;
                            _processor._saveImg = false;
                            auto res = _processor.process(_processBatch, _outputFrames, 0, _numberOfLines);

                            //qDebug() << _processor._labeledFrame.size();
                            if(!_processor._labeledFrame.empty())
                            {
                                pack.capture->_defectFrameMat[0] = _processor._labeledFrame[m_frameIdx[0].toInt() - 1];
                                pack.capture->_defectFrameMat[1] = _processor._labeledFrame[m_frameIdx[0].toInt() - 1 + 7];
                            }
                        }
                        // else
                        // {
                        //     if(m_cameraIsOpened[0])
                        //     {
                        //         for(int j=0;j<7;j++)
                        //         {
                        //             _processBatch.push_back(_framesBatch[0]);
                        //         }

                        //         std::vector<cv::Mat> _outputFrames(_processBatch.size());
                        //         _processor._saveResult = true;
                        //         _processor._saveImg = false;
                        //         auto res = _processor.process(_processBatch, _outputFrames, _cup, _numberOfLines);

                        //         pack.capture->_defectFrameMat[0] = _processor._labeledFrame[0];
                        //     }
                        //     else if(m_cameraIsOpened[1])
                        //     {
                        //         for(int j=0;j<7;j++)
                        //         {
                        //             _processBatch.push_back(_framesBatch[0]);
                        //         }

                        //         std::vector<cv::Mat> _outputFrames(_processBatch.size());
                        //         _processor._saveResult = true;
                        //         _processor._saveImg = false;
                        //         auto res = _processor.process(_processBatch, _outputFrames, _cup, _numberOfLines);

                        //         pack.capture->_defectFrameMat[1] = _processor._labeledFrame[0];
                        //     }
                        // }

                        _processor._labeledFrame.clear();

                        Q_EMIT pack.capture->updateRequired();
                        _processBatch.clear();
                    }
                }

            }
        }
    }
}

void CameraController::timerFunction()
{


}

void CameraController::stopAnalise()
{
    qDebug() << "---" << Q_FUNC_INFO;

    showFlag = false;

    camerasStarted = 0;

    capViewPortList.clear();

    _processor._coloredFrames.clear();
    _processor._coloredFrames2.clear();

    _processor._labeledFrame.clear();

    disconnect(_showFrameTimer, &QTimer::timeout, this, &CameraController::process);
    _showFrameTimer->stop();

    m_cameraIsStarted.fill(false);

    setAnalyseStatue(false);
    setReadyToProcess(false);
    releaseTheCameras();
}

void CameraController::modelWarmUp()
{
    qInfo() << "------ " << Q_FUNC_INFO;
    int warmup_epochs=10;
    for(int i=0; i<warmup_epochs; i++){

        cv::Mat blackFrame = cv::Mat::zeros(mySetup.height, mySetup.height, CV_8UC3);
        std::vector<cv::Mat> blackFrameBatch;

        // if(m_cameraIsOpened[0] && m_cameraIsOpened[1])
        //     for(int j=0; j< SharedData::_batchSize; j++){
        //         blackFrameBatch.push_back(blackFrame);
        //     }
        // else if(m_cameraIsOpened[0])
        //     for(int j=0; j< SharedData::_batchSize / 2; j++){
        //         blackFrameBatch.push_back(blackFrame);
        //     }
        // else if(m_cameraIsOpened[1])
        //     for(int j=0; j< SharedData::_batchSize / 2; j++){
        //         blackFrameBatch.push_back(blackFrame);
        //     }

        for(int j=0; j< SharedData::_batchSize; j++){
            blackFrameBatch.push_back(blackFrame);
        }

        std::vector<cv::Mat> _outputFrames(blackFrameBatch.size());
        _processor.process(blackFrameBatch, _outputFrames, 0, SharedData::_numberOfLines);
    }
    qInfo() << "------ ✓ model warmup done.";
}

void CameraController:: preparingTheCameras()
{
    qDebug() << "---" << Q_FUNC_INFO;

    set_Camera_Ethernet_Setting();

    //m_serials = {"02324177", "02324166"};

    for (int i = 0; i < m_serials.size(); i++) {
        CaptureThread *_capture = new CaptureThread;
        camPack _pack;
        _pack.status = cameraStatus(m_serials[i]);

        _capture->width = mySetup.width;
        _capture->height = mySetup.height;

        if (_pack.status){
            _db->readCams(m_serials[i]);
            QVector<QString> camSetting = _db->camSetting();
            QString pipeline_description_str = "tcambin name=source serial=" + m_serials[i] + " TriggerMode=on ! video/x-raw, format=BGRx, width=" + QString::number(mySetup.width) + ", height=" + QString::number(mySetup.height) + ", framerate=100/1 ! videoconvert ! appsink name=sink";

            _capture->setCapture(pipeline_description_str, camSetting); // This line gives error if serial in pipeline be invalid
        }

        _capture->setCaptureID(i);
        _pack.capture = _capture;
        _camPackList.push_back(_pack);

        if(camerasAddedToDb.indexOf(m_serials[i]) != -1)
            m_cameraIsStarted[camerasAddedToDb.indexOf(m_serials[i])] = true;

        camerasStarted++;
    }

}

void CameraController::releaseTheCameras() {
    qDebug() << "---" << Q_FUNC_INFO;
    for(auto &_pack : _camPackList) {
        _pack.capture->releaseCapture();
        _pack.capture->quit();
    }
}

QVector<cv::Mat> CameraController::separateFrames(cv::Mat& frame)
{
    QVector<cv::Mat> output;

    if(!frame.empty())
        for(int i=0;i<4;i++)
        {
            output.append(frame(cv::Range(0, mySetup.height), cv::Range(i * mySetup.height,i * mySetup.height + mySetup.height)).clone());
        }

    return output;
}

QVector<cv::Mat> CameraController::separateLeftAndRight(cv::Mat& frame)
{
    QVector<cv::Mat> output;

    int leftCut = 180;
    int rightCut = 180;
    int middleWidth = (mySetup.width - leftCut - rightCut) / 3;

    for (int i = 0; i < 3; ++i) {
        int startX = leftCut + i * middleWidth;

        // Extract the region
        output.append(frame(cv::Range(0, mySetup.height), cv::Range(startX, startX + middleWidth)).clone());
    }

    return output;
}

QVector<cv::Mat> CameraController::separateMiddle(cv::Mat& frame){

    QVector<cv::Mat> output;
    cv::Mat middle = frame(cv::Range(0,768), cv::Range(336,1104)).clone();
    output.append(middle);
    return output;
}

void CameraController::mainLoop(int _en, int _cup){
    if(_cup == 282)
    {
        _processor._saveResult = false;
    }

    if(m_graderStatus == Grading)
    {
        //qDebug() << _en << "\t" << _cup % 283;

        //grab first multi frame

        if (_en > mySetup.grabTimes[0][0] && _en < mySetup.grabTimes[0][1] && !grabFirst  && mySetup.multiFrame){
            //qDebug() << " ------- grab1" << _cup % 283;
            try {
                for(auto &_pac : _camPackList){
                    _pac.capture->grabFrame();
                }
            } catch (...) {
                qWarning("Failed to grab first image!");
            }

            grabFirst = true;
        }

        //retrieve first multi frame
        if ((_en > mySetup.retrieveTimes[0][0]) && (_en < mySetup.retrieveTimes[0][1]) && grabFirst && !retrieveFirst && mySetup.multiFrame){
            //qDebug() << " ------- retrieve1" << _cup % 283;
            _framesBatch.clear();
            for(auto &_pac : _camPackList){
                auto f = _pac.capture->retrieveFrame(_cup % 283);
                const auto &separatedFrames = separateFrames(f);

                if(Q_LIKELY(!f.empty())){
                    for(const auto &separatedFrame:separatedFrames)
                        _framesBatch.push_back(separatedFrame.clone());
                }
            }
            //if((int)_framesBatch.size() != 4) // what if not be true? were it will be trapped? output table will be lagged?
            //    qDebug() << "retrieve 1, _framesBatch.size() = " << _framesBatch.size()  << "EEEEEEEEEEEEERRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRROOOOOOOOOOOOOOOOOOOOOOOOOORRRRRRRRRRRRRRRRRRRRRR" << _framesBatch.size();
            retrieveFirst = true;
        }

        //grab second multi frame
        if (_en > mySetup.grabTimes[1][0] && _en < mySetup.grabTimes[1][1] && grabFirst && retrieveFirst && !grabSecond  && mySetup.multiFrame){
            //qDebug() << " ------- grab2" << _cup % 283;
            try {
                for(auto &_pac : _camPackList){
                    _pac.capture->grabFrame();
                }
            } catch (...) {
                qWarning("Failed to grab first image!");
            }

            grabSecond = true;
        }


        //retrieve second multi frame
        if ((_en > mySetup.retrieveTimes[1][0]) && (_en < mySetup.retrieveTimes[1][1]) && grabSecond && !retrieveSecond && mySetup.multiFrame){
            //qDebug() << " ------- retrieve2 " << _cup % 283 ;
            for(auto &_pac : _camPackList){
                auto f = _pac.capture->retrieveFrame(_cup % 283);

                const auto &separatedFrames = separateLeftAndRight(f);
                if(Q_LIKELY(!f.empty())){
                    for(const auto &separatedFrame:separatedFrames)
                        _framesBatch.push_back(separatedFrame.clone());
                }
            }
            //if((int)_framesBatch.size() != mySetup.batchSize) // what if not be true? were it will be trapped? output table will be lagged?
            //    qDebug() << "retrieve 2, _framesBatch.size() = " << _framesBatch.size()  << "EEEEEEEEEEEEERRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRROOOOOOOOOOOOOOOOOOOOOOOOOORRRRRRRRRRRRRRRRRRRRRR" << _framesBatch.size();
            retrieveSecond = true;
        }

        if ((_en > mySetup.processTime[0][0]) && (_en < mySetup.processTime[0][1]) && ((mySetup.multiFrame && retrieveFirst && retrieveSecond))){
            //qDebug() << " ------- Inference_multi" << _cup % 283;

            grabFirst = false;
            retrieveFirst = false;
            grabSecond = false;
            retrieveSecond = false;

            //_framesBatch = {f1, f2, f3, f4, f5, f6, f7, f1, f2, f3, f4, f5, f6, f7};
            _framesBatchReadyForInference.clear();
            _framesBatchReadyForInference.push_back(_framesBatch[0].clone());
            _framesBatchReadyForInference.push_back(_framesBatch[1].clone());
            _framesBatchReadyForInference.push_back(_framesBatch[2].clone());
            _framesBatchReadyForInference.push_back(_framesBatch[3].clone());
            _framesBatchReadyForInference.push_back(_framesBatch[8].clone());
            _framesBatchReadyForInference.push_back(_framesBatch[9].clone());
            _framesBatchReadyForInference.push_back(_framesBatch[10].clone());
            _framesBatchReadyForInference.push_back(_framesBatch[4].clone());
            _framesBatchReadyForInference.push_back(_framesBatch[5].clone());
            _framesBatchReadyForInference.push_back(_framesBatch[6].clone());
            _framesBatchReadyForInference.push_back(_framesBatch[7].clone());
            _framesBatchReadyForInference.push_back(_framesBatch[11].clone());
            _framesBatchReadyForInference.push_back(_framesBatch[12].clone());
            _framesBatchReadyForInference.push_back(_framesBatch[13].clone());

            if(m_showChecks[0]) //show cam setting
            {
                for(const auto &pack:_camPackList)
                {

                    pack.capture->_orgFrameMat[0] = _framesBatch[0];
                    pack.capture->_orgFrameMat[1] = _framesBatch[4];

                    Q_EMIT pack.capture->updateRequired();

                }
            }

            std::vector<cv::Mat> _colorBatch = {_framesBatchReadyForInference[0], _framesBatchReadyForInference[1], _framesBatchReadyForInference[2],
                                                _framesBatchReadyForInference[3], _framesBatchReadyForInference[4], _framesBatchReadyForInference[5], _framesBatchReadyForInference[6]};
            std::vector<cv::Mat> _colorBatch2 = {_framesBatchReadyForInference[7], _framesBatchReadyForInference[8], _framesBatchReadyForInference[9],
                                                 _framesBatchReadyForInference[10], _framesBatchReadyForInference[11], _framesBatchReadyForInference[12], _framesBatchReadyForInference[13]};

            if(_framesBatch.size() == 14)
            {
                QtConcurrent::run(&_processor, &Processor::getColorPercents, _colorBatch, _cup, 0);
                QtConcurrent::run(&_processor, &Processor::getColorPercents, _colorBatch2, _cup, 1);
            }

            _framesBatch.clear();

            Q_EMIT goToProcess(_cup);
        }
    }
}


void CameraController::processFrames(int _cup)
{
    std::vector<std::vector<Detection>> _res;
    QFutureWatcher<std::vector<std::vector<Detection>>> *watcher = new QFutureWatcher<std::vector<std::vector<Detection>>>(this);
    QObject::connect(watcher, &QFutureWatcher<std::vector<std::vector<Detection>>>::finished, [=]() {
        try {
            postProcessing(watcher->result(), _cup);
        } catch (...) {
            qDebug() << "*//////////////////////////////////////////////////////////////////////////////////////////// : ";
        }
    });

    int _num = _numberOfLines;
    _inferenceResult = QtConcurrent::run(&_processor, &Processor::process, _framesBatchReadyForInference, _framesBatchReadyForInference, _cup, _num);
    watcher->setFuture(_inferenceResult);
}

void CameraController::saveImgs(std::vector<cv::Mat> &imgs, int cup)
{
    int i = 0;

    cv::imwrite("f" + to_string(cup) + "_" + to_string(1) + ".jpg", imgs[4]);
    cv::imwrite("f" + to_string(cup-1) + "_" + to_string(3) + ".jpg", imgs[5]);
    cv::imwrite("f" + to_string(cup-2) + "_" + to_string(5) + ".jpg", imgs[6]);
    cv::imwrite("f" + to_string(cup-3) + "_" + to_string(7) + ".jpg", imgs[7]);
    cv::imwrite("f" + to_string(cup) + "_" + to_string(2) + ".jpg", imgs[11]);
    cv::imwrite("f" + to_string(cup-1) + "_" + to_string(4) + ".jpg", imgs[12]);
    cv::imwrite("f" + to_string(cup-2) + "_" + to_string(6) + ".jpg", imgs[13]);
}

void CameraController::postProcessing(std::vector<std::vector<Detection>> _res, int _cup)
{
    if(_cup > mySetup.batchIDIgnogance){
        QVector<QVector<int>> batchEjection = calcEjectorVectorRow(_res, _cup);
        if(2 == batchEjection.size())
            _prevbatchRes = batchEjection;
    }
}

QVector<QVector<int>> CameraController::calcEjectorVectorRow(std::vector<std::vector<Detection>> _detList, int _cup)
{
    // Creating empty matrix of batchEjection with size of: 2(new and old) x numOfLines
    QVector<QVector<int>> batchEjection;
    QVector<int> zerosVector(SharedData::_numberOfLines,0);
    for (int j = 0; j < 2; j++){
        batchEjection.append(zerosVector);
    }
    assert(batchEjection.size() == 2);


    // Creating two matrix with size of: batchSize x numOfClasses. lineClassValidityTotal(filled 0,1), _NumberOfDefectsInEachImageOfBatch(number of each defects in images)
    QVector<QVector<int>> lineClassValidityTotal;
    QVector<QVector<int>> _NumberOfDefectsInEachImageOfBatch;
    int idx=0;

    int _lineID = 0;
    int _cupID;

    //qDebug() << "_det Size________________" << _detList.size();

    for (const auto &batch : _detList) {

        QVector<int> lineClassValidity(m_classNamesList.size());
        QVector<int> defectCount(m_classNamesList.size());

        _cupID = (_cup % SharedData::_totalCupNumber + SharedData::_totalCupNumber - (idx % 7) % 4) % (SharedData::_totalCupNumber);

        for (const auto &_detObj : batch) {
            if(_detObj.confidence * 100 >= m_classConfList[_detObj.classID])
            {

                //qDebug() << "_______" << m_classNamesList[_detObj.classID] << _detObj.confidence * 100;
                //2 Lines
                //234 cups per line
                //7 images per cup
                qDebug() << "Defect___________________" << _cupID << m_classNamesList[_detObj.classID] << _detObj.classID << idx % 7 << _cupID << _detObj.confidence * 100 << _lineID;
                _output->lines[_lineID].cups[_cupID].Images[idx % 7].detectedObjects[_detObj.classID] = true;
            }
            // else
            // {
            //     //qDebug() << "No Defect___________________" << _cupID << m_classNamesList[_detObj.classID] << _detObj.classID << idx % 7 << _cupID << _detObj.confidence * 100 << _lineID;
            //     _output->lines[_lineID].cups[_cupID].Images[idx % 7].detectedObjects[_detObj.classID] = false;
            // }

            // Defect counting
            if(m_quantityEnable && _detObj.confidence * 100 >= m_quantityBasedClassConfList[_detObj.classID])
            {
                qDebug() << "Quantity___________________" << _cupID << m_classNamesList[_detObj.classID] << _detObj.classID << idx % 7 << _cupID << _detObj.confidence * 100 << _lineID;
                _output->lines[_lineID].cups[_cupID].Images[idx % 7].numberOfDefects[_detObj.classID] ++;
            }
            else
            {
                _output->lines[_lineID].cups[_cupID].Images[idx % 7].numberOfDefects[_detObj.classID] = 0;
            }
        }


        if(idx % 7 == 3)
        {
            if(_output->lines[_lineID].cups[_cupID].Images[idx % 7].detectedObjects[0])
            {
                qDebug() << "ccc______" << _cupID;
            }
        }
        idx++;

        if(idx == SharedData::_batchSize / 2)
        {
            _lineID++;

        }

        lineClassValidityTotal.push_back(lineClassValidity);
        _NumberOfDefectsInEachImageOfBatch.push_back(defectCount);
    }

    return batchEjection;
}


void CameraController::appendViewPort(CaptureViewPort *newCapViewPort)
{
    qDebug() << "---" << Q_FUNC_INFO << "ViewPort ID:" << newCapViewPort->id_viewPort();

    for (auto &_cap : capViewPortList) {
        if(_cap == newCapViewPort)
            return;
    }

    for (auto &_pack : _camPackList) {
        newCapViewPort->setCaptureThread(_pack.capture);
        capViewPortList.push_back(newCapViewPort);
    }
}


int CameraController::encoder() const
{
    return _encoder;
}

void CameraController::setEncoder(int newEncoder)
{
}


bool CameraController::analyseStatue() const
{
    return m_analyseStatue;
}
void CameraController::setAnalyseStatue(bool newAnalyseStatue)
{
    qDebug() << "---" << Q_FUNC_INFO << newAnalyseStatue;

    if (m_analyseStatue == newAnalyseStatue)
        return;
    m_analyseStatue = newAnalyseStatue;
    Q_EMIT analyseStatueChanged();
}

bool CameraController::readyToProcess() const
{
    return m_readyToProcess;
}
void CameraController::setReadyToProcess(bool newReadyToProcess)
{
    m_readyToProcess = newReadyToProcess;
    Q_EMIT readyToProcessChanged();
}

bool CameraController::saveResult() const
{
    return m_saveResult;
}
void CameraController::setSaveResult(bool newSaveResult)
{
    qDebug() << "---" << Q_FUNC_INFO << newSaveResult;

    if (m_saveResult == newSaveResult)
        return;

    m_saveResult = newSaveResult;
    _processor._saveResult = m_saveResult;
    Q_EMIT saveResultChanged();
}

const QVector<QString> &CameraController::classNamesList() const
{
    return m_classNamesList;
}

void CameraController::setClassNamesList(const QVector<QString> &newClassNamesList)
{
    m_classNamesList = newClassNamesList;
    Q_EMIT classNamesListChanged();
}

QVector<bool> CameraController::classEnableList() const
{
    return m_classEnableList;
}

void CameraController::setClassEnableList(QVector<bool> newClassEnableList)
{
    if (m_classEnableList == newClassEnableList)
        return;
    m_classEnableList = newClassEnableList;
    Q_EMIT classEnableListChanged();
}

void CameraController::setClassesEnabled(int _id, bool _enb)
{
    qDebug() << "---" << Q_FUNC_INFO << m_classNamesList[_id] << " ->" << _enb;

    try {
        m_classEnableList[_id] = _enb;
    } catch (...) {
        qDebug()<< "Can not set enable/disable this class:";
    }
}

const QVector<int> &CameraController::classConfList() const
{
    return m_classConfList;
}

void CameraController::setClassConfList(const QVector<int> &newClassConfList)
{
    m_classConfList = newClassConfList;
    Q_EMIT classConfListChanged();
}

void CameraController::setClassesConf(int _id, int _conf)
{
    qDebug() << "---" << Q_FUNC_INFO << m_classNamesList[_id] << " ->" << _conf;

    try {
        m_classConfList[_id] = _conf;
    } catch (...) {
        qDebug()<< "Can not save this Confidence:" << _conf;
    }
}


bool CameraController::quantityEnable() const
{
    return m_quantityEnable;
}

void CameraController::setQuantityEnable(bool newQuantityEnable)
{
    qDebug() << "---" << Q_FUNC_INFO << newQuantityEnable;

    if (m_quantityEnable == newQuantityEnable)
        return;

    m_quantityEnable = newQuantityEnable;
    //    _processor._saveResult = m_quantityEnable; //ToDo
    Q_EMIT quantityEnableChanged();
}

const QVector<int> &CameraController::quantityBasedClassCountList() const
{
    return m_quantityBasedClassCountList;
}

void CameraController::setQuantityBasedClassCountList(const QVector<int> &newQuantityBasedClassCountList)
{
    m_quantityBasedClassCountList = newQuantityBasedClassCountList;
    Q_EMIT quantityBasedClassCountListChanged();
}

const QVector<float> &CameraController::quantityBasedClassConfList() const
{
    return m_quantityBasedClassConfList;
}
void CameraController::setQuantityBasedClassConfList(const QVector<float> &newQuantityBasedClassConfList)
{
    m_quantityBasedClassConfList = newQuantityBasedClassConfList;
    Q_EMIT quantityBasedClassConfListChanged();
}

void CameraController::setclassQuantity(int _id, int _qnt)
{
    qDebug() << "---" << Q_FUNC_INFO << m_classNamesList[_id] << " ->" << _qnt;

    try {
        m_quantityBasedClassCountList[_id] = _qnt;
    } catch (...) {
        qDebug()<< "Can not save this Confidence:" << _qnt;
    }
}

const QVector<bool> &CameraController::classQuantityEnabledList() const
{
    return m_classQuantityEnabledList;
}
void CameraController::setclassQuantityEnabledList(const QVector<bool> &newclassQuantityEnabledList)
{
    m_classQuantityEnabledList = newclassQuantityEnabledList;
    Q_EMIT classQuantityEnabledListChanged();
}
void CameraController::setclassEnabledQuantity(int _id, bool _qntEnable)
{
    qDebug() << "---" << Q_FUNC_INFO << m_classNamesList[_id] << " ->" << _qntEnable;

    try {
        m_classQuantityEnabledList[_id] = _qntEnable;
    } catch (...) {
        qDebug()<< "Can not save this Confidence:" << _qntEnable;
    }
}

////
const QVector<int> &CameraController::classesCount() const
{
    return m_classesCount;
}

void CameraController::setClassesCount(const QVector<int> &newClassesCount)
{
    if (m_classesCount == newClassesCount)
        return;

    m_classesCount = newClassesCount;
    Q_EMIT classesCountChanged();
}

void CameraController::increaseClassesCountByID(int _id)
{
    try {
        m_classesCount[_id] += 1;
        Q_EMIT classesCountChanged();
    } catch (...) {
        qDebug()<< "Can not increase this classes counter:" << m_classNamesList[_id];
    }
}

int CameraController::ejectorPosition() const
{
    return m_ejectorPosition;
}
void CameraController::setEjectorPosition(int newEjectorPosition)
{
    qDebug() << "---" << Q_FUNC_INFO << newEjectorPosition;

    if (m_ejectorPosition == newEjectorPosition)
        return;
    m_ejectorPosition = newEjectorPosition;
    Q_EMIT ejectorPositionChanged();
}

int CameraController::energize() const
{
    return m_energize;
}
void CameraController::setEnergize(int newEnergize)
{
    qDebug() << "---" << Q_FUNC_INFO << newEnergize;

    if (m_energize == newEnergize)
        return;
    m_energize = newEnergize;
    Q_EMIT energizeChanged();
}

int CameraController::deEnergize() const
{
    return m_deEnergize;
}
void CameraController::setDeEnergize(int newDeEnergize)
{
    qDebug() << "---" << Q_FUNC_INFO << newDeEnergize;

    if (m_deEnergize == newDeEnergize)
        return;
    m_deEnergize = newDeEnergize;
    Q_EMIT deEnergizeChanged();
}

int CameraController::batchID() const
{
    return m_batchID;
}
void CameraController::stepBatchID()
{
    m_batchID++;


    if(Q_UNLIKELY(restartFlag && analyseStatue() && m_batchID > mySetup.batchIDIgnogance)){
        setReadyToProcess(true);
        restartFlag = false;
    }

    Q_EMIT batchIDChanged();
}
void CameraController::setBatchID(int newBatchID)
{
    m_batchID = newBatchID;
    Q_EMIT batchIDChanged();
}

int CameraController::total() const
{
    return m_total;
}
void CameraController::increaseTotalFruits(){
    //    if(Q_UNLIKELY(restartFlag && analyseStatue() && m_batchID > mySetup.batchIDIgnogance))
    // if(Q_UNLIKELY(_cup > mySetup.batchIDIgnogance))
    //     m_total += 1;
    Q_EMIT totalChanged();
}
void CameraController::setTotal(int newtotal)
{
    //    qDebug() << "m total : " << newtotal;
    m_total = newtotal;
    Q_EMIT totalChanged();
}

const QVector<QString> &CameraController::serials() const
{
    return m_serials;
}

void CameraController::setSerials(const QVector<QString> &newSerials)
{
    //    if (m_serials == newSerials)
    //        return;
    //    m_serials = newSerials;
    Q_UNUSED(newSerials);

    //    Q_EMIT serialsChanged();
}

bool CameraController::ethStatus() const
{
    return m_ethStatus;
}
void CameraController::setEthStatus(bool newEthStatus)
{
    m_ethStatus = newEthStatus;
    Q_EMIT ethStatusChanged();
}

bool CameraController::ardStatus() const
{
    return m_ardStatus;
}
void CameraController::setArdStatus(bool newArdStatus)
{
    m_ardStatus = newArdStatus;
    Q_EMIT ardStatusChanged();
}

QVector<bool> CameraController::showChecks() const
{
    return m_showChecks;
}

void CameraController::setShowChecks(const QVector<bool> &newShowChecks)
{
    if (m_showChecks == newShowChecks)
        return;
    m_showChecks = newShowChecks;
    Q_EMIT showChecksChanged();
}

void CameraController::setIdxShowChecks(int idx, bool value)
{
    m_showChecks[idx] = value;
}

QVector<QString> CameraController::cropHsv() const
{
    return m_cropHsv;
}

void CameraController::setCropHsv(const QVector<QString> &newCropHsv)
{
    if (m_cropHsv == newCropHsv)
        return;
    m_cropHsv = newCropHsv;
    _processor._cropHsv = m_cropHsv;
    _processor._colorDetector->_cropHsv = m_cropHsv;
    Q_EMIT cropHsvChanged();
}

QVector<QString> CameraController::rdHsv() const
{
    return m_rdHsv;
}

void CameraController::setRdHsv(const QVector<QString> &newRdHsv)
{
    if (m_rdHsv == newRdHsv)
        return;
    m_rdHsv = newRdHsv;
    _processor._rdHsv = m_rdHsv;
    _processor._colorDetector->_rdHsv = m_rdHsv;
    Q_EMIT cropHsvChanged();
}

QVector<QString> CameraController::orHsv() const
{
    return m_orHsv;
}

void CameraController::setOrHsv(const QVector<QString> &newOrHsv)
{
    if (m_orHsv == newOrHsv)
        return;
    m_orHsv = newOrHsv;
    _processor._orHsv = m_orHsv;
    _processor._colorDetector->_orHsv = m_orHsv;
    Q_EMIT orHsvChanged();
}

QVector<QString> CameraController::ylHsv() const
{
    return m_ylHsv;
}

void CameraController::setYLHsv(const QVector<QString> &newYlHsv)
{
    if (m_ylHsv == newYlHsv)
        return;
    m_ylHsv = newYlHsv;
    _processor._ylHsv = m_ylHsv;
    _processor._colorDetector->_ylHsv = m_ylHsv;
    Q_EMIT ylHsvChanged();
}

QVector<QString> CameraController::gnHsv() const
{
    return m_gnHsv;
}

void CameraController::setGnHsv(const QVector<QString> &newGnHsv)
{
    if (m_gnHsv == newGnHsv)
        return;
    m_gnHsv = newGnHsv;
    _processor._gnHsv = m_gnHsv;
    _processor._colorDetector->_gnHsv = m_gnHsv;
    Q_EMIT gnHsvChanged();
}

QVector<int> CameraController::colorPercents() const
{
    return m_colorPercents;
}

void CameraController::setColorPercents(const QVector<int> &newColorPercents)
{
    if (m_colorPercents == newColorPercents)
        return;
    m_colorPercents = newColorPercents;
    Q_EMIT colorPercentsChanged();
}

QVector<bool> CameraController::cameraIsOpened() const
{
    return m_cameraIsOpened;
}

void CameraController::setCameraIsOpened(const QVector<bool> &newCameraIsOpened)
{
    if (m_cameraIsOpened == newCameraIsOpened)
        return;
    m_cameraIsOpened = newCameraIsOpened;
    _processor.cameraOpened = m_cameraIsOpened;

    Q_EMIT cameraIsOpenedChanged();
}

QVector<bool> CameraController::cameraIsStarted() const
{
    return m_cameraIsStarted;
}

void CameraController::setCameraIsStarted(const QVector<bool> &newCameraIsStarted)
{
    if (m_cameraIsStarted == newCameraIsStarted)
        return;
    m_cameraIsStarted = newCameraIsStarted;
    Q_EMIT cameraIsStartedChanged();
}

QVector<QString> CameraController::frameIdx() const
{
    return m_frameIdx;
}

void CameraController::setFrameIdx(const QVector<QString> &newFrameIdx)
{
    if (m_frameIdx == newFrameIdx)
        return;
    m_frameIdx = newFrameIdx;
    Q_EMIT frameIdxChanged();
}

void CameraController::setIdxFrameIdx(int idx, QString value)
{
    m_frameIdx[idx] = value;
    Q_EMIT frameIdxChanged();
}

bool CameraController::startCameras() const
{
    return m_startCameras;
}

void CameraController::setStartCameras(bool newStartCameras)
{
    if (m_startCameras == newStartCameras)
        return;
    m_startCameras = newStartCameras;
    Q_EMIT startCamerasChanged();
}
