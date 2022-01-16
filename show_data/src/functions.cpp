
#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include "processor.hpp"

using namespace std;

int readTimestampFile(const string & filename, vector<size_t> & image_ts){
    ifstream f_ts(filename);
    if(!f_ts.is_open()){
        cout << "[Error] Cannot open ts file from: " << filename << endl;
        std::abort();
    }
    image_ts.resize(0);
    while(!f_ts.eof()){
        int idx;
        size_t ts;
        f_ts >> idx >> ts;
        image_ts.push_back(ts);
    }
    return 0;
}


cv::Mat getEventFrame(const vector<EventPoint>& eps, cv::Size image_size){
    cv::Mat img = cv::Mat::zeros(image_size, CV_8UC1);
    img.setTo(128);
    uchar *data = img.data;
    for(auto ep : eps){
        int idx = ep.y * image_size.width + ep.x;
        uchar v = ep.p == 0 ? 0 : 255;
        data[idx] = v;
    }
    return img;
}


cv::Mat getAccumulateFrame(const vector<EventPoint>& eps, cv::Size image_size){
    cv::Mat img = cv::Mat::zeros(image_size, CV_8UC1);
    img.setTo(0);
    uchar *data = img.data;
    for(auto ep : eps){
        int idx = ep.y * image_size.width + ep.x;
        // uchar v = ep.p == 0 ? 0 : 255;
        int v = data[idx];
        v += 10;
        v = (v > 255 ? 255 : (v < 0 ? 0 : v));
        data[idx] = v;
    }
    return img;
}
