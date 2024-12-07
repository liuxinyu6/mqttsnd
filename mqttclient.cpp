#include "mqttclient.h"
#include <cstring>
#include <QDebug>

MqttClient::MqttClient(const char* addr, int port, const char* username, const char* password):
    _port(port)
{
    strcpy(_addr, addr);
    strcpy(_username, username);
    strcpy(_password, password);
}

MqttClient::~MqttClient()
{
    if(_mos)
    {
        mosquitto_destroy(_mos);
    }
    mosquitto_lib_cleanup();
}

bool MqttClient::Init()
{
    int ret = mosquitto_lib_init();
    if(MOSQ_ERR_SUCCESS != ret)
    {
        qDebug()<<"mosquitto_lib_init failed,"<<mosquitto_strerror(ret);
        return false;
    }
    _mos = mosquitto_new(nullptr, true, nullptr);
    if(nullptr == _mos)
    {
        qDebug()<<"mosquitto_new failed,"<<mosquitto_strerror(errno);
        return false;
    }

 //   mosquitto_username_pw_set(_mos,_username,_password);

    mosquitto_connect_callback_set(_mos, MqttClient::on_connect);
    mosquitto_disconnect_callback_set(_mos, MqttClient::on_disconnect);
    mosquitto_publish_callback_set(_mos,MqttClient::on_publish);

    return true;
}

void MqttClient::on_connect(struct mosquitto * mos, void *userdata, int result)
{
    if(result != 0)
    {
        //重连
        qDebug()<<"connect failed: "<<mosquitto_strerror(result);
        mosquitto_reconnect(mos);
    }
    else
    {
        qDebug()<<"connect success";
    }
}

void MqttClient::on_disconnect(struct mosquitto *mos, void *userdata, int result)
{
    if(result == 0)
    {
        //重连
        qDebug()<<"disconnect success";
        mosquitto_reconnect(mos);
    }
}

void MqttClient::on_publish(struct mosquitto *mos, void *userdata, int result)
{

}

int MqttClient::Connect(int keepalive)
{
    int ret = mosquitto_connect(_mos, _addr, _port, keepalive);
    if(MOSQ_ERR_SUCCESS != ret)
    {
        qDebug()<<"mosquitto_connect failed,"<<mosquitto_strerror(ret);
    }
    return ret;
}

int MqttClient::Publish(int *mid, const char *topic, int payloadlen, const void *payload, int qos, bool retain)
{
    int ret = mosquitto_publish(_mos, mid, topic, payloadlen, payload, qos, retain);
    if(MOSQ_ERR_SUCCESS != ret)
    {
        qDebug()<<"mosquitto_publish failed,"<<mosquitto_strerror(ret);
    }
    return ret;
}

int MqttClient::Loop(int timeout, int max_packets)
{
    int ret = mosquitto_loop_forever(_mos, timeout, max_packets);
    if(MOSQ_ERR_SUCCESS != ret)
    {
        qDebug()<<"loop failed,"<<mosquitto_strerror(ret);
    }
    return ret;
}
