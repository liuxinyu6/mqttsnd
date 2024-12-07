#include "framesend.h"
#include <thread>
#include <opencv2/opencv.hpp>
#include <iostream>

FrameSend::FrameSend(const char* addr, int port,
                     const char* username, const char* password, const char* topic,
                     int index, const char* path)
    :_index(index), _mqttclient(addr, port, username, password)
{
    strcpy(_topic, topic);

    memset(_path, 0, 1024);
    strcpy(_path, path);

    _mqttclient.Init();
    _mqttclient.Connect(60);
    std::thread f([&]()
    {
        _mqttclient.Loop(-1, 1);
    });
    f.detach();
};

void FrameSend::send()
{
    cv::VideoCapture cap;
    if(_index >= 0)
    {
        cap.open(_index);
    }
    else if(strlen(_path) > 0)
    {
        cap.open(_path);
    }
    if(cap.isOpened())
    {
        cv::Mat frame;
        cap>>frame;
        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);

        double fps = (double)1000 / cap.get(cv::CAP_PROP_FPS);
        int len = sizeof(int) * 3 + frame.total() * frame.elemSize();

        unsigned char* p = new unsigned char[len];
        memset(p, 0, len);
        while(!frame.empty())
        {
            int width = frame.cols;
            int height = frame.rows;
            int type = frame.type();
            memcpy(p, &width, sizeof(int));
            memcpy(p + sizeof(int), &height, sizeof(int));
            memcpy(p + sizeof(int) * 2, &type, sizeof(int));
            memcpy(p + sizeof(int) * 3, frame.data, frame.total() * frame.elemSize());
            _mqttclient.Publish(nullptr, _topic, len, p, 1, false);
            cv::waitKey(fps * 2);
            memset(p, 0, len);
            std::cout<<frame.total() * frame.elemSize()<<"\n";
            cap>>frame;
            cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
        }
        delete []p;
    }
}
