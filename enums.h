#ifndef ENUMS_H
#define ENUMS_H

#include <QDebug>
#include <QObject>

enum GraderStatus {Grading, Calibrating, Stopped};

enum ConnectionStatus {Connected, Disconnected};

enum CommunicationType {UDP, TCP};

Q_DECLARE_METATYPE(ConnectionStatus);
Q_DECLARE_METATYPE(GraderStatus);

inline QDebug operator<<(QDebug debug, GraderStatus myEnum) {
    switch (myEnum) {
    case GraderStatus::Stopped:
        debug << "Stopped";
        break;
    case GraderStatus::Calibrating:
        debug << "Calibrating";
        break;
    case GraderStatus::Grading:
        debug << "Grading";
        break;
    default:
        debug << "Unknown";
        break;
    }
    return debug;
}

inline QDebug operator<<(QDebug debug, ConnectionStatus myEnum) {
    switch (myEnum) {
    case ConnectionStatus::Connected:
        debug << "Connected";
        break;
    case ConnectionStatus::Disconnected:
        debug << "Disconnected";
        break;
    default:
        debug << "Unknown";
        break;
    }
    return debug;
}

#endif // ENUMS_H
