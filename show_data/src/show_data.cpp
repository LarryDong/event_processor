#include <iostream>
#include <fstream>

#include <gflags/gflags.h>
#include <opencv2/opencv.hpp>
#include <processor.hpp>


using namespace std;


DEFINE_string(event_file, "/home/larrydong/output/celex.txt", "event file");
DEFINE_string(ts_file, "/home/larrydong/output/image/image_ts.txt", "image ts file");

DEFINE_bool(ts_from_file, false, "ts from file or 50ms");
DEFINE_int32(time_duration, 50, "time_duration from file or 50ms");

DEFINE_int32(camera_width, 1280, "camera width");
DEFINE_int32(camera_height, 800, "camera height");


DEFINE_int32(max_frame_number, 10000, "Max frame number (if ts_file not provided)");



int readTimestampFile(const string & filename, vector<size_t> & image_ts){
    ifstream f_ts(filename);
    if(!f_ts.is_open()){
        cout << "[Error] Cannot open ts file from: " << FLAGS_ts_file << endl;
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


cv::Mat getEventFrame(const vector<EventPoint>& eps, cv::Size image_size = cv::Size(1280, 1024)){
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


cv::Mat getAccumulateFrame(const vector<EventPoint>& eps, cv::Size image_size = cv::Size(1280, 1024)){
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


int main(int argc, char **argv){

    google::ParseCommandLineFlags(&argc, &argv, true);
    CameraConfig cameraConfig(FLAGS_camera_width, FLAGS_camera_height, 1);

    vector<size_t> v_image_ts;
    if(FLAGS_ts_from_file){
        // Read image ts.
        if(readTimestampFile(FLAGS_ts_file, v_image_ts) == 0){
            cout << "Image ts from ( " << v_image_ts[0] << ", " << v_image_ts[v_image_ts.size() - 1] << ") us." << endl;
        }
    }
    else{
        cout << "No image ts file. Using default duration: " << FLAGS_time_duration << " ms." << endl;
        for(int i=0; i<FLAGS_max_frame_number; ++i){
            double ts = i * FLAGS_time_duration * 1e3;
            v_image_ts.push_back(ts);
        }
    }


    // STEP 2. Read events & process.    
    ifstream f_events(FLAGS_event_file);
    if (!f_events.is_open()){
        cout << "Cannot open events ts file from: " << FLAGS_event_file << endl;
        return -1;
    }

    // vector<cv::Point> v_points;
    vector<EventPoint> event_points;
    for (int i = 0; i < v_image_ts.size(); ++i){
        event_points.clear();
        size_t ts = 0;
        int x, y, p;
        while(ts < v_image_ts[i]){        // TODO: ts is unsigned. Avoid negative values.
            f_events >> ts >> x >> y >> p;
            event_points.push_back(EventPoint(x, y, p));
        }
        cv::Mat img = getEventFrame(event_points, cameraConfig.getSize());
        // cv::Mat img = getAccumulateFrame(event_points, cameraConfig.getSize());
        cv::imshow("img", img);
        cv::waitKey(1);
        cout << "show event frame to: " << ts << endl;
        uchar c = cv::waitKey(0);
        if (c == 'q'){
            cout << "quit..." << endl;
            return 0;
        }
            
    }

    return 0;
}