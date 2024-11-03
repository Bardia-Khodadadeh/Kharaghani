#include "ethernet.h"

#include <QDebug>

#include <QCoreApplication>
#include <QtNetwork>
#include <iostream>
#include <iomanip>
#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include <QProcess>

Ethernet::Ethernet(QObject *parent) : QObject{parent}{}

void Ethernet::set_Camera_Ethernet_Setting()
{
    qDebug() << "--- must be deleted" << Q_FUNC_INFO;
    qFatal("Crashed");

//    QProcess process;

//    QString NetMask = "255.255.255.0";


//    // Ethernet card esp6
//    QString esp6s0IP1 = "192.168.6.200";QString esp6s0IP2 = "192.168.6.201"; QString esp6s0GW = "192.168.6.10";

//    process.start("tcam-gigetool", QStringList() << "rescue" << "--ip" << esp6s0IP1 << "--netmask" << NetMask << "--gateway" << esp6s0GW << "45224160");
//    process.waitForFinished();

//    process.start("tcam-gigetool", QStringList() << "rescue" << "--ip" << esp6s0IP2 << "--netmask" << NetMask << "--gateway" << esp6s0GW << "45224154");
//    process.waitForFinished();



    // Ethernet card esp8
//    QString esp8s0IP = "192.168.8.200"; QString esp8s0GW = "192.168.8.10";
//    process.start("tcam-gigetool", QStringList() << "rescue" << "--ip" << esp8s0IP << "--netmask" << NetMask << "--gateway" << esp8s0GW << "45224154");
//    process.waitForFinished();

}
