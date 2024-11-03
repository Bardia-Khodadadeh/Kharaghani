#ifndef SHAREDDATA_H
#define SHAREDDATA_H

#include "databasecontroller.h"
#include "output.h"
#include "enums.h"

class SharedData
{

private:

public:
    SharedData();
    ~SharedData() = default;

    static DatabaseController *_db;

    static const int _totalCupNumber = 283;
    static const int _numberOfLines = 2;
    static const int _numberOfClasses = 9;
    static const int _numberOfColors = 5;
    static const int _batchSize = 14;

    static int _encoder;
    static int _encode;

    static Output *_output;
    static GraderStatus m_graderStatus;
    static CommunicationType communicationType;



};

#endif // SHAREDDATA_H
