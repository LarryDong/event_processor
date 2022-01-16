
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;

class CameraConfig{
public:
    CameraConfig(void) { cout << "Empyt init not supported"; }
    CameraConfig(int width, int height, double r = 1.0) : width(width), height(height), r(r) {}
    int width;
    int height;
    double r;

    cv::Size getSize(void) { return cv::Size(width, height); }
};


class EventPoint{
public:
    EventPoint(int x, int y, int p = 0) : x(x), y(y), p(p) {}
    int x;
    int y;
    int p;

    cv::Point getPoint(void) { return cv::Point(x, y); }
};


int readTimestampFile(const string & filename, vector<size_t> & image_ts);
cv::Mat getEventFrame(const vector<EventPoint>& eps, cv::Size image_size = cv::Size(1280, 1024));
cv::Mat getAccumulateFrame(const vector<EventPoint>& eps, cv::Size image_size = cv::Size(1280, 1024));
