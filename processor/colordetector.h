#ifndef COLORDETECTOR_H
#define COLORDETECTOR_H

#include <opencv2/opencv.hpp>
#include <QtConcurrent/QtConcurrent>
#include "shareddata.h"

struct fruit
{
    std::vector<std::vector<cv::Point>> contours;
    int largest_contour_index=0;

    cv::Mat binary_red;
    cv::Mat binary_orange;
    cv::Mat binary_yellow;
    cv::Mat binary_green;
    cv::Mat binary_blue;

    cv::Mat binary_fruit;
    cv::Mat binary_thresh;

    cv::Mat hsv_frame;
    cv::Mat rgb_fruit;
    cv::Mat hsv_fruit;
    cv::Mat rgb_final;

    cv::Mat red = cv::imread("/home/perticon/Projects/Perticon_8_8_new_serial_2/colors/red_color.jpg");
    cv::Mat orange = cv::imread("/home/perticon/Projects/Perticon_8_8_new_serial_2/colors/orange_color.jpg");
    cv::Mat yellow = cv::imread("/home/perticon/Projects/Perticon_8_8_new_serial_2/colors/yellow_color.jpg");
    cv::Mat green = cv::imread("/home/perticon/Projects/Perticon_8_8_new_serial_2/colors/green_color.jpg");
    cv::Mat blue = cv::imread("/home/perticon/Projects/Perticon_8_8_new_serial_2/colors/blue_color.jpg");
};

class ColorDetector : public SharedData
{
private:

    int _colorcup = 0;


public:
    ColorDetector();
    std::vector<int> getColorPercents(cv::Mat &rgb_frame);
    std::vector<std::vector<int>> _sizes;
    std::vector<std::vector<int>> _densities;

    QVector<QString> _cropHsv;
    QVector<QString> _rdHsv;
    QVector<QString> _orHsv;
    QVector<QString> _ylHsv;
    QVector<QString> _gnHsv;

    int _frameIdx = 1;

    cv::Mat rgb_final;

    std::vector<cv::Mat> _coloredFrames;
    std::vector<cv::Mat> _coloredFrames2;

    QVector<int> _colorsPer;

    QVector<int> _colorsPers1;
    QVector<int> _colorsPers2;

    std::vector<cv::Mat> _orgFrames1;
    std::vector<cv::Mat> _orgFrames2;



    void getColorPercents(std::vector<cv::Mat> &frames, int _cupNumber, int _lineID);

    cv::Mat &getColorBinary(fruit &_fruit, QString color);
    QVector<QString> &getCorrespondingVec(QString text);
    void floodfill(cv::Mat &rgb_frame, fruit &_fruit);
public Q_SLOTS:
    void saveImmgs(std::vector<cv::Mat> frames);
};

#endif // COLORDETECTOR_H
