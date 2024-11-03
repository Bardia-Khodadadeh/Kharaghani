#ifndef CAPTURETHREAD_H
#define CAPTURETHREAD_H

#include <QThread>
#include <opencv2/opencv.hpp>
#include <QMutex>
#include <QObject>
#include "captureviewport.h"
#include <QtConcurrent/QtConcurrent>
#include <QSize>
#include <QImage>
#include <QPainter>
#include <gst/gst.h>
#include <shareddata.h>

class CameraController;
class CaptureThread : public QThread
{
    Q_OBJECT
public:
    bool _newFrameFound = false;
    GstElement* _source;
    explicit CaptureThread(QObject *parent = nullptr);
    void setCapture(QString pipeline_description_str, float gain, float exposure, float red,float green, float blue, float gamma);
    void releaseCapture();
    static GstPadProbeReturn handle_new_sample(GstPad *pad, GstPadProbeInfo *info, gpointer user_data);
    void setCaptureID(int id);

    cv::Mat retrieveFrame(int cup = 0);
    cv::Mat _orgFrameMat[2];
    cv::Mat _colorFrameMat[2];
    cv::Mat _defectFrameMat[2];
    gint width, height;
    void paintFrame(QPainter *painter, CaptureViewPort *viewport);
    void paintFrame(QPainter *painter, const cv::Mat &mat, const QSize &size);

    void setNewSetting(GstElement *_source, QString setting, int idx);
    void setCapture(QString pipeline_description_str, QVector<QString> &camSetting);

protected:
    void run();

private:
    GstPad*  _src_pad;
    GstElement* _pipeline;
    gulong _probe_id;

    QAtomicInt _locker;
    QMutex _frameLocker;

    //    gint width, height;
    int _captureID = -1;
    cv::Mat _frameMat;

    QVector<QString> _cameraParams = {"ExposureTime", "Gain", "BlackLevel", "BalanceWhiteRed", "BalanceWhiteGreen",
                                      "BalanceWhiteBlue", "Gamma", "BalanceWhiteAuto", "BalanceWhiteMode",
                                      "BalanceWhiteAutoPreset", "BalanceWhiteTemperaturePreset", "StrobeEnable"};
public Q_SLOTS:
    void grabFrame();


Q_SIGNALS:
    void grabTheFrame();
    void updateRequired();
};

#endif // CAPTURETHREAD_H
