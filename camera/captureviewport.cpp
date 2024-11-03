#include "captureviewport.h"
#include <QPainter>
#include "cameracontroller.h"
#include "capturethread.h"
#include <QDebug>


CaptureViewPort::CaptureViewPort(QQuickItem *parent) : QQuickPaintedItem(parent) {}

void CaptureViewPort::paint(QPainter *painter)
{
    if (Q_UNLIKELY(!m_captureThread)) {
        if(m_id_viewPort == 1){
            painter->fillRect(clipRect(), Qt::white);
        }
        if(m_id_viewPort == 2){

        }

        return;
    }
    try {
        m_captureThread->paintFrame(painter, this);
    } catch (...) {

    }
}

void CaptureViewPort::setCaptureThread(CaptureThread *newCaptureThread)
{
    qDebug() << "---" << Q_FUNC_INFO;

    connect(newCaptureThread, &CaptureThread::updateRequired, this, &QQuickItem::update);
    m_captureThread = newCaptureThread;
}

int CaptureViewPort::id_viewPort() const
{
    return m_id_viewPort;
}
void CaptureViewPort::setId_viewPort(int newId_viewPort)
{
    if (m_id_viewPort == newId_viewPort)
        return;

    m_id_viewPort = newId_viewPort;
    Q_EMIT id_viewPortChanged();
}

const QSize &CaptureViewPort::inputResulation() const
{
    return m_inputResulation;
}
void CaptureViewPort::setInputResulation(const QSize &newInputResulation)
{
    if (m_inputResulation == newInputResulation)
        return;

    m_inputResulation = newInputResulation;
    Q_EMIT inputResulationChanged();
}

int CaptureViewPort::id() const
{
    return m_id;
}
void CaptureViewPort::setId(int newId)
{
    if (m_id == newId)
        return;

    m_id = newId;
    Q_EMIT idChanged();
}
