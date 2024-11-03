#ifndef DETECTOR_H
#define DETECTOR_H

#include <QObject>
#include <QUrl>
#include <opencv2/opencv.hpp>



struct Detection
{
    cv::Rect bbox;
    float    confidence;
    int      classID;
    QString  className;
    int      framePosition;
};
class Detector : public QObject
{
    Q_OBJECT
public:
    explicit Detector(QObject *parent = nullptr);
    bool modelIsLoaded = false;
    virtual void loadModel(const QString modelPath);
    virtual void setOutputSize(const cv::Size &newOutputSize);
    virtual std::vector<std::vector<Detection>> run(std::vector<cv::Mat> &images, int cup);
    cv::Size outputSize;
};

#endif // DETECTOR_H
