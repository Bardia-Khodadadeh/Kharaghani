#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include "camera/cameracontroller.h"
#include <QtNetwork>
#include <QIcon>
#include <databasecontroller.h>
#include "weight/weightcontroller.h"
#include <ethernetcontroller.h>
#include "camera/captureviewport.h"
#include <opencv2/opencv.hpp>


int main(int argc, char *argv[])
{



#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    //    app.setWindowIcon(QIcon(":/Icons/outline_settings_black_48pt_3x.png"));
    app.setWindowIcon(QIcon(":/Icons/Logo1.png"));

    QQuickStyle::setStyle("material");
    qmlRegisterType<WeightController>("app.WeightController", 1, 0, "WeightController");
    qmlRegisterType<EthernetController>("app.EthernetController", 1, 0, "EthernetController");
    qmlRegisterType<DatabaseController>("app.DatabaseController", 1, 0, "DatabaseController");
    qmlRegisterType<CameraController>("app.CameraController", 1, 0, "CameraController");
    qmlRegisterType<CaptureViewPort>("app.CaptureViewPort", 1, 0, "CaptureViewPort");

    gst_init(&argc, &argv);
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
    return app.exec();
}
