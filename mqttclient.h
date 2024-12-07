#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

#include "mosquitto.h"

class MqttClient
{
public:
    MqttClient(const char* addr = "127.0.0.1", int port = 1883,
               const char* username = nullptr, const char* password = nullptr);
    ~MqttClient();

    bool Init();

    int Connect(int keepalive);
    int Publish(int *mid, const char *topic, int payloadlen, const void *payload, int qos, bool retain);
    int Loop(int 		timeout,int 		max_packets	);

public:
        static void on_connect(struct mosquitto *, void *, int);
        static void on_disconnect(struct mosquitto *, void *, int);
        static void on_publish(struct mosquitto *, void *, int);
private:
    int _port;
    const static int addrlen{1024};
    char _addr[addrlen];

    const static int infolen{100};
    char _username[infolen];
    char _password[infolen];

    mosquitto* _mos;
};

#endif // MQTTCLIENT_H
