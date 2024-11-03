#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <QObject>
#include <opencv2/opencv.hpp>
#include <QtConcurrent/QtConcurrent>
#include "detector.h"
#include <QFuture>
#include "colordetector.h"
//#include <cameracontroller.h>

class Processor : public QObject, public SharedData
{
    Q_OBJECT
public:
    explicit Processor(QObject *parent = nullptr);
    void setModelPath(const QString &newModelPath);
    void setClassesList(const QVector<QString> &newClassesList);
    QVector<QString> classesList;
    void setOutputSize(const cv::Size &newOutputSize);
    cv::Size _outputSize;
    std::vector<std::vector<Detection>> process(const std::vector<cv::Mat> &inputs, std::vector<cv::Mat> &outputs, int batchID, int camSize);
    bool _saveResult = false;
    QString _folderPath = "";
    void setterMySetup(bool multiFrame, int numOfLines);

    void getColorPercents(std::vector<cv::Mat> &frames, int cupNumber, int lineID);

    QVector<QString> _cropHsv;
    QVector<QString> _rdHsv;
    QVector<QString> _orHsv;
    QVector<QString> _ylHsv;
    QVector<QString> _gnHsv;

    std::vector<cv::Mat> _orgFrames1;
    std::vector<cv::Mat> _orgFrames2;

    int _frameIdx;

    bool _saveImg = false;

    std::vector<cv::Mat> _labeledFrame;

    std::vector<cv::Mat> _coloredFrames;
    std::vector<cv::Mat> _coloredFrames2;
    std::vector<cv::Mat> _testFrames;
    std::vector<cv::Mat> _lblFrames;


    QVector<int> _colorsPer;

    QVector<int> _colorsPers1;
    QVector<int> _colorsPers2;


    QVector<bool> cameraOpened;

    ColorDetector *_colorDetector = new ColorDetector();

private:
    Detector *_detector = nullptr;
    std::vector<std::vector<int>> colorOfClasses = {{255,255,255},{255,0,0},{0,255,0},{255,255,0},{0,0,255},{255,0,255},{0,255,255},{255,128,0},{130,130,130},{128,0,255},{255,0,128},{0,0,0}};
    void saveOutputs(std::vector<cv::Mat> &inputs, std::vector<std::vector<Detection> > result, int batchID, int camSize);
    void drawBoxes(std::vector<cv::Mat> &inputs, std::vector<std::vector<Detection>> _detections);
    bool m_multiFrame;
    int m_numOfLines;

    int i = 0;
    int idx = 1;

};

#endif // PROCESSOR_H
