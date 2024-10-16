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
    void setLWT(const char* willTopic, uint8_t willQos, boolean willRetain, const char* willMessage);
    void loop();
    void publish(const char *topic, const char *payload);
    void subscribe(const char *topic);
    void onMessage(void (*callback)(char *, uint8_t *, unsigned int));
    void setPrefix(const char *prefix);
    bool isConnected();
    bool connect();
    const char * state();
    MQTTEventHandler _eventHandler{nullptr};
    void onEvent(MQTTEventHandler handler);
    void setBufferSize(uint16_t size);

private:
    static MQTT *_instance;
    PubSubClient *_client;
    const char *_broker;
    uint16_t _port;
    const char *_id;
    const char *_user;
    const char *_pass;
    const char *_prefix;
    const char *_willTopic;
    uint8_t _willQos;
    boolean _willRetain;
    const char *_willMessage;
    unsigned long _lastReconnectAttempt = 0;
};
