#pragma once
#include <Arduino.h>
#include <PubSubClient.h>
#include <Client.h>

#define MQTT_RECONNECT_INTERVAL 5000

typedef enum
{
    MQTT_EVT_CONNECT,
    MQTT_EVT_DISCONNECT,
    MQTT_EVT_CONNECT_FAILED
} MQTTEventType;
class MQTT
{

    using MQTTEventHandler = std::function<void(MQTTEventType type)>;

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
    void onMessage(void (*callback)(char *, uint8_t *, unsigned int));
    bool isConnected();
    bool connect();
    int state();
    MQTTEventHandler _eventHandler{nullptr};
    void onEvent(MQTTEventHandler handler);

private:
    static MQTT *_instance;
    PubSubClient *_client;
    String _broker;
    uint16_t _port;
    String _id;
    String _user;
    String _pass;
    unsigned long _lastReconnectAttempt = 0;
};
