#include "capturethread.h"
#include "qdebug.h"
#include "cameracontroller.h"

CaptureThread::CaptureThread(QObject *parent) : QThread{parent} {
    //connect(this,SIGNAL(encoderChanged()), this, SLOT(mainLoop()));

}

void CaptureThread::paintFrame(QPainter *painter, CaptureViewPort *viewport)
{
    //qDebug() << "---" << Q_FUNC_INFO << "--> ViewPort ID:" << viewport->id_viewPort();

    _locker.ref();
    viewport->setInputResulation({360, 360});

    if(viewport->id_viewPort() == 1 || viewport->id_viewPort() == 3 || viewport->id_viewPort() == 7)
        paintFrame(painter, _orgFrameMat[0], viewport->size().toSize());
    else if(viewport->id_viewPort() == 2 || viewport->id_viewPort() == 5 || viewport->id_viewPort() == 9)
        paintFrame(painter, _orgFrameMat[1], viewport->size().toSize());

    else if(viewport->id_viewPort() == 4)
        paintFrame(painter, _colorFrameMat[0], viewport->size().toSize());
    else if(viewport->id_viewPort() == 6)
        paintFrame(painter, _colorFrameMat[1], viewport->size().toSize());

    else if(viewport->id_viewPort() == 8)
        paintFrame(painter, _defectFrameMat[0], viewport->size().toSize());
    else if(viewport->id_viewPort() == 10)
        paintFrame(painter, _defectFrameMat[1], viewport->size().toSize());

    _locker.deref();
}

void CaptureThread::paintFrame(QPainter *painter, const cv::Mat &mat, const QSize &size)
{
    if (mat.empty()) {
        // set black screen if frame is empty
        painter->fillRect(QRect{0, 0, size.width(), size.height()}, Qt::black);
        return;
    }

    auto _frameImage = QImage((uchar *)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_BGR888);
    painter->drawImage({0, 0, size.width(), size.height()}, _frameImage, _frameImage.rect());
}

void CaptureThread::setCapture(QString pipeline_description_str, QVector<QString> &camSetting)
{
    qDebug() << "---" << Q_FUNC_INFO << pipeline_description_str;

    GError* err = NULL;
    QByteArray pipeline_description_byteArray = pipeline_description_str.toLocal8Bit();
    const char *pipeline_description_constChar = pipeline_description_byteArray.data();
    _pipeline = gst_parse_launch(pipeline_description_constChar, &err);
    if (_pipeline == NULL)
    {
        printf("Could not create pipeline. Cause: %s\n", err->message);
        return;
    }
    _source = gst_bin_get_by_name(GST_BIN(_pipeline), "source");

    gst_element_set_state(_pipeline, GST_STATE_READY);

    tcam_property_provider_set_tcam_enumeration(TCAM_PROPERTY_PROVIDER(_source), "ExposureAuto", "Off", &err);
    tcam_property_provider_set_tcam_enumeration(TCAM_PROPERTY_PROVIDER(_source), "GainAuto", "Off", &err);

    for (int i = 3; i < camSetting.size();i++) {
        bool isDouble = false;
        QString setting = camSetting[i];
        setting.toDouble(&isDouble);
        if(isDouble){
            tcam_property_provider_set_tcam_float(TCAM_PROPERTY_PROVIDER(_source), _cameraParams[i-3].toUtf8().constData(),setting.toFloat(), &err);
        }
        else{
            tcam_property_provider_set_tcam_enumeration(TCAM_PROPERTY_PROVIDER(_source), _cameraParams[i-3].toUtf8().constData(), setting.toUtf8().constData(), &err);
        }
    }

    tcam_property_provider_set_tcam_boolean(TCAM_PROPERTY_PROVIDER(_source),"ExposureAutoHighlightReduction",false,&err);
    tcam_property_provider_set_tcam_enumeration(TCAM_PROPERTY_PROVIDER(_source), "StrobeOperation", "Exposure", &err);
    tcam_property_provider_set_tcam_enumeration(TCAM_PROPERTY_PROVIDER(_source), "StrobePolarity", "ActiveLow", &err);
    tcam_property_provider_set_tcam_enumeration(TCAM_PROPERTY_PROVIDER(_source), "TriggerMode", "On", &err);

    if (err)
    {
        qDebug() << "Error in setting camera params" << err->message;
        qDebug() << "================================================================";

        g_error_free(err);
        err = NULL;
    }

    _src_pad = gst_element_get_static_pad(_source, "src");

    _probe_id = gst_pad_add_probe(_src_pad, GST_PAD_PROBE_TYPE_BUFFER, (GstPadProbeCallback)handle_new_sample, this, NULL);

    gst_element_set_state(_pipeline, GST_STATE_PLAYING);
}

void CaptureThread::setCapture(QString pipeline_description_str, float gain, float exposure, float red,float green, float blue, float gamma)
{
    qDebug() << "---" << Q_FUNC_INFO << pipeline_description_str;

    GError* err = NULL;
    QByteArray pipeline_description_byteArray = pipeline_description_str.toLocal8Bit();
    const char *pipeline_description_constChar = pipeline_description_byteArray.data();
    _pipeline = gst_parse_launch(pipeline_description_constChar, &err);
    if (_pipeline == NULL)
    {
        printf("Could not create pipeline. Cause: %s\n", err->message);
        return;
    }
    _source = gst_bin_get_by_name(GST_BIN(_pipeline), "source");

    gst_element_set_state(_pipeline, GST_STATE_READY);

    tcam_property_provider_set_tcam_enumeration(TCAM_PROPERTY_PROVIDER(_source), "TriggerMode", "On", &err);
    //    tcam_property_provider_set_tcam_enumeration(TCAM_PROPERTY_PROVIDER(_source), "TriggerMode", "Off", &err);
    tcam_property_provider_set_tcam_enumeration(TCAM_PROPERTY_PROVIDER(_source), "GainAuto", "Off", &err);
    tcam_property_provider_set_tcam_float(TCAM_PROPERTY_PROVIDER(_source), "Gain", gain, &err);
    tcam_property_provider_set_tcam_boolean(TCAM_PROPERTY_PROVIDER(_source),"ExposureAutoHighlightReduction",false,&err);
    tcam_property_provider_set_tcam_enumeration(TCAM_PROPERTY_PROVIDER(_source), "ExposureAuto", "Off", &err);
    tcam_property_provider_set_tcam_float(TCAM_PROPERTY_PROVIDER(_source), "ExposureTime", exposure, &err);
    tcam_property_provider_set_tcam_float(TCAM_PROPERTY_PROVIDER(_source), "BlackLevel", 0.00, &err);
    tcam_property_provider_set_tcam_float(TCAM_PROPERTY_PROVIDER(_source), "Saturation", 100.00, &err);
    tcam_property_provider_set_tcam_float(TCAM_PROPERTY_PROVIDER(_source), "Hue", 0.0, &err);
    tcam_property_provider_set_tcam_enumeration(TCAM_PROPERTY_PROVIDER(_source), "BalanceWhiteAuto", "Off", &err);
    tcam_property_provider_set_tcam_float(TCAM_PROPERTY_PROVIDER(_source), "BalanceWhiteRed", red, &err);
    tcam_property_provider_set_tcam_float(TCAM_PROPERTY_PROVIDER(_source), "BalanceWhiteGreen", green, &err);
    tcam_property_provider_set_tcam_float(TCAM_PROPERTY_PROVIDER(_source), "BalanceWhiteBlue", blue, &err);
    tcam_property_provider_set_tcam_float(TCAM_PROPERTY_PROVIDER(_source), "Gamma", gamma, &err);
    tcam_property_provider_set_tcam_enumeration(TCAM_PROPERTY_PROVIDER(_source), "GainAuto", "Off", &err);
    tcam_property_provider_set_tcam_enumeration(TCAM_PROPERTY_PROVIDER(_source), "StrobeOperation", "Exposure", &err);
    tcam_property_provider_set_tcam_enumeration(TCAM_PROPERTY_PROVIDER(_source), "StrobePolarity", "ActiveLow", &err);
    tcam_property_provider_set_tcam_enumeration(TCAM_PROPERTY_PROVIDER(_source), "StrobeEnable", "On", &err);


    if (err)
    {
        qDebug() << "Error in setting camera params" << err->message;
        qDebug() << "================================================================";

        g_error_free(err);
        err = NULL;
    }

    _src_pad = gst_element_get_static_pad(_source, "src");
    _probe_id = gst_pad_add_probe(_src_pad, GST_PAD_PROBE_TYPE_BUFFER, (GstPadProbeCallback)handle_new_sample, this, NULL);

    gst_element_set_state(_pipeline, GST_STATE_PLAYING);
}

void CaptureThread::releaseCapture()
{
    qDebug() << "---" << Q_FUNC_INFO;

    GError* err = NULL;
    tcam_property_provider_set_tcam_enumeration(TCAM_PROPERTY_PROVIDER(_source), "TriggerMode", "Off", &err);
    gst_element_set_state(_pipeline, GST_STATE_NULL);
    gst_object_unref(_source);
    gst_pad_remove_probe(_src_pad, _probe_id);
    gst_object_unref(_pipeline);
}

GstPadProbeReturn CaptureThread::handle_new_sample(GstPad *pad, GstPadProbeInfo *info, gpointer user_data)
{
    CaptureThread *_parent = static_cast<CaptureThread*>(user_data);
    //_parent->setPriority(QThread::TimeCriticalPriority);
    GstBuffer* buffer = GST_PAD_PROBE_INFO_BUFFER(info);

    if (buffer != NULL)
    {
        GstMapInfo map_info;
        gst_buffer_map(buffer, &map_info, GST_MAP_READ);
        cv::Mat image(_parent->height, _parent->width, CV_8UC4, map_info.data);
        cv::cvtColor(image, image, cv::COLOR_BGRA2BGR);

        _parent->_frameMat = image.clone();
        _parent->_newFrameFound = true;

        image.release();

        gst_buffer_unmap(buffer, &map_info);
        //        gst_buffer_unref(buffer);
        gst_buffer_remove_all_memory(buffer);
    }

    return GST_PAD_PROBE_OK;
}

void CaptureThread::grabFrame()
{
    start();
}

void CaptureThread::run()
{
    GError* err = NULL;
    tcam_property_provider_set_tcam_command(TCAM_PROPERTY_PROVIDER(_source), "TriggerSoftware", &err);
    //qDebug() << "-------" << "trigger";
}

cv::Mat CaptureThread::retrieveFrame(int cup)
{
    if(_newFrameFound){
        _newFrameFound = false;
        return _frameMat;
    }
    qDebug() << "Black" << cup;
    return cv::Mat::zeros(height, width, CV_8UC3);
}

void CaptureThread::setCaptureID(int id)
{
    qDebug() << "---" << Q_FUNC_INFO << id;

    _captureID = id;
}

void CaptureThread::setNewSetting(GstElement *_source, QString setting, int idx)
{
    GError* err = NULL;
    bool isDouble = false;
    setting.toDouble(&isDouble);
    if(isDouble){
        tcam_property_provider_set_tcam_float(TCAM_PROPERTY_PROVIDER(_source), _cameraParams[idx-3].toUtf8().constData(),setting.toFloat(), &err);
        // qDebug() << _cameraParams[idx-3] << setting;
    }
    else{
        tcam_property_provider_set_tcam_enumeration(TCAM_PROPERTY_PROVIDER(_source), _cameraParams[idx-3].toUtf8().constData(), setting.toUtf8().constData(), &err);
    }
    _newFrameFound = true;
}
