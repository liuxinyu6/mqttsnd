#ifndef FRAMESEND_H
#define FRAMESEND_H
#include "mqttclient.h"


class FrameSend
{
public:
    FrameSend(const char* addr = "127.0.0.1", int port = 1883,
              const char* username = nullptr, const char* password = nullptr, const char* topic = "",
              int index = -1, const char* path = "");
    void send();
private:
    char _topic[1024];
    int _index;
    char _path[1024];
    MqttClient _mqttclient;
};

#endif // FRAMESEND_H
