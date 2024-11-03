#ifndef CAPTUREVIEWPORT_H
#define CAPTUREVIEWPORT_H

#include <QQuickPaintedItem>

class CameraController;
class CaptureThread;
extern QString home_dir;

class CaptureViewPort : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(int id_viewPort READ id_viewPort WRITE setId_viewPort NOTIFY id_viewPortChanged)
    Q_PROPERTY(QSize inputResulation READ inputResulation WRITE setInputResulation NOTIFY inputResulationChanged)

public:
    explicit CaptureViewPort(QQuickItem *parent = nullptr);
    void paint(QPainter *painter) override;
    void setCaptureThread(CaptureThread *newCaptureThread);

    int id() const;
    void setId(int newId);

    int id_viewPort() const;
    void setId_viewPort(int newId_viewPort);

    const QSize &inputResulation() const;
    void setInputResulation(const QSize &newInputResulation);

private:
    int m_id;
    CaptureThread *m_captureThread;
    int m_id_viewPort;
    QSize m_inputResulation = QSize(960, 960);


Q_SIGNALS:
    void idChanged();
    void id_viewPortChanged();
    void inputResulationChanged();

};

#endif // CAPTUREVIEWPORT_H
