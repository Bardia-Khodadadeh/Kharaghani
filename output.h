#ifndef OUTPUT_H
#define OUTPUT_H

#include <qvector.h>

struct image
{
    QVector<bool> detectedObjects;
    QVector<int>  numberOfDefects;
    QVector<int>  colorPercents;
    int           size = 0;
    int           density = 0;
};

struct cup
{
    QVector<image> Images;
    double weight = 0;
    int pixCount = 0;
};

struct line
{
    QVector<cup> cups;
};

struct outputSetup//TODO
{
    const int  numberOfColors = 5;
    const int _totalCupNumber = 283;
    const int _numberOfLines = 2;
    const int _numberOfClasses = 9;
    const int _batchSize = 14;

};


class Output
{

public:
    Output();
    std::vector<line> lines;

private:
    outputSetup _outputSetup;
};

#endif // OUTPUT_H
