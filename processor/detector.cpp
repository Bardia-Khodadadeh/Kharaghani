#include "detector.h"

Detector::Detector(QObject *parent) : QObject{parent} {}

void Detector::loadModel(const QString modelPath)
{
    Q_UNUSED(modelPath);
}

void Detector::setOutputSize(const cv::Size &newOutputSize)
{
    Q_UNUSED(newOutputSize);
}

std::vector<std::vector<Detection> > Detector::run(std::vector<cv::Mat> &images, int cup)
{
    Q_UNUSED(images);
    return{};
}
