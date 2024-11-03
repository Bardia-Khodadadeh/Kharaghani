QT += quick \
    widgets
QT += quickcontrols2
QT += serialport
QT += core network
QT += sql
QT += charts
QT += serialbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++17
#CONFIG += console
#CONFIG -= app_bundle
DEFINES += QT_NO_KEYWORDS
TARGET = QTUDPSender
TARGET = Perticon
CONFIG += console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        camera/cameracontroller.cpp \
        camera/capturethread.cpp \
        camera/captureviewport.cpp \
        camera/ctcamproperty.cpp \
        camera/tcamcamera.cpp \
        communication/abstractcontroller.cpp \
        communication/ejectorboard.cpp \
        communication/motorboard.cpp \
        communication/pulsereader.cpp \
        communication/tcp_server.cpp \
        databasecontroller.cpp \
        ethernetcontroller.cpp \
        main.cpp \
        output.cpp \
        processor/colordetector.cpp \
        processor/detector.cpp \
        processor/detector_torchscript_yolo.cpp \
        processor/processor.cpp \
        shareddata.cpp \
        weight/lineweighthandler.cpp \
        weight/weightcontroller.cpp \

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =


QMAKE_LFLAGS += -Wl,-rpath,"'$$ORIGIN'"

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    camera/cameracontroller.h \
    camera/capturethread.h \
    camera/captureviewport.h \
    camera/ctcamproperty.h \
    camera/tcamcamera.h \
    communication/abstractcontroller.h \
    communication/ejectorboard.h \
    communication/motorboard.h \
    communication/pulsereader.h \
    communication/tcp_server.h \
    databasecontroller.h \
    enums.h \
    ethernetcontroller.h \
    output.h \
    processor/colordetector.h \
    processor/detector.h \
    processor/detector_torchscript_yolo.h \
    processor/processor.h \
    shareddata.h \
    weight/lineweighthandler.h \
    weight/weightcontroller.h \

 INCLUDEPATH  += /usr/local/cuda-11.6/include
 QMAKE_LIBDIR += /usr/local/cuda-11.6/lib64

# ----- opencv path
 unix: CONFIG += link_pkgconfig
 unix: PKGCONFIG += opencv4

INCLUDEPATH += /usr/local/include/opencv4
LIBS += -L/usr/local/lib \
     -lopencv_core\
     -lopencv_imgcodecs \
     -lopencv_imgproc \
     -lopencv_photo \
     -lopencv_videoio \
     -lopencv_video \

CONFIG += no_keywords
INCLUDEPATH += /home/perticon/libtorch/include/torch/csrc/api/include
INCLUDEPATH += /home/perticon/libtorch/include
INCLUDEPATH += /home/perticon/libtorch/lib

DEPENDPATH +=  /home/perticon/libtorch/include/torch/csrc/api/include
DEPENDPATH +=  /home/perticon/libtorch/include
DEPENDPATH +=  /home/perticon/libtorch/lib

INCLUDEPATH += /home/perticon/libxlsxwriter/include


LIBS += -L/home/perticon/libtorch/lib -Wl,--no-as-needed \
    -lc10 \
    -lc10_cuda \
    -lc10d_cuda_test \
    -ltorch \
    -ltorch_cpu \
    -ltorch_cuda
#    -lcaffe2_module_test_dynamic \
#    -lcaffe2_observers \
#    -lcaffe2_detectron_ops_gpu \
#    -lcaffe2_nvrtc \
#    -lfmt \
#    -lshm \
#    -ltensorpipe \
#    -lnvrtc-builtins

LIBS += -L/home/perticon/libxlsxwriter -lxlsxwriter

PKGCONFIG += gstreamer-1.0 gstreamer-app-1.0 gstreamer-video-1.0
PKGCONFIG += tcam
