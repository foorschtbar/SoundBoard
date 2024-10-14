#pragma once
#include <Arduino.h>
#include <PubSubClient.h>
#include <Client.h>

class MQTT
{
public:
    inline static MQTT *instance()
    {
        return _instance;
    }
    explicit MQTT(Client &espClient);
    ~MQTT();
    bool begin(const char *broker, uint16_t port, const char *id, const char *user, const char *pass);
    void loop();
    void publish(const char *topic, const char *payload);
    void subscribe(const char *topic);
    void setCallback(void (*callback)(char *, uint8_t *, unsigned int));
    bool isConnected();
    bool connect();
    int state();
private:
    static MQTT *_instance;
    PubSubClient* _client;
    String _broker;
    uint16_t _port;
    String _id;
    String _user;
    String _pass;
    unsigned long _lastReconnectAttempt = 0;
};
