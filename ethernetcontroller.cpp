#include "ethernetcontroller.h"

EthernetController::EthernetController(QObject *parent) : QObject{parent}
{
    _pulseReader = new PulseReader();
    _pulseReader->start();

    tcpServer = new Tcp_Server();
    tcpServer->start();

    connect(_connectionTimer, &QTimer::timeout, this, &EthernetController::checkConnection);
    _connectionTimer->start(1000);

    connect(tcpServer, &Tcp_Server::boardStatusChanged, this, &EthernetController::checkBoards);

    filligArraysAndVars();
}

EthernetController::~EthernetController() {

}

void EthernetController::filligArraysAndVars()
{
    for(int i=0;i<2;i++)
        for(int j=0;j<283;j++)
            for(int k=0;k<8;k++)
                _res[i][j][k] = false;

    m_boardStatus.resize(3);
    m_boardStatus.fill(false);

    engineStatus.resize(9);
    engineStatus.fill("OFF");

    for(int i=0;i<8;i++) {
        m_testLeft.push_back(false);
        m_testRight.push_back(false);
    }

    for(int i=0;i<SharedData::_numberOfLines;i++)
    {
        for(int j=0;j<8;j++)
        {
            _weightConditions[i][j] = false;
            _densityConditions[i][j] = false;
            _sizeConditions[i][j] = false;
            _colorConditions[i][j] = false;
            _defectConditions[i][j] = false;
        }
    }

    _lnaCup.resize(8);
    _lnaCup.fill(0);

    _lnbCup.resize(8);
    _lnbCup.fill(0);

    m_totalCountLeft.resize(8);
    m_totalCountLeft.fill(0);

    m_totalCountRight.resize(8);
    m_totalCountRight.fill(0);

    _totalWeightLeft.resize(8);
    _totalWeightLeft.fill(0);

    _totalWeightRight.resize(8);
    _totalWeightRight.fill(0);

    m_totalWeightOutput.resize(8);
    m_totalWeightOutput.fill("0");

    setTotalWeight("0");

}

QString formatWithSpaces(const QString &numberStr) {
    QtConcurrent::run([=](){
        QString formattedNumber;
        int len = numberStr.length();

        // Reverse iterate through the number string and insert spaces
        for (int i = 0; i < len; ++i) {
            if (i > 0 && (len - i) % 3 == 0) {
                formattedNumber.append(' ');
            }
            formattedNumber.append(numberStr.at(i));
        }
        return formattedNumber;
    });
}


void EthernetController::checkBoards()
{
    m_boardStatus[0] = tcpServer->boards[0];
    m_boardStatus[1] = tcpServer->boards[1];
    m_boardStatus[2] = tcpServer->boards[2];

    Q_EMIT boardStatusChanged();
}

void EthernetController::assignControllers(WeightController *wc, CameraController *cc)
{
    _weightController = wc;
    _cameraController = cc;

    _weightController->_output = _output;
    _cameraController->_output = _output;
}

void EthernetController::assignDb(DatabaseController *db)
{
    _db = db;
}

void EthernetController::clearArrays()
{
    m_ruleID.clear();
    m_wLow.clear();
    m_wHigh.clear();
    m_enables.clear();
    m_sizeDensity.clear();
    m_outputsUsed.clear();
    _colorCondition.clear();
    _defectEnables.clear();

    _ejectorA_Ips.clear();
    _ejectorA_Outputs.clear();

    _ejectorB_Ips.clear();
    _ejectorB_Outputs.clear();
}

void EthernetController::sendEject()
{
    tcpServer->_ejectorBoards[0]->_posFinal = _pulseReader->_posFinal;
    tcpServer->_ejectorBoards[1]->_posFinal = _pulseReader->_posFinal;

    tcpServer->_ejectorBoards[0]->_cup = _pulseReader->_cup;
    tcpServer->_ejectorBoards[1]->_cup = _pulseReader->_cup;

    tcpServer->_ejectorBoards[0]->sendEjectCommand(_ejectCommand_L);
    tcpServer->_ejectorBoards[1]->sendEjectCommand(_ejectCommand_R);
}

void EthernetController::readDb(QString exf)
{
    connect(_pulseReader, &PulseReader::encoderChanged, this, &EthernetController::sendEject);

    QVector<QVector<QString>> res = _db->readOutputs(exf);

    this->exf = exf;

    clearArrays();

    for(const auto &r:res[0])
        m_ruleID.push_back(r);

    for(const auto &r:res[1])
        m_wLow.push_back(r);

    for(const auto &r:res[2])
        m_wHigh.push_back(r);

    for(const auto &r:res[3])
        m_sizeDensity.push_back(r);

    for(const auto &r:res[4])
        m_enables.push_back(r);

    for(const auto &r:res[5])
        _defectEnables.push_back(r);

    for(const auto &r:res[7])
        _colorCondition.push_back(r);

    for(const auto &r:res[8])
        m_outputsUsed.push_back(r);

    //qDebug() << "m_o" << m_enables;


    // qDebug() << m_sizeDensity;

    Q_EMIT wLowChanged();
}

void EthernetController::checkMotorsStatus()
{
    b1Status = {_db->motor()[2], _db->motor()[3], _db->motor()[6], _db->motor()[10], _db->motor()[11], "OFF",
        _db->motor()[7], _db->motor()[8], _db->motor()[9]
    };

    for(int i=0;i<9;i++)
    {
        tcpServer->_motorBoard->_engineStatus[i] = b1Status[i];
    }

    t2Status = {_db->motor()[9]};

    if(_pulseReader->_cup - _currentCup == 20 && (tcpServer->_motorBoard->t2Bytes == 2 || tcpServer->_motorBoard->t2Bytes == 3))
    {
        tcpServer->_motorBoard->sendStr = "Z:000001111100";

        if(m_graderStatus != Grading)
            _pulseReader->_cup = 0;

        tcpServer->_motorBoard->tenFinished = true;

        _db->readLna(_speed);
        _db->readLnb(_speed);

        if(_weightController->_lines.size() == 2)
        {
            _weightController->_lines[0]->setVariables(_db->lnaSetting());
            _weightController->_lines[1]->setVariables(_db->lnbSetting());
        }
    }

    if(_pulseReader->_cup == 10 && (tcpServer->_motorBoard->t2Bytes |= 2 || tcpServer->_motorBoard->t2Bytes != 3))
        tcpServer->_motorBoard->sendStr[13] = '0';

    if(_pulseReader->_cup == _calRounds * SharedData::_totalCupNumber)
    {
        tcpServer->_motorBoard->sendStr = "Z:000001111100";
    }

    if(_pulseReader->_cup == _calRounds * SharedData::_totalCupNumber + 20)
    {
        tcpServer->_motorBoard->sendStr[13] = '0';
    }

    if(_pulseReader->_cup >= _calRounds * SharedData::_totalCupNumber + 20)
    {
        QtConcurrent::run([=](){
            if(m_autoFeeding)
            {
                tcpServer->_motorBoard->_calibratingFinished = true;
            }
        });
    }
}


bool EthernetController::evaluateColorCondition(const QString &condition, std::vector<int> colorPercents)
{
    static QJSEngine engine;

    // Expose the color percentages to the script
    engine.globalObject().setProperty("rd", colorPercents[0]);
    engine.globalObject().setProperty("or", colorPercents[1]);
    engine.globalObject().setProperty("yl", colorPercents[2]);
    engine.globalObject().setProperty("gn", colorPercents[3]);

    // Create a script with the given condition
    QString script = QString("result = (%1);").arg(condition);

    // Compile the script if it's not already compiled
    QJSValue result = engine.evaluate(script);

    // Check the result and convert it to a boolean
    if (result.isBool())
        return result.toBool();

    return false;
}

void EthernetController::encoderPulseChanged()
{
    if(programStarted)
    {
        qDebug() << "___________________________" << _pulseReader->_posFinal;

        readDataEthernet(_pulseReader->_posFinal, _pulseReader->_prevPosFinal);

        if(_cameraController->m_cameraIsStarted[0] && _cameraController->m_cameraIsStarted[1])
        {
            _cameraController->mainLoop(_pulseReader->_posFinal, _pulseReader->_cup);
        }

        _weightController->mainLoop(_pulseReader->_posFinal, _pulseReader->_cup);
    }
}

void EthernetController::setupEthernetConnection()
{
    qDebug() << "---" << Q_FUNC_INFO;

    _pulseReader->_cup = 0;
    setGraderStatus(Calibrating);

    connect(_pulseReader, &PulseReader::encoderChanged, this, &EthernetController::encoderPulseChanged);

    QObject::connect(_weightController, &WeightController::weightingFinished, this, &EthernetController::eject);

    programStarted = true;
}

void EthernetController::readDataEthernet(int pos, int prevPos)
{
    if(programStarted)
    {
        _en = pos;

        _connectionTryCounter = 0;

        if(0 <= _pulseReader->_cup && _pulseReader->_cup < SharedData::_totalCupNumber)
        {
            setGraderStatus(GraderStatus::Calibrating);
        }

        _cupID = _pulseReader->_cup % SharedData::_totalCupNumber;

        if(_pulseReader->_cup >= _calRounds * SharedData::_totalCupNumber) setGraderStatus(GraderStatus::Grading);
        //qDebug() << _finalLeft << _finalRight;

        _weightController->pushToWeights(0, _pulseReader->_finalLeft);
        _weightController->m_posLeft.push_back(pos);

        _weightController->pushToWeights(1, _pulseReader->_finalRight);
        _weightController->m_posRight.push_back(pos);

        if (prevPos > 80 && pos < 20)
        {
            stepCup();
            calcElapsedTime();
        }
    }

}

void EthernetController::setBytesToSend()
{

}

void EthernetController::start(QString speed)
{
    setCup(0);
    m_totalCountLeft.fill(0);
    m_totalCountRight.fill(0);
    _totalWeightLeft.fill(0);
    _totalWeightRight.fill(0);
    m_totalWeightOutput.fill("0");

    _weightController->m_weightLeft.clear();
    _weightController->m_weightRight.clear();

    Q_EMIT totalCountLeftChanged();
    Q_EMIT totalCountRightChanged();
    Q_EMIT totalWeightOutputChanged();

    connect(this, &EthernetController::ejectDecisionFinished, this, &EthernetController::updateUI);

    setTotalWeight("0");

    //TODO might need to chnage the place of it
    _db->readGenerals(exf);
    _db->readGeneral();


    _distanceFromLC = _db->gen()[2].toInt();

    _db->readLna(speed);
    _db->readLnb(speed);

    _db->readEjectorA();
    _db->readEjectorB();

    for(auto &str:_db->ejectorA())
    {
        _ejectorA_Ips.push_back(str.split(',')[0]);
        _ejectorA_Outputs.push_back((str.split(',')[1]).toInt());
    }

    tcpServer->_ejectorBoards[0]->_outputs =  _ejectorA_Outputs;

    for(auto &str:_db->ejectorB())
    {
        _ejectorB_Ips.push_back(str.split(',')[0]);
        _ejectorB_Outputs.push_back((str.split(',')[1]).toInt());
    }

    tcpServer->_ejectorBoards[1]->_outputs =  _ejectorB_Outputs;

    for(int i=7;i<=14;i++) {
        _lnaCup[i - 7] = _db->lnaSetting()[i].toInt();
        _lnbCup[i - 7] = _db->lnbSetting()[i].toInt();
    }

    tcpServer->_lnaCup = _lnaCup;
    tcpServer->_lnbCup = _lnbCup;
    tcpServer->_distanceFromLC = _distanceFromLC;
    tcpServer->_distanceFromLC = _distanceFromLC;
    tcpServer->_ejectorBoards[0]->_shift =  _db->gen()[0].toInt();

    tcpServer->_energize =  _db->lnaSetting()[16].toInt();
    tcpServer->_ejectorBoards[0]->_energize = _db->lnaSetting()[16].toInt();
    tcpServer->_ejectorBoards[1]->_energize = _db->lnbSetting()[16].toInt();
    _pulseReader->_shift = _db->gen()[0].toInt();
    _pulseReader->_energize = _db->lnaSetting()[16].toInt();
    _calRounds = _db->gen()[1].toInt();
    _weightController->_calRounds = _calRounds;

    tcpServer->setVars();

    if(_cameraController->m_serials.size() == 2)
        if(_cameraController->m_startCameras)
        {
            if(!_cameraController->m_cameraIsStarted[0] && !_cameraController->m_cameraIsStarted[1])
            {
                _cameraController->startAnalise(_pulseReader->_cup);
            }
        }

    setupEthernetConnection();

    connect(_reportTimer, &QTimer::timeout, this, &EthernetController::updateReport);
    _reportTimer->start(1000);
    insertReport(exf);

    Start_time = QTime::currentTime();
}

void EthernetController::makeEjectCommand()
{
    for(int i=0;i<_maxStringLength;i++)
    {
        _ejectCommand_L[i + 2] = '0';
        _ejectCommand_R[i + 2] = '0';
    }

    for(int i=0;i<8;i++)
    {
        int index_L = (_cupID % SharedData::_totalCupNumber + 283 - _distanceFromLC - _lnaCup[i]) % 283;

        for(auto o:_boardOutputs[0][index_L])
            _ejectCommand_L[o + 2] = '1';

        int index_R = (_cupID % SharedData::_totalCupNumber + 283 - _distanceFromLC - _lnbCup[i]) % 283;

        for(auto o:_boardOutputs[1][index_R])
            _ejectCommand_R[o + 2] = '1';
    }
}

void EthernetController::updateUI()
{
    makeEjectCommand();


    if(m_errorCupA)
    {
        if(abs(_wLeft) >= m_errorCupValueA)
        {
            int index = _ejectorA_Ips[0] == tcpServer->_ejectorBoards[0]->_ip ? 0 : 1;

            _boardOutputs[index][_ejectCup].push_back(_ejectorA_Outputs[0]);

            tcpServer->_ejectorBoards[0]->_cupEjectorNum[_ejectCup] = 0;
            _weightController->m_flagTableA = false;

            Q_EMIT stopChartA();
        }
    }


    if(m_errorCupB)
        if(abs(_wRight) >= m_errorCupValueB)
        {
            int index = _ejectorB_Ips[0] == tcpServer->_ejectorBoards[0]->_ip ? 0 : 1;

            _boardOutputs[index][_ejectCup].push_back(_ejectorB_Outputs[0]);

            tcpServer->_ejectorBoards[1]->_cupEjectorNum[_ejectCup] = 0;
            _weightController->m_flagTableB = false;

            Q_EMIT stopChartB();
        }


    if(m_errorCupA == false)
    {
        if(ind_res != -1)
        {
            if(_res_L[_ejectCup][ind_res] != 0)
            {
                m_totalCountLeft[ind_res]++;
                _totalWeightLeft[ind_res] += _wLeft;

                Q_EMIT totalCountLeftChanged();
            }
        }
    }

    if(m_errorCupB == false)
    {
        if(ind_res2 != -1)
        {
            if(_res_R[_ejectCup][ind_res2] != 0)
            {
                m_totalCountRight[ind_res2]++;
                _totalWeightRight[ind_res2] += _wRight;

                Q_EMIT totalCountRightChanged();
            }
        }
    }

    for(int i=0;i<8;i++)
    {
        _totalWeights[i] = (_totalWeightLeft[i] + _totalWeightRight[i]) / 1000;
    }

    for(int i=0;i<8;i++)
    {
        if(_totalWeights[i] < 1)
        {
            if(_totalWeights[i] != 0)
            {
                m_totalWeightOutput[i] = QString::number(_totalWeights[i], 'f', 3);

                Q_EMIT totalWeightOutputChanged();
            }
        }
        else if (_totalWeights[i] >= 1 && _totalWeights[i] < 10){
            m_totalWeightOutput[i] = QString::number(_totalWeights[i], 'f', 2);
            Q_EMIT totalWeightOutputChanged();

        }
        else if(_totalWeights[i] >= 10 && _totalWeights[i] < 1000) {
            m_totalWeightOutput[i] = QString::number(_totalWeights[i], 'f', 1);
            Q_EMIT totalWeightOutputChanged();

        }
        else if(_totalWeights[i] >= 1000) {
            m_totalWeightOutput[i] = formatWithSpaces(QString::number(_totalWeights[i]));
            Q_EMIT totalWeightOutputChanged();

        }

    }

    double _sumWeight = 0;

    for(int i=0;i<8;i++)
    {
        _sumWeight += _totalWeights[i];
    }

    if(_sumWeight < 1)
    {
        if(_sumWeight != 0)
            setTotalWeight(QString::number(_sumWeight, 'f', 3));
    }
    else if (_sumWeight >= 1 && _sumWeight < 10){
        setTotalWeight(QString::number(_sumWeight, 'f', 2));
    }
    else if(_sumWeight >= 10 && _sumWeight < 1000) {
        setTotalWeight(QString::number(_sumWeight, 'f', 1));
    }
    else if(_sumWeight >= 1000) {
        setTotalWeight(formatWithSpaces(QString::number(_sumWeight)));
    }

}

void EthernetController::insertReport(QString exf)
{
    _report.clear();
    QDateTime currentDateTime = QDateTime::currentDateTime();

    QLocale englishLocale(QLocale::English);

    start_report = englishLocale.toString(currentDateTime, "yyyy-MM-dd hh:mm:ss");

    _report.push_back(start_report);
    _report.push_back("");
    _report.push_back(exf);
    for(int i=0;i<=10;i++) {
        _report.push_back("0");
    }

    _db->setReport(_report);
    _db->insert("Report");
}

void EthernetController::updateReport()
{
    QDateTime currentDateTime = QDateTime::currentDateTime();

    QLocale englishLocale(QLocale::English);

    QString formattedDateTime = englishLocale.toString(currentDateTime, "yyyy-MM-dd hh:mm:ss");

    _report.clear();
    _report.push_back(start_report);
    _report.push_back(formattedDateTime);
    _report.push_back(exf);
    for(int i=0;i<8;i++) {
        _report.push_back(m_totalWeightOutput[i]);
    }

    int totalMinutes = Start_time.secsTo(QTime::currentTime()) / 60;
    int totalHours = totalMinutes / 60;

    _report.push_back(QString::number(totalHours));
    _report.push_back(m_totalWeight);
    //qDebug() << _report;
    _db->setReport(_report);
    _db->updateReport(start_report);
}

void EthernetController::writeToExcel() {
    QString filename = "/home/perticon/report/" + _db->m_reportData[0][0].split(" ")[0] + ".xlsx";

    // Convert the QString to a const char*
    const char *filenameChar = filename.toUtf8().constData();

    lxw_workbook *workbook  = workbook_new(filenameChar);
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

    worksheet_write_string(worksheet, 0, 0, "Start Time", NULL);
    worksheet_write_string(worksheet, 0, 1, "Stop Time", NULL);
    worksheet_write_string(worksheet, 0, 2, "Exit Feature", NULL);
    worksheet_write_string(worksheet, 0, 3, "Exit A", NULL);
    worksheet_write_string(worksheet, 0, 4, "Exit B", NULL);
    worksheet_write_string(worksheet, 0, 5, "Exit C", NULL);
    worksheet_write_string(worksheet, 0, 6, "Exit D", NULL);
    worksheet_write_string(worksheet, 0, 7, "Exit E", NULL);
    worksheet_write_string(worksheet, 0, 8, "Exit F", NULL);
    worksheet_write_string(worksheet, 0, 9, "Exit G", NULL);
    worksheet_write_string(worksheet, 0, 10, "Exit H", NULL);
    worksheet_write_string(worksheet, 0, 11, "Total RunTime", NULL);
    worksheet_write_string(worksheet, 0, 12, "Total Weight", NULL);

    for(int i=0;i<_db->m_reportData.size();i++) {
        for(int j=0;j<_db->m_reportData[i].size();j++)
        {
            if(j <= 2)
                worksheet_write_string(worksheet, i + 1, j % 13, _db->m_reportData[i][j].toUtf8().constData(), NULL);
            else
                worksheet_write_number(worksheet, i + 1, j % 13, _db->m_reportData[i][j].toDouble(), NULL);
        }
    }

    // Save the workbook and free memory.
    int result = workbook_close(workbook);

    if (result == LXW_ERROR_CREATING_XLSX_FILE) {
        fprintf(stderr, "Error creating XLSX file\n");
    }
}

void EthernetController::stepCup()
{
    //_cup++;
    setCup(_pulseReader->_cup);
    //qDebug() << _pulseReader->_cup;
}

void EthernetController::setSizeAndDensity()
{
    int _size[2] = {0};
    int _density[2] = {0};

    for(int i=0;i<SharedData::_batchSize / 2;i++)
    {
        _size[0] += _output->lines[0].cups[_cupID].Images[i].size;
        _size[1] += _output->lines[1].cups[_cupID].Images[i].size;

        _density[0] += _output->lines[0].cups[_cupID].Images[i].density;
        _density[1] += _output->lines[1].cups[_cupID].Images[i].density;
    }
    // if(_cup%2) a=15;
    // QByteArray txDatagram;
    // txDatagram.resize(2);
    // txDatagram[0] = 'P';
    // txDatagram[0] = a + 128;

    _size[0] /= (SharedData::_batchSize / 2);
    _size[1] /= (SharedData::_batchSize / 2);

    _density[0] /= (SharedData::_batchSize / 2);
    _density[1] /= (SharedData::_batchSize / 2);

    // for(int i=0;i<_sizeCondition.size();i++)
    // {
    //     _sizeConditions[0][i]= evaluateSizeCondition(_sizeCondition[i], _size[0]);
    //     _sizeConditions[0][i]= evaluateSizeCondition(_sizeCondition[i], _size[1]);

    //     _densityConditions[0][i]= evaluateDensityCondition(_densityCondition[i], _density[0]);
    //     _densityConditions[0][i]= evaluateDensityCondition(_densityCondition[i], _density[1]);
    // }
}

void EthernetController::setColor(int idx, int outputID, QVector<bool> _colorIsEnabled)
{
    for(auto &enable:m_enables)
    {
        _colorIsEnabled.push_back(enable.split(',')[1] == "true" ? true : false);
    }

    int _avgOfColors[2][5];

    for(int i=0;i<2;i++)
        for(int j=0;j<5;j++)
            _avgOfColors[i][j] = 0;

    int j = 0;

    while(j < SharedData::_numberOfColors)
    {
        int sum = 0, sum2 = 0;

        for(int i=0;i<SharedData::_batchSize / 2;i++)
        {
            sum += _output->lines[0].cups[_ejectCup].Images[i].colorPercents[j];
            sum2 += _output->lines[1].cups[_ejectCup].Images[i].colorPercents[j];
        }

        _avgOfColors[0][j] = sum / (SharedData::_batchSize / 2);
        _avgOfColors[1][j] = sum2 / (SharedData::_batchSize / 2);

        j++;
    }

    if(_avgOfColors[1][0] > 40)
    {
        qDebug() << "rrrr" << _ejectCup << "_" << _wRight << _avgOfColors[1][0] << _avgOfColors[1][1] << _avgOfColors[1][2] << _avgOfColors[1][3] << _avgOfColors[1][4];
    }

    if(_avgOfColors[1][2] > 40)
    {
        qDebug() << "yyyy" << _ejectCup << "_" << _wRight << _avgOfColors[1][0] << _avgOfColors[1][1] << _avgOfColors[1][2] << _avgOfColors[1][3] << _avgOfColors[1][4];
    }


    if(_colorIsEnabled[idx])
    {
        _colorConditions[0][outputID] = evaluateColorCondition(_colorCondition[idx], {_avgOfColors[0][0], _avgOfColors[0][1], _avgOfColors[0][2], _avgOfColors[0][3]});
        _colorConditions[1][outputID] = evaluateColorCondition(_colorCondition[idx], {_avgOfColors[1][0], _avgOfColors[1][1], _avgOfColors[1][2], _avgOfColors[1][3]});
    }
    else
    {
        _colorConditions[0][outputID] = true;
        _colorConditions[1][outputID] = true;
    }

}

void EthernetController::setWeight(int idx, int outputID)
{

    if(m_wLow[idx].toInt() < _wLeft && _wLeft <= m_wHigh[idx].toInt())
    {
        _weightConditions[0][outputID] = true;
    }

    if(m_wLow[idx].toInt() < _wRight && _wRight <= m_wHigh[idx].toInt())
    {
        qDebug() << "wwww" << _wRight;

        _weightConditions[1][outputID] = true;
    }

}

void EthernetController::setDefect(int idx, int outputID, QVector<bool> _defectIsEnabled)
{
    int _numberOfDefects[2][SharedData::_numberOfClasses] = {{0}};

    int _imageHasDefects[2][SharedData::_numberOfClasses] = {{0}};

    QStringList _defects = _defectEnables[idx].split(',');

    for(int i=0;i<2;i++)
    {
        for(int j=0;j<SharedData::_numberOfClasses;j++)
        {
            _numberOfDefects[i][j] = 0;
            _imageHasDefects[i][j] = 0;
        }
    }


    //auto start = std::chrono::high_resolution_clock::now();

    for(auto &enable:m_enables)
    {
        _defectIsEnabled.push_back(enable.split(',')[0] == "true" ? true : false);
    }

    int j= 0;

    while(j < SharedData::_numberOfClasses) // Defect Counting
    {
        int sum = 0, sum2 = 0;

        for(int i=0;i<SharedData::_batchSize / 2;i++)
        {
            sum += _output->lines[0].cups[_ejectCup].Images[i].numberOfDefects[j];
            sum2 += _output->lines[1].cups[_ejectCup].Images[i].numberOfDefects[j];

            _imageHasDefects[0][j] += _output->lines[0].cups[_ejectCup].Images[i].detectedObjects[j];
            _imageHasDefects[1][j] += _output->lines[1].cups[_ejectCup].Images[i].detectedObjects[j];
        }

        _numberOfDefects[0][j] = sum;
        _numberOfDefects[1][j] = sum2;

        j++;
    }


    if(_defectIsEnabled[idx])
    {
        if(_imageHasDefects[0][0] >= 1) // Check if there is an apple in the image
        {
            if
                (
                    _defects[0] == "false" && _defects[1] == "false" && _defects[2] == "false" &&
                    _defects[3] == "false" && _defects[4] == "false"
                    )
            {
                if
                    (
                        _imageHasDefects[0][1] == false && _imageHasDefects[0][2] == false && _imageHasDefects[0][5] == false &&
                        _imageHasDefects[0][6] == false && _imageHasDefects[0][7] == false
                        )
                {
                    _defectConditions[0][outputID] = true;
                }
            }
            else
            {
                int k = 0;
                for(int i=1;i<SharedData::_numberOfClasses;i++)
                {

                    if(i!=3 && i!=4) // check if there is not top and bottom
                    {
                        bool isEnabled;
                        isEnabled = _defects[k] == "true" ? true : false;

                        if(_imageHasDefects[0][i] && isEnabled)
                        {
                            _defectConditions[0][outputID] = true;
                            break;
                        }
                        else if(_cameraController->m_quantityEnable && _numberOfDefects[0][i] > _cameraController->m_quantityBasedClassCountList[i] && isEnabled)
                        {
                            _defectConditions[0][outputID] = true;

                            break;
                        }
                        else _defectConditions[0][outputID] = false;
                        k++;
                    }
                }
            }

        }


        if(_imageHasDefects[1][0] >= 1) // Check if there is an apple in the image
        {
            //qDebug() << "EJECT______" << _imageHasDefects[1][1] << _imageHasDefects[1][2] << _imageHasDefects[1][5] << _imageHasDefects[1][6] << _imageHasDefects[1][7];
            if
                (
                    _defects[0] == "false" && _defects[1] == "false" && _defects[2] == "false" &&
                    _defects[3] == "false" && _defects[4] == "false"
                    )
            {
                if
                    (
                        _imageHasDefects[1][1] == 0 && _imageHasDefects[1][2] == 0 && _imageHasDefects[1][5] == 0 &&
                        _imageHasDefects[1][6] == 0 && _imageHasDefects[1][7] == 0
                        )
                {
                    _defectConditions[1][outputID] = true;
                }
            }
            else
            {
                int k = 0;
                for(int i=1;i<SharedData::_numberOfClasses;i++)
                {

                    if(i!=3 && i!=4) // check if there is not top and bottom
                    {
                        bool isEnabled;
                        isEnabled = _defects[k] == "true" ? true : false;

                        if(_imageHasDefects[1][i] && isEnabled)
                        {
                            _defectConditions[1][outputID] = true;
                            break;
                        }
                        else if(_cameraController->m_quantityEnable && _numberOfDefects[1][i] > _cameraController->m_quantityBasedClassCountList[i] && isEnabled)
                        {
                            _defectConditions[1][outputID] = true;

                            break;
                        }
                        else _defectConditions[1][outputID] = false;
                        k++;
                    }
                }
            }

        }

    }
    else
    {
        _defectConditions[0][outputID] = true;
        _defectConditions[1][outputID] = true;
    }
}

void EthernetController::eject()
{
    if(programStarted)
    {
        auto start = std::chrono::high_resolution_clock::now();

        //qDebug() << _pulseReader->_cup;
        _ejectCup = (_cupID + 283 - _distanceFromLC) % 283;

        _wLeft = _output->lines[0].cups[_ejectCup].weight;
        _wRight = _output->lines[1].cups[_ejectCup].weight;

        //qDebug() << _ejectCup << "_____________" << _wRight;

        QtConcurrent::run([=](){
            QVector<QVector<int>> _outpuRules;

            for(int i=0;i<8;i++)
            {
                QVector<int> indexes;
                for(int j=0;j<m_outputsUsed.size();j++)
                {
                    if(m_outputsUsed[j].split(',')[i] == "true") indexes.push_back(j);
                }

                _outpuRules.push_back(indexes);
            }



            tcpServer->_ejectorBoards[0]->_cupEjectorNum[_ejectCup] = -1;
            tcpServer->_ejectorBoards[1]->_cupEjectorNum[_ejectCup] = -1;

            _boardOutputs[0][_ejectCup].clear();
            _boardOutputs[1][_ejectCup].clear();

            for(int i=0;i<8;i++)
            {
                _res_L[_ejectCup][i] = 0;
                _res_R[_ejectCup][i] = 0;
            }


            if(m_graderStatus == Grading)
            {

                val_res=-1  ;cnt_res=-1 , ind_res=-1;
                val_res2=-1  ;cnt_res2=-1 , ind_res2=-1;
                QVector<bool> _colorIsEnabled;
                QVector<bool> _defectIsEnabled;

                for(auto &enable:m_enables)
                {
                    _colorIsEnabled.push_back(enable.split(',')[1] == "true" ? true : false);
                }

                for(auto &enable:m_enables)
                {
                    _defectIsEnabled.push_back(enable.split(',')[0] == "true" ? true : false);
                }

                for(int i=0;i<8;i++)
                {
                    for(auto idx:_outpuRules[i])
                    {
                        //qDebug() << "ej-----" << idx << i << _cup << _ejectCup;
                        setWeight(idx, i);
                        setDefect(idx, i, _defectIsEnabled);
                        setColor(idx, i, _colorIsEnabled);

                        if(_cameraController->m_serials.empty() || !_cameraController->m_startCameras)
                        {

                            for(int x=0;x<2;x++)
                                for(int y=0;y<8;y++)
                                {
                                    _densityConditions[x][y] = true;
                                    _sizeConditions[x][y] = true;
                                    _colorConditions[x][y] = true;
                                    _defectConditions[x][y] = true;
                                }

                            if(_weightConditions[0][i])
                                _res_L[_ejectCup][i] += 2;

                            if(_weightConditions[1][i])
                                _res_R[_ejectCup][i] += 2;

                        }
                        else
                        {
                            if(_defectConditions[0][i])
                                if(_defectIsEnabled[i])
                                    _res_L[_ejectCup][i] += 4;
                            if(_weightConditions[0][i])
                                _res_L[_ejectCup][i] += 2;
                            if(_colorConditions[0][i])
                                if(_colorIsEnabled[i])
                                    _res_L[_ejectCup][i] += 1;

                            if(_defectConditions[1][i])
                                if(_defectIsEnabled[i])
                                    _res_R[_ejectCup][i] += 4;
                            if(_weightConditions[1][i])
                                _res_R[_ejectCup][i] += 2;
                            if(_colorConditions[1][i])
                                if(_colorIsEnabled[i])
                                    _res_R[_ejectCup][i] += 1;

                            //if(_weightConditions[1][i]) qDebug() << _ejectCup << "mmmmmmmmmmmmmmmmmmmmmm";
                        }


                        // tcpServer->_ejectorBoards[0]->_res[0][_ejectCup][i] |= _weightConditions[0][i] && _defectConditions[0][i] && _colorConditions[0][i];
                        // tcpServer->_ejectorBoards[1]->_res[0][_ejectCup][i] |= _weightConditions[1][i] && _defectConditions[1][i] && _colorConditions[1][i];

                    }

                    for(int x=0;x<2;x++)
                        for(int y=0;y<8;y++)
                        {
                            _weightConditions[x][y] = false;
                            _densityConditions[x][y] = false;
                            _sizeConditions[x][y] = false;
                            _colorConditions[x][y] = false;
                            _defectConditions[x][y] = false;
                        }
                }

                for(int i=0;i<8;i++)
                {
                    int val = _res_L[_ejectCup][i];
                    int cnt = m_totalCountLeft[i];

                    if((val % 4) / 2 > 1)
                    {
                        if (val > val_res && val > 0){
                            val_res = val;
                            cnt_res = cnt;
                            ind_res = i;
                        }
                        else if (val == val_res && val > 0){
                            if (cnt < cnt_res){
                                val_res = val;
                                cnt_res = cnt;
                                ind_res = i;
                            }
                        }
                    }
                }

                for(int i=0;i<8;i++)
                {
                    int val = _res_R[_ejectCup][i];
                    int cnt = m_totalCountRight[i];

                    if((val % 4) / 2 > 1)
                    {
                        if (val > val_res2 && val > 0){
                            val_res2 = val;
                            cnt_res2 = cnt;
                            ind_res2 = i;
                        }
                        else if (val == val_res2 && val > 0){
                            if (cnt < cnt_res2){
                                val_res2 = val;
                                cnt_res2 = cnt;
                                ind_res2 = i;
                            }
                        }
                    }
                }


                if(ind_res != -1)
                {
                    QString ip = _ejectorA_Ips[ind_res];

                    int index = _ejectorA_Ips[ind_res] == tcpServer->_ejectorBoards[0]->_ip ? 0 : 1;

                    _boardOutputs[index][_ejectCup].push_back(_ejectorA_Outputs[ind_res]);
                }

                if(ind_res2 != -1)
                {
                    int index = _ejectorB_Ips[ind_res2] == tcpServer->_ejectorBoards[0]->_ip ? 0 : 1;

                    _boardOutputs[index][_ejectCup].push_back(_ejectorA_Outputs[ind_res]);
                }
            }

            if(ind_res2 != -1)
            {
                // qDebug() << "T" << _ejectCup << tcpServer->_ejectorBoards[1]->_res[_ejectCup][0] << tcpServer->_ejectorBoards[1]->_res[_ejectCup][1] <<
                //     tcpServer->_ejectorBoards[1]->_res[_ejectCup][2] <<  tcpServer->_ejectorBoards[1]->_res[_ejectCup][3] <<
                //     tcpServer->_ejectorBoards[1]->_res[_ejectCup][4] << tcpServer->_ejectorBoards[1]->_res[_ejectCup][5] <<
                //     tcpServer->_ejectorBoards[1]->_res[_ejectCup][6] << tcpServer->_ejectorBoards[1]->_res[_ejectCup][7];
                qDebug() << "Index" << _ejectCup << ind_res2;
            }


            for(int i=0;i<SharedData::_numberOfClasses;i++) // Defect Counting
            {
                for(int j=0;j<SharedData::_batchSize / 2;j++)
                {

                    _output->lines[0].cups[_ejectCup].Images[j].numberOfDefects[i] = 0;
                    _output->lines[1].cups[_ejectCup].Images[j].numberOfDefects[i] = 0;

                    _output->lines[0].cups[_ejectCup].Images[j].detectedObjects[i] = false;
                    _output->lines[1].cups[_ejectCup].Images[j].detectedObjects[i] = false;
                }
            }

            _totalWeights.resize(8);
            _totalWeights.fill(0);

            Q_EMIT ejectDecisionFinished();
        });




        //qDebug() << "_____" << _ejectCup << _wRight << tcpServer->_ejectorBoards[1]->_res[0][_ejectCup][4];

        // if(tcpServer->_ejectorBoards[1]->_res[0][_ejectCup][0])
        //     qDebug() << "Ejc___________" << _ejectCup;



        auto end = std::chrono::high_resolution_clock::now();

        //qDebug() << "Color Percents Done____________: " << _cup;


        qDebug() << "Eject time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }
    //qDebug() << "ej" << _cup;


    //setTotalWeight(_sumWeight);
    //auto end = std::chrono::high_resolution_clock::now();

    //eoooooooooo

}

void EthernetController::startGrader(QString speed)
{
    _speed = speed;


    _db->readLna("2");
    _db->readLnb("2");

    if(_weightController->_lines.size() == 2)
    {
        _weightController->_lines[0]->setVariables(_db->lnaSetting());
        _weightController->_lines[1]->setVariables(_db->lnbSetting());
    }

    _currentCup = _pulseReader->_cup;

    tcpServer->_motorBoard->startAuto = true;

    // b1Status = {_db->motor()[1], _db->motor()[4], _db->motor()[12], _db->motor()[13], _db->motor()[0],
    //     _db->motor()[14], _db->motor()[15], _db->motor()[16], _db->motor()[17], _db->motor()[18], _db->motor()[5], "OFF"
    // };

    tcpServer->_motorBoard->sendStr = "Z:000000000000";

    QTimer::singleShot(500, [=](){
        tcpServer->_motorBoard->t1Bytes = 16;
        tcpServer->_motorBoard->sendStr = "Z:000001111100";
        connect(_motorTimer, &QTimer::timeout, this, &EthernetController::checkMotorsStatus);
        _motorTimer->start(1);
    });
}

void EthernetController::startFeeding()
{
    tcpServer->_motorBoard->startFeeding = true;
}

void EthernetController::stopFeeding()
{
    tcpServer->_motorBoard->startFeeding = false;


    for(int i=0;i<9;i++)
    {
        if(i == 4)
            tcpServer->_motorBoard->_engineStatus[i] = "ON";
        else
            tcpServer->_motorBoard->_engineStatus[i] = "OFF";
    }
}

void EthernetController::stopGrader()
{
    tcpServer->_motorBoard->sendStr = "Z:000000000000";

    QTimer::singleShot(500, [=](){
        tcpServer->_motorBoard->sendStr = "Z:000000000000";
    });


    for(int i=0;i<9;i++)
    {
        tcpServer->_motorBoard->_engineStatus[i] = "OFF";
    }
    tcpServer->_motorBoard->startAuto = false;
    tcpServer->_motorBoard->tenFinished = false;

    _motorTimer->stop();


}

void EthernetController::startManual(int boadrID, int motorID, bool checked)
{
    if(checked)
    {
        tcpServer->_motorBoard->sendStr[motorID + 2] = '1';
    }
    else
    {
        tcpServer->_motorBoard->sendStr[motorID + 2] = '0';
    }
}

void EthernetController::startManualTika(int boadrID, int motorID, bool checked)
{
    int a = 0;
    int b = 0;

    if(_speed == "2")       a = 2;
    else if(_speed == "6")  a = 4;
    else if(_speed == "8")  a = 8;
    if(motorID == 0)        a = a+1;



    // tcpServer->_motorBoard->t2Bytes   = a;

    // qDebug() << tcpServer->_motorBoard->t2Bytes << pow(2, motorID);



    // qDebug() << tcpServer->_motorBoard->t2Bytes << pow(2, motorID);
}

void EthernetController::setManualStatus(int idx, bool value)
{
    value ? engineStatus[idx] = "ON" : engineStatus[idx] = "OFF";

    for(int i=0;i<9;i++)
    {
        tcpServer->_motorBoard->_engineStatus[i] = engineStatus[i];
    }
}

void EthernetController::setTikaSpeed(QString speed)
{
    if(tcpServer->_motorBoard)
        tcpServer->_motorBoard->_speed = speed;
}

void EthernetController::mainLoop()
{
    //qDebug() << "Eject__________________________:" << _cup;
    // _cupID = _cup % SharedData::_totalCupNumber;

    // auto start = std::chrono::high_resolution_clock::now();


    // setDefect();
    // setSizeAndDensity();
    // setColor();
    // setWeight();

    // auto end = std::chrono::high_resolution_clock::now();

    //qDebug() << "eject DEcision: -------" << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

void EthernetController::calcElapsedTime()
{
    auto Current_time = QTime::currentTime();
    auto totalNumberOfSeconds = Start_time.secsTo(Current_time);
    int seconds = totalNumberOfSeconds % 60;
    int minutes = (totalNumberOfSeconds / 60) % 60;
    int hours = (totalNumberOfSeconds / 60 / 60);

    QString timeString = QString("%1:%2:%3")
                             .arg(hours, 2, 10, QChar('0'))
                             .arg(minutes, 2, 10, QChar('0'))
                             .arg(seconds, 2, 10, QChar('0'));

    setElapsedTime(timeString);
}

void EthernetController::checkConnection()
{
    if(_pulseReader->_connectionCounter == 0)
        setConnectionStatus(ConnectionStatus::Connected);
    _pulseReader->_connectionCounter++;

    if (_pulseReader->_connectionCounter > 3) setConnectionStatus(ConnectionStatus::Disconnected);

    if (tcpServer->_ejectorBoards[0]->_connectionCounter > 3000)
    {
        m_boardStatus[0] = tcpServer->boards[0] = false;
        Q_EMIT boardStatusChanged();
    }
    if (tcpServer->_ejectorBoards[1]->_connectionCounter > 3000)
    {
        m_boardStatus[1] = tcpServer->boards[1] = false;
        Q_EMIT boardStatusChanged();
    }
    if (tcpServer->_motorBoard->_connectionCounter > 600)
    {
        m_boardStatus[2] = tcpServer->boards[2] = false;
        Q_EMIT boardStatusChanged();
    }

}

void EthernetController::applyGeneral(const QVector<QString> &vars)
{
    _pulseReader->_shift = vars[0].toInt();
    //_pulseReader->_energize = vars[2].toInt();
    _distanceFromLC = vars[1].toInt();
    _weightController->_distanceFromLC = _distanceFromLC;
    _calRounds = vars[3].toInt();;
    _weightController->_calRounds = vars[3].toInt();

    // tcpServer->_ejectorBoards[0]->_energize =  vars[2].toInt();
    // tcpServer->_ejectorBoards[1]->_energize =  vars[2].toInt();

    tcpServer->_ejectorBoards[0]->_distanceFromLC = vars[0].toInt();
    tcpServer->_ejectorBoards[1]->_distanceFromLC = vars[0].toInt();

}

void EthernetController::applyLineA(const QVector<QString> &vars)
{
    for(int i=6;i<=13;i++)
        _lnaCup[i - 6] = vars[i].toInt();

    tcpServer->_ejectorBoards[0]->_lnCup = _lnaCup;

    tcpServer->_ejectorBoards[0]->_energize =  vars[15].toInt();

}

void EthernetController::applyLineB(const QVector<QString> &vars)
{
    for(int i=6;i<=13;i++)
        _lnbCup[i - 6] = vars[i].toInt();

    tcpServer->_ejectorBoards[1]->_lnCup = _lnbCup;

    tcpServer->_ejectorBoards[1]->_energize =  vars[15].toInt();


    //qDebug() << _lnbCup[0];
}

void EthernetController::stop()
{
    setGraderStatus(Stopped);
    programStarted = false;
    tcpServer->_ejectorBoards[0]->_cup = 0;

    tcpServer->_motorBoard->_calibratingFinished = false;

    disconnect(_weightController, &WeightController::weightingFinished, this, &EthernetController::eject);
    disconnect(_motorTimer, &QTimer::timeout, this, &EthernetController::checkMotorsStatus);
    disconnect(_pulseReader, &PulseReader::encoderChanged, this, &EthernetController::sendEject);
    disconnect(this, &EthernetController::ejectDecisionFinished, this, &EthernetController::updateUI);
    disconnect(_reportTimer, &QTimer::timeout, this, &EthernetController::updateReport);
    disconnect(_pulseReader, &PulseReader::encoderChanged, this, &EthernetController::encoderPulseChanged);


    // disconnect(_pulseReader, &PulseReader::encoderChanged, this, &EthernetController::readDataEthernet);

    // disconnect(_pulseReader, &PulseReader::encoderChanged, _weightController, &WeightController::mainLoop);

    // disconnect(_weightController, &WeightController::weightingFinished, this, &EthernetController::eject);
    if(_cameraController->m_cameraIsStarted[0] && _cameraController->m_cameraIsStarted[1])
    {
        _cameraController->m_cameraIsStarted[0] = false;
        _cameraController->m_cameraIsStarted[1] = false;

        _cameraController->stopAnalise();
    }

    for(int i=0;i<283;i++)
    {
        _boardOutputs[0][i].clear();
        _boardOutputs[1][i].clear();

        tcpServer->_ejectorBoards[0]->_cupEjectorNum[i] = -1;
        tcpServer->_ejectorBoards[1]->_cupEjectorNum[i] = -1;
    }


    _weightController->stop();

    for(int j=0;j<283;j++)
        for(int k=0;k<8;k++)
        {
            _res_L[j][k] = 0;
            _res_R[j][k] = 0;
        }

    for(int i=0;i<SharedData::_numberOfLines;i++)
    {
        for(int j=0;j<8;j++)
        {
            _weightConditions[i][j] = false;
            _densityConditions[i][j] = false;
            _sizeConditions[i][j] = false;
            _colorConditions[i][j] = false;
            _defectConditions[i][j] = false;
        }
    }

    for(auto &line:_output->lines)
    {
        for(auto &cup:line.cups)
        {
            cup.weight = 0;
            for(auto &image:cup.Images)
            {
                image.detectedObjects.fill(false);
                image.numberOfDefects.fill(0);
                image.colorPercents.fill(0);
            }
        }
    }

    //server->close();
}

QVector<QString> EthernetController::wLow() const
{
    return m_wLow;
}

void EthernetController::setWLow(const QVector<QString> &newWLow)
{
    if (m_wLow == newWLow)
        return;
    m_wLow = newWLow;
    Q_EMIT wLowChanged();
}

QVector<QString> EthernetController::wHigh() const
{
    return m_wHigh;
}

void EthernetController::setWHigh(const QVector<QString> &newWHigh)
{
    if (m_wHigh == newWHigh)
        return;
    m_wHigh = newWHigh;
    Q_EMIT wHighChanged();
}

QVector<QString> EthernetController::enables() const
{
    return m_enables;
}

void EthernetController::setEnables(const QVector<QString> &newEnables)
{
    if (m_enables == newEnables)
        return;
    m_enables = newEnables;
    Q_EMIT enablesChanged();
}

int EthernetController::getEncoder() const
{
    return m_encoder;
}

void EthernetController::setEncoder(int newEncoder)
{
    if (m_encoder == newEncoder)
        return;
    m_encoder = newEncoder;
    _encoder = newEncoder;
    encoderChanged();
}

GraderStatus EthernetController::graderStatus() const
{
    return m_graderStatus;
}

void EthernetController::setGraderStatus(GraderStatus newGraderStatus)
{
    // if (m_graderStatus == newGraderStatus)
    //     return;
    m_graderStatus = newGraderStatus;
    Q_EMIT graderStatusChanged();
}

ConnectionStatus EthernetController::connectionStatus() const
{
    return m_connectionStatus;
}

void EthernetController::setConnectionStatus(ConnectionStatus newConnectionStatus)
{
    if (m_connectionStatus == newConnectionStatus)
        return;
    m_connectionStatus = newConnectionStatus;
    Q_EMIT connectionStatusChanged();
}

int EthernetController::cup() const
{
    return m_cup;
}

void EthernetController::setCup(int newCup)
{
    if (m_cup == newCup)
        return;
    m_cup = newCup;
    Q_EMIT cupChanged();
}

QString EthernetController::elapsedTime() const
{
    return m_elapsedTime;
}

void EthernetController::setElapsedTime(const QString &newElapsedTime)
{
    if (m_elapsedTime == newElapsedTime)
        return;
    m_elapsedTime = newElapsedTime;
    Q_EMIT elapsedTimeChanged();
}

QVector<bool> EthernetController::testLeft() const
{
    return m_testLeft;
}

void EthernetController::setTestLeft(const QVector<bool> &newTestLeft)
{
    if (m_testLeft == newTestLeft)
        return;
    m_testLeft = newTestLeft;
    Q_EMIT testLeftChanged();
}

QVector<bool> EthernetController::testRight() const
{
    return m_testRight;
}

void EthernetController::setTestRight(const QVector<bool> &newTestRight)
{
    if (m_testRight == newTestRight)
        return;
    m_testRight = newTestRight;
    Q_EMIT testRightChanged();
}

void EthernetController::setIdxTestLeft(int idx, bool checked)
{
    m_testLeft[idx] = checked;
    tcpServer->_ejectorBoards[0]->_test[idx] = checked;
}

void EthernetController::setIdxTestRight(int idx, bool checked)
{
    m_testRight[idx] = checked;
    tcpServer->_ejectorBoards[1]->_test[idx] = checked;
}

void EthernetController::testEachEjector(int lineID, int ejectorID)
{
    tcpServer->_ejectorBoards[0]->sendStr = "Z:000000000000";
    tcpServer->_ejectorBoards[1]->sendStr = "Z:000000000000";

    if(lineID == 0)
    {
        tcpServer->_ejectorBoards[0]->sendStr[ejectorID + 2] = '1';
        tcpServer->_ejectorBoards[0]->flagEject = true;
    }
    else
    {
        tcpServer->_ejectorBoards[1]->sendStr[ejectorID + 2] = '1';
        tcpServer->_ejectorBoards[1]->flagEject = true;
    }

    QTimer::singleShot(50, [=](){
        tcpServer->_ejectorBoards[0]->sendStr = "Z:000000000000";
        tcpServer->_ejectorBoards[1]->sendStr = "Z:000000000000";

        tcpServer->_ejectorBoards[0]->flagEject = true;
        tcpServer->_ejectorBoards[1]->flagEject = true;
    });
}

QVector<int> EthernetController::getTotalCountRight() const
{
    return m_totalCountRight;
}

void EthernetController::setTotalCountRight(const QVector<int> &newTotalCountRight)
{
    if (m_totalCountRight == newTotalCountRight)
        return;
    m_totalCountRight = newTotalCountRight;
    Q_EMIT totalCountRightChanged();
}

QVector<int> EthernetController::getTotalCountLeft() const
{
    return m_totalCountLeft;
}

void EthernetController::setTotalCountLeft(const QVector<int> &newTotalCountLeft)
{
    if (m_totalCountLeft == newTotalCountLeft)
        return;
    m_totalCountLeft = newTotalCountLeft;
    Q_EMIT totalCountLeftChanged();
}

QVector<QString> EthernetController::outputsUsed() const
{
    return m_outputsUsed;
}

void EthernetController::setOutputsUsed(const QVector<QString> &newOutputsUsed)
{
    if (m_outputsUsed == newOutputsUsed)
        return;
    m_outputsUsed = newOutputsUsed;
    Q_EMIT outputsUsedChanged();
}

QVector<QString> EthernetController::sizeDensity() const
{
    return m_sizeDensity;
}

void EthernetController::setSizeDensity(const QVector<QString> &newSizeDensity)
{
    if (m_sizeDensity == newSizeDensity)
        return;
    m_sizeDensity = newSizeDensity;
    Q_EMIT sizeDensityChanged();
}

QVector<QString> EthernetController::ruleID() const
{
    return m_ruleID;
}

void EthernetController::setRuleID(const QVector<QString> &newRuleID)
{
    if (m_ruleID == newRuleID)
        return;
    m_ruleID = newRuleID;
    Q_EMIT ruleIDChanged();
}

QString EthernetController::getTotalWeight() const
{
    return m_totalWeight;
}

void EthernetController::setTotalWeight(const QString &newTotalWeight)
{
    if (m_totalWeight == newTotalWeight)
        return;
    m_totalWeight = newTotalWeight;
    Q_EMIT totalWeightChanged();
}

QVector<QString> EthernetController::getTotalWeightOutput() const
{
    return m_totalWeightOutput;
}

void EthernetController::setTotalWeightOutput(const QVector<QString> &newTotalWeightOutput)
{
    if (m_totalWeightOutput == newTotalWeightOutput)
        return;
    m_totalWeightOutput = newTotalWeightOutput;
    Q_EMIT totalWeightOutputChanged();
}

bool EthernetController::autoGrading() const
{
    return m_autoGrading;
}

void EthernetController::setAutoGrading(bool newAutoGrading)
{
    if (m_autoGrading == newAutoGrading)
        return;
    m_autoGrading = newAutoGrading;
    Q_EMIT autoGradingChanged();
}

bool EthernetController::autoFeeding() const
{
    return m_autoFeeding;
}

void EthernetController::setAutoFeeding(bool newAutoFeeding)
{
    if (m_autoFeeding == newAutoFeeding)
        return;
    m_autoFeeding = newAutoFeeding;
    Q_EMIT autoFeedingChanged();
}

QString EthernetController::getSpeed() const
{
    return _speed;
}

void EthernetController::setSpeed(const QString &newSpeed)
{
    if (_speed == newSpeed)
        return;
    _speed = newSpeed;
    Q_EMIT speedChanged();
}

QVector<bool> EthernetController::boardStatus() const
{
    return m_boardStatus;
}

void EthernetController::setBoardStatus(const QVector<bool> &newBoardStatus)
{
    if (m_boardStatus == newBoardStatus)
        return;
    m_boardStatus = newBoardStatus;
    Q_EMIT boardStatusChanged();
}

int EthernetController::getErrorCupValueA() const
{
    return m_errorCupValueA;
}

void EthernetController::setErrorCupValueA(int newErrorCupValueA)
{
    if (m_errorCupValueA == newErrorCupValueA)
        return;
    m_errorCupValueA = newErrorCupValueA;
    Q_EMIT errorCupValueAChanged();
}

int EthernetController::getErrorCupValueB() const
{
    return m_errorCupValueB;
}

void EthernetController::setErrorCupValueB(int newErrorCupValueB)
{
    if (m_errorCupValueB == newErrorCupValueB)
        return;
    m_errorCupValueB = newErrorCupValueB;
    Q_EMIT errorCupValueBChanged();
}

bool EthernetController::errorCupA() const
{
    return m_errorCupA;
}

void EthernetController::setErrorCupA(bool newErrorCupA)
{
    if (m_errorCupA == newErrorCupA)
        return;
    m_errorCupA = newErrorCupA;
    Q_EMIT errorCupAChanged();
}

bool EthernetController::errorCupB() const
{
    return m_errorCupB;
}

void EthernetController::setErrorCupB(bool newErrorCupB)
{
    if (m_errorCupB == newErrorCupB)
        return;
    m_errorCupB = newErrorCupB;
    Q_EMIT errorCupBChanged();
}
