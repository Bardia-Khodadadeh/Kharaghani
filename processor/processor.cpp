#include "processor.h"
#include "detector_torchscript_yolo.h"
#include "qdebug.h"

Processor::Processor(QObject *parent) : QObject{parent} {}

void Processor::setModelPath(const QString &newModelPath)
{
    qDebug() << "---" << Q_FUNC_INFO << newModelPath;

    _detector = new Detector_TorchScript_Yolo;
    _detector->loadModel(newModelPath);
}

void Processor::setClassesList(const QVector<QString> &newClassesList)
{
    classesList = newClassesList;
}

void Processor::setOutputSize(const cv::Size &newOutputSize)
{
    qDebug() << "---" << Q_FUNC_INFO << newOutputSize.height << newOutputSize.height;

    _outputSize = newOutputSize;
    _detector->setOutputSize(_outputSize);

    _colorDetector->_cropHsv = _cropHsv;
    _colorDetector->_rdHsv = _rdHsv;
    _colorDetector->_orHsv = _orHsv;
    _colorDetector->_ylHsv = _ylHsv;
    _colorDetector->_gnHsv = _gnHsv;
}


void Processor::getColorPercents(std::vector<cv::Mat> &frames, int cupNumber, int lineID)
{
    _colorDetector->_frameIdx = _frameIdx;
    _colorDetector->getColorPercents(frames, cupNumber, lineID);

    if(m_graderStatus == Stopped || m_graderStatus == Calibrating)
    {
        _colorsPer = _colorDetector->_colorsPer;

        _coloredFrames = _colorDetector->_coloredFrames;
        _coloredFrames2 = _colorDetector->_coloredFrames2;

    }
    // _orgFrames1 = _colorDetector->_orgFrames1;
    // _orgFrames2 = _colorDetector->_orgFrames2;
    _colorsPers1 = _colorDetector->_colorsPers1;
    _colorsPers2 = _colorDetector->_colorsPers2;


}

std::vector<std::vector<Detection>> Processor::process(const std::vector<cv::Mat> &inputs, std::vector<cv::Mat> &outputs, int batchID, int camSize)
{
    //    qDebug() << "Process launched ...";
    std::vector<cv::Mat> images;

    for (size_t i = 0; i < inputs.size(); ++i){
        //        images.push_back(inputs[i].clone());
        images.push_back(inputs[i]);
    }

    auto _res = _detector->run(images, batchID);

    for (auto &_batch : _res) {
        for (auto &_det : _batch) {
            _det.className = classesList[_det.classID];
        }
    }

    outputs = inputs;
    QtConcurrent::run(this, &Processor::saveOutputs, outputs, _res, batchID, camSize);


    return _res;
}

void Processor::setterMySetup(bool multiFrame, int numOfLines){
    m_multiFrame = multiFrame;
    m_numOfLines = numOfLines;
}

void Processor::saveOutputs(std::vector<cv::Mat> &inputs, std::vector<std::vector<Detection>> result, int batchID, int camSize)
{
    if(result.size() != inputs.size())
        return;

    cv::Mat img;
    // result = {
    //     result[0], result[1], result[2], result[3], result[8], result[9], result[10],
    //     result[4], result[5], result[6], result[7], result[11], result[12], result[13]
    // };
    _testFrames.clear();
    _lblFrames.clear();


    // _testFrames.push_back(inputs[0]);
    // _testFrames.push_back(inputs[7]);


    if(m_multiFrame){
        for (size_t j = 0; j < result.size(); ++j) {
            if((m_graderStatus == Stopped || m_graderStatus == Calibrating) || _saveResult)
            {
                //_labeledFrame.clear();

                img = inputs[j].clone();
                for (auto &_det : result[j]) {
                    rectangle(img, cv::Point(_det.bbox.x, _det.bbox.y), cv::Point(_det.bbox.x + _det.bbox.width, _det.bbox.y + _det.bbox.height),
                              cv::Scalar(colorOfClasses[_det.classID][0], colorOfClasses[_det.classID][1], colorOfClasses[_det.classID][2]), 2);
                    std::string label = (classesList[_det.classID] + ": " + QString::number((int)(_det.confidence*1000)/10.0)).toStdString();
                    int baseLine;
                    cv::Size label_size = cv::getTextSize(label, cv::FONT_ITALIC, 0.6, 1, &baseLine);
                    auto top = std::max(_det.bbox.y, label_size.height);
                    cv::Point tl = cv::Point(_det.bbox.x, top);
                    cv::Point br = cv::Point(_det.bbox.x + label_size.width, top + label_size.height+2 + baseLine);
                    //rectangle(img, tl, br, cv::Scalar(0, 0, 0), 1);
                    putText(img, label, cv::Point(_det.bbox.x+1, top + label_size.height+3), cv::FONT_ITALIC, 0.6, cv::Scalar(colorOfClasses[_det.classID][0], colorOfClasses[_det.classID][1], colorOfClasses[_det.classID][2]), 2);
                }

                _labeledFrame.push_back(img);
                // _lblFrames.push_back(img);
            }

            if(j == 0)
            {
                i = 0;
                idx = 1;
            }

            if(i == 4)
            {
                i = 0;
                idx = 2;
            }

            if(j == 7)
            {
                i = 0;
                idx = 1;
            }

            std::string _imageName;

            // cv::imwrite("f" + to_string(cup) + "_" + to_string(1) + ".jpg", imgs[4]);
            // cv::imwrite("f" + to_string(cup-1) + "_" + to_string(3) + ".jpg", imgs[5]);
            // cv::imwrite("f" + to_string(cup-2) + "_" + to_string(5) + ".jpg", imgs[6]);
            // cv::imwrite("f" + to_string(cup-3) + "_" + to_string(7) + ".jpg", imgs[7]);
            // cv::imwrite("f" + to_string(cup) + "_" + to_string(2) + ".jpg", imgs[11]);
            // cv::imwrite("f" + to_string(cup-1) + "_" + to_string(4) + ".jpg", imgs[12]);
            // cv::imwrite("f" + to_string(cup-2) + "_" + to_string(6) + ".jpg", imgs[13]);

            if(j < 7)
                _imageName = "L_" + std::to_string(batchID - i) + "_" + std::to_string(idx) + ".jpg";
            else
                _imageName = "R_" + std::to_string(batchID - i) + "_" + std::to_string(idx) + ".jpg";

            i++;
            idx += 2;

            if(_saveResult)
            {
                QString _imagePath = _folderPath + QString::fromStdString(_imageName);
                cv::imwrite(_imagePath.toStdString(), inputs[j]);
                QString _annotImagePath = _imagePath.replace(".jpg", "_annot.jpg");
                cv::imwrite(_annotImagePath.toStdString(), img);
            }


        }

        // _testFrames.push_back(_lblFrames[0]);
        // _testFrames.push_back(_lblFrames[7]);
    }
}



