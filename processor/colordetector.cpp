#include "colordetector.h"
#include <opencv4/opencv2/core/mat.hpp>

ColorDetector::ColorDetector()
{

}

cv::Mat &ColorDetector::getColorBinary(fruit &_fruit, QString color)
{
    if(color == "Red") return _fruit.binary_red;
    else if(color == "Orange") return _fruit.binary_orange;
    else if(color == "Yellow") return _fruit.binary_yellow;
    else if(color == "Green") return _fruit.binary_green;
}

QVector<QString> &ColorDetector::getCorrespondingVec(QString text)
{
    if(text == "Crop") return _cropHsv;
    else if(text == "Red") return _rdHsv;
    else if(text == "Orange") return _orHsv;
    else if(text == "Yellow") return _ylHsv;
    else if(text == "Green") return _gnHsv;
}

void ColorDetector::floodfill(cv::Mat &rgb_frame, fruit &_fruit)
{
    _fruit.rgb_fruit.release();
    int width = rgb_frame.rows;
    int height = rgb_frame.cols;

    cv::Mat mask;
    cv::Rect d = cv::Rect(180, 0, width, height);
    mask = cv::Mat::zeros(width + 2, height + 2, CV_8UC1);

    int largest_area=0;
    _fruit.largest_contour_index=0;
    cv::Rect bounding_rect;

    std::vector<cv::Vec4i> hierarchy;
    findContours(_fruit.binary_thresh, _fruit.contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);


    for(size_t i = 0; i< _fruit.contours.size(); i++) // iterate through each contour.
    {
        double area = contourArea( _fruit.contours[i] );  //  Find the area of contour
        if( area > largest_area )
        {
            largest_area = area;
            _fruit.largest_contour_index = i;
        }
    }


    //if(_lineID == 1)    cv::imwrite("b" + std::to_string(1) + ".jpg", _fruit.rgb_fruit);


    auto cnt = _fruit.contours[_fruit.largest_contour_index];
    cv::Mat black;
    bitwise_not(_fruit.binary_thresh, _fruit.binary_thresh);

    bitwise_xor(_fruit.binary_thresh,_fruit.binary_thresh,_fruit.binary_fruit);

    //Mat copy = rgb_frame.clone();
    drawContours(_fruit.binary_fruit, _fruit.contours,_fruit.largest_contour_index, cv::Scalar( 255, 255, 255), 3);

    auto M = moments(cnt , false);
    auto cx = M.m10/M.m00;
    auto cy = M.m01/M.m00;

    floodFill(_fruit.binary_fruit, mask, cv::Point(cx, cy), cv::Scalar(255), &d);


    int erosion_size = 12; // Adjust size as needed
    cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE,
                                                cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
                                                cv::Point(erosion_size, erosion_size));

    // Step 3: Apply the erosion operation
    cv::Mat eroded_image;
    cv::erode(_fruit.binary_fruit, _fruit.binary_fruit, element);

    bitwise_or(rgb_frame, rgb_frame, _fruit.rgb_fruit, _fruit.binary_fruit);



    //cv::imwrite("erode.jpg",_fruit.binary_fruit );
    // cv::Mat binary_fill = _fruit.binary_thresh.clone();

    // cv::Mat mask = cv::Mat::zeros(rgb_frame.rows + 2, rgb_frame.cols + 2, CV_8UC1); // Adjust the mask size
    // cv::Rect d;

    // floodFill(binary_fill, mask, cv::Point(180, 180), cv::Scalar(0), &d);

    // cv::Mat binary_fill_not;
    // cv::bitwise_not(binary_fill, binary_fill_not);
    // cv::bitwise_and(binary_fill_not, _fruit.binary_thresh, _fruit.binary_fruit);

    // bitwise_or(rgb_frame, rgb_frame, _fruit.rgb_fruit, _fruit.binary_fruit);
}
void ColorDetector::getColorPercents(std::vector<cv::Mat> &frames, int _cupNumber, int _lineID)
{
    QVector<QVector<int>> _colorPercents;
    QVector<int> _sizes;
    QVector<int> _densities;
    cv::Mat rgb_final;
    std::vector<cv::Mat> _coloredFruits;

    auto start = std::chrono::high_resolution_clock::now();

    int l = 0;
    for(auto &rgb_frame:frames)
    {

        fruit _fruit;
        QVector<int> _percents;
        cv::cvtColor(rgb_frame, _fruit.hsv_frame, cv::COLOR_BGR2HSV);

        if(_cropHsv[2] == "true")
        {
            cv::Mat bt1,bt2;

            cv::inRange(_fruit.hsv_frame, cv::Scalar(0,_cropHsv[3].toInt(),_cropHsv[5].toInt()), cv::Scalar(_cropHsv[0].toInt(),_cropHsv[4].toInt(),_cropHsv[6].toInt()), bt1);
            cv::inRange(_fruit.hsv_frame, cv::Scalar(_cropHsv[1].toInt(),_cropHsv[3].toInt(),_cropHsv[5].toInt()), cv::Scalar(179,_cropHsv[4].toInt(),_cropHsv[6].toInt()), bt2);

            cv::bitwise_or(bt1, bt2, _fruit.binary_thresh);
        }
        else
        {
            cv::Scalar lowerBounds(_cropHsv[0].toInt(), _cropHsv[3].toInt(), _cropHsv[5].toInt());
            cv::Scalar upperBounds(_cropHsv[1].toInt(), _cropHsv[4].toInt(), _cropHsv[6].toInt());

            cv::inRange(_fruit.hsv_frame, lowerBounds, upperBounds, _fruit.binary_thresh);
        }



        if(cv::countNonZero(_fruit.binary_thresh) > 20000)
        {
            int _cupPixel = ((_cupNumber) % SharedData::_totalCupNumber + SharedData::_totalCupNumber) % (SharedData::_totalCupNumber);

            _output->lines[_lineID].cups[_cupPixel].pixCount = cv::countNonZero(_fruit.binary_thresh);


            _sizes.push_back(cv::countNonZero(_fruit.binary_thresh));
            _densities.push_back(cv::countNonZero(_fruit.binary_thresh));

            floodfill(rgb_frame, _fruit);

            cvtColor(_fruit.rgb_fruit, _fruit.hsv_fruit, cv::COLOR_BGR2HSV);

            QVector<QString> colors = {"Red", "Orange", "Yellow", "Green"};

            cv::Mat bt1, bt2;

            for(const auto &color:colors)
            {
                QVector<QString> &vec = getCorrespondingVec(color);
                cv::Mat &binary = getColorBinary(_fruit, color);

                if(vec[2] == "true")
                {
                    cv::Mat bt1,bt2;

                    cv::inRange(_fruit.hsv_fruit, cv::Scalar(0,vec[3].toInt(),vec[5].toInt()), cv::Scalar(vec[0].toInt(),vec[4].toInt(),vec[6].toInt()), bt1);
                    cv::inRange(_fruit.hsv_fruit, cv::Scalar(vec[1].toInt(),vec[3].toInt(),vec[5].toInt()), cv::Scalar(179,vec[4].toInt(),vec[6].toInt()), bt2);
                    cv::bitwise_or(bt1, bt2, binary);
                }
                else
                {
                    cv::inRange(_fruit.hsv_fruit, cv::Scalar(vec[0].toInt(),vec[3].toInt(),vec[5].toInt()), cv::Scalar(vec[1].toInt(),vec[4].toInt(),vec[6].toInt()), binary);
                }
            }

            //cv::imwrite("binary.jpg", _fruit.rgb_fruit);


            cv::Mat binary_not;
            cv::bitwise_not(_fruit.binary_red, binary_not);
            cv::bitwise_and(binary_not, _fruit.binary_orange, _fruit.binary_orange);
            cv::bitwise_and(binary_not, _fruit.binary_yellow, _fruit.binary_yellow);

            cv::Mat binary_not1;
            cv::bitwise_not(_fruit.binary_orange, binary_not1);
            cv::bitwise_and(binary_not1, _fruit.binary_yellow, _fruit.binary_yellow);

            cv::Mat binary_not2;
            cv::bitwise_not(_fruit.binary_green, binary_not2);
            cv::bitwise_and(binary_not2, _fruit.binary_yellow, _fruit.binary_yellow);

            cv::bitwise_or(_fruit.blue, _fruit.blue, _fruit.rgb_final, _fruit.binary_fruit);
            cv::bitwise_or(_fruit.red, _fruit.red, _fruit.rgb_final, _fruit.binary_red);
            cv::bitwise_or(_fruit.orange, _fruit.orange, _fruit.rgb_final, _fruit.binary_orange);
            cv::bitwise_or(_fruit.yellow, _fruit.yellow, _fruit.rgb_final, _fruit.binary_yellow);
            cv::bitwise_or(_fruit.green, _fruit.green, _fruit.rgb_final, _fruit.binary_green);

            _coloredFruits.push_back(_fruit.rgb_final);

            int count_red = cv::countNonZero(_fruit.binary_red);
            int count_orange = cv::countNonZero(_fruit.binary_orange);
            int count_yellow = cv::countNonZero(_fruit.binary_yellow);
            int count_green = cv::countNonZero(_fruit.binary_green);
            int total = cv::countNonZero(_fruit.binary_fruit);

            if(total != 0)
            {
                _percents.push_back(count_red*100/total);
                _percents.push_back(count_orange*100/total);
                _percents.push_back(count_yellow*100/total);
                _percents.push_back(count_green*100/total);
                _percents.push_back((total - (count_red + count_orange + count_yellow + count_green))*100/total);
            }
        }
        else
        {
            _coloredFruits.push_back(cv::Mat::zeros(360, 360, CV_8UC3));
            _sizes.push_front(0);
            _densities.push_back(0);
        }

        _percents.empty() ? _colorPercents.push_back({0, 0, 0, 0, 0}) : _colorPercents.push_back(_percents);
    }

    if(!_coloredFruits.empty())
    {
        _lineID == 0 ? _coloredFrames = _coloredFruits : _coloredFrames2 = _coloredFruits;
    }
    else
    {
        std::vector<cv::Mat> blackFrames;
        for(int i=0;i<7;i++)
        {
            blackFrames.push_back(cv::Mat::zeros(360, 360, CV_8UC3));
        }
        _lineID == 0 ? _coloredFrames = blackFrames : _coloredFrames2 = blackFrames;
    }

    if(m_graderStatus == GraderStatus::Stopped || m_graderStatus == GraderStatus::Calibrating)
        if(_frameIdx > 0)
            for(auto &color:_colorPercents[_frameIdx - 1])
            {
                _colorsPer.push_back(color);
            }

    // qDebug() << _colorsPer;

    // if(_lineID == 0)
    // {
    //     _orgFrames1.push_back(frames[0]);
    // }
    // else
    // {
    //     _orgFrames2.push_back(frames[0]);
    // }

    // if(_lineID == 0)
    // {
    //     _colorsPers1.push_back(_colorPercents[_frameIdx - 1][0]);
    //     _colorsPers1.push_back(_colorPercents[_frameIdx - 1][1]);
    //     _colorsPers1.push_back(_colorPercents[_frameIdx - 1][2]);
    //     _colorsPers1.push_back(_colorPercents[_frameIdx - 1][3]);
    //     _colorsPers1.push_back(_colorPercents[_frameIdx - 1][4]);
    // }
    // else
    // {
    //     _colorsPers2.push_back(_colorPercents[_frameIdx - 1][0]);
    //     _colorsPers2.push_back(_colorPercents[_frameIdx - 1][1]);
    //     _colorsPers2.push_back(_colorPercents[_frameIdx - 1][2]);
    //     _colorsPers2.push_back(_colorPercents[_frameIdx - 1][3]);
    //     _colorsPers2.push_back(_colorPercents[_frameIdx - 1][4]);
    // }

    // if(_lineID == 0)
    // {
    //     qDebug() << "-_______-" << _frameIdx << _colorsPers1;

    // }


    if(m_graderStatus == GraderStatus::Grading)
    {
        if(_lineID == 1)
        {
            //QtConcurrent::run(this, &ColorDetector::saveImmgs, _coloredFrames2);
        }

        for(int i=0;i<7;i++)
        {
            int _cupID = ((_cupNumber) % SharedData::_totalCupNumber + SharedData::_totalCupNumber - (i % 7) % 4) % (SharedData::_totalCupNumber);

            if(_output->lines[_lineID].cups[_cupID].Images[i % 7].colorPercents[0] > 20)
                _colorcup = _cupID;

            _output->lines[_lineID].cups[_cupID].Images[i % 7].colorPercents = _colorPercents[i % 7];
            _output->lines[_lineID].cups[_cupID].Images[i % 7].size = _sizes[i % 7];
            _output->lines[_lineID].cups[_cupID].Images[i % 7].density = _densities[i % 7];
        }

        // if(_lineID == 1)
        // {
        //     // if(_colorcup != 0)
        //     // {
        //     //     qDebug() << "mmmmmm0" << _output->lines[_lineID].cups[_colorcup].Images[0].colorPercents;
        //     //     qDebug() << "mmmmmm1" << _output->lines[_lineID].cups[_colorcup].Images[1].colorPercents;
        //     //     qDebug() << "mmmmmm2" << _output->lines[_lineID].cups[_colorcup].Images[2].colorPercents;
        //     //     qDebug() << "mmmmmm3" << _output->lines[_lineID].cups[_colorcup].Images[3].colorPercents;
        //     //     qDebug() << "mmmmmm4" << _output->lines[_lineID].cups[_colorcup].Images[4].colorPercents;
        //     //     qDebug() << "mmmmmm5" << _output->lines[_lineID].cups[_colorcup].Images[5].colorPercents;
        //     //     qDebug() << "mmmmmm6" << _output->lines[_lineID].cups[_colorcup].Images[6].colorPercents;
        //     // }
        //     for(int i=0;i<7;i++)
        //     {
        //         int _cupID = (_cupNumber % SharedData::_totalCupNumber + SharedData::_totalCupNumber - (i % 7) % 4) % (SharedData::_totalCupNumber);

        //         // if(_output->lines[_lineID].cups[_cupID].Images[4].colorPercents[0] > 10)
        //         // {
        //         //     qDebug() << "mmmmmm0" << _output->lines[_lineID].cups[_cupID].Images[0].colorPercents << _cupID;
        //         //     qDebug() << "mmmmmm1" << _output->lines[_lineID].cups[_cupID].Images[1].colorPercents << _cupID;
        //         //     qDebug() << "mmmmmm2" << _output->lines[_lineID].cups[_cupID].Images[2].colorPercents << _cupID;
        //         //     qDebug() << "mmmmmm3" << _output->lines[_lineID].cups[_cupID].Images[3].colorPercents << _cupID;
        //         //     qDebug() << "mmmmmm4" << _output->lines[_lineID].cups[_cupID].Images[4].colorPercents << _cupID;
        //         //     qDebug() << "mmmmmm5" << _output->lines[_lineID].cups[_cupID].Images[5].colorPercents << _cupID;
        //         //     qDebug() << "mmmmmm6" << _output->lines[_lineID].cups[_cupID].Images[6].colorPercents << _cupID;
        //         // }

        //         qDebug() << "____colorpercs_" << i << _output->lines[_lineID].cups[_cupID].Images[i % 7].colorPercents << _cupID;


        //     }
        // }

        // if(_lineID == 1)
        // {
        //     qDebug() << "____colorpercs_0" << _output->lines[_lineID].cups[_cup % 283].Images[0].colorPercents << _cup;
        //     qDebug() << "____colorpercs_1" << _output->lines[_lineID].cups[_cup % 283].Images[1].colorPercents << _cup - 1;
        //     qDebug() << "____colorpercs_2" << _output->lines[_lineID].cups[_cup % 283].Images[2].colorPercents << _cup - 2;
        //     qDebug() << "____colorpercs_3" << _output->lines[_lineID].cups[_cup % 283].Images[3].colorPercents << _cup - 3;
        //     qDebug() << "____colorpercs_4" << _output->lines[_lineID].cups[_cup % 283].Images[4].colorPercents << _cup;
        //     qDebug() << "____colorpercs_5" << _output->lines[_lineID].cups[_cup % 283].Images[5].colorPercents << _cup - 1;
        //     qDebug() << "____colorpercs_6" << _output->lines[_lineID].cups[_cup % 283].Images[6].colorPercents << _cup - 2;

        // }

        // if(_output->lines[1].cups[_cup % 283].Images[0].colorPercents[0] > 20)
        // {
        //     _colorcup = _cup % 283;
        // }

        // if(_output->lines[1].cups[_colorcup].Images[0].colorPercents[0] > 20)
        // {
        //     _colorPercents[i % 7];
        //     qDebug() << "COOOOOOOOOOOOO_0" << _output->lines[1].cups[_colorcup].Images[0].colorPercents << _colorcup;
        //     qDebug() << "COOOOOOOOOOOOO_1" << _output->lines[1].cups[_colorcup].Images[1].colorPercents << _colorcup;
        //     qDebug() << "COOOOOOOOOOOOO_2" << _output->lines[1].cups[_colorcup].Images[2].colorPercents << _colorcup;
        //     qDebug() << "COOOOOOOOOOOOO_3" << _output->lines[1].cups[_colorcup].Images[3].colorPercents << _colorcup;
        //     qDebug() << "COOOOOOOOOOOOO_4" << _output->lines[1].cups[_colorcup].Images[4].colorPercents << _colorcup;
        //     qDebug() << "COOOOOOOOOOOOO_5" << _output->lines[1].cups[_colorcup].Images[5].colorPercents << _colorcup;
        //     qDebug() << "COOOOOOOOOOOOO_6" << _output->lines[1].cups[_colorcup].Images[6].colorPercents << _colorcup;

        // }


    }


    //qDebug() << "Cuppppppppp_Last" << _cupNumber;


    // if(_imagesID.size() == 2)
    // {
    //     _output->lines[_lineID].cups[_cupNumber].Images[_imagesID[0]].colorPercents = _colorPercents[0];
    //     _output->lines[_lineID].cups[_cupNumber].Images[_imagesID[1]].colorPercents = _colorPercents[1];
    // }
    // else
    // {
    //     _output->lines[_lineID].cups[_cupNumber].Images[_imagesID[0]].colorPercents = _colorPercents[0];
    // }


    // _sizes.push_back(_size);
    // _densities.push_back(_density);


    auto end = std::chrono::high_resolution_clock::now();

    //qDebug() << "Color Percents Done____________: " << _cup;


    //qDebug() << "Color Percents: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

}

void ColorDetector::saveImmgs(std::vector<cv::Mat> frames)
{
    // cv::imwrite("f" + std::to_string(_cup) + "_" + std::to_string(1) + ".jpg", frames[0]);
    // cv::imwrite("f" + std::to_string(_cup-1) + "_" + std::to_string(3) + ".jpg", frames[1]);
    // cv::imwrite("f" + std::to_string(_cup-2) + "_" + std::to_string(5) + ".jpg", frames[2]);
    // cv::imwrite("f" + std::to_string(_cup-3) + "_" + std::to_string(7) + ".jpg", frames[3]);
    // cv::imwrite("f" + std::to_string(_cup) + "_" + std::to_string(2) + ".jpg", frames[4]);
    // cv::imwrite("f" + std::to_string(_cup-1) + "_" + std::to_string(4) + ".jpg", frames[5]);
    // cv::imwrite("f" + std::to_string(_cup-2) + "_" + std::to_string(6) + ".jpg", frames[6]);
}
