#include "mqtt.h"

MQTT *MQTT::_instance = nullptr;

MQTT::MQTT(Client &_espClient) : _client(new PubSubClient(_espClient))
{
    _instance = this;
}

MQTT::~MQTT()
{
    if (_client)
    {
        delete _client;
    }
}

bool MQTT::begin(const char *broker, uint16_t port, const char *id, const char *user, const char *pass)
{
    _broker = broker;
    _port = port;
    _id = id;
    _user = user;
    _pass = pass;
    _client->setServer(broker, port);
    return _client->connect(id, user, pass);
}

bool MQTT::connect()
{
    if (_client->connect(_id.c_str(), _user.c_str(), _pass.c_str()))
    {
        _eventHandler(MQTT_EVT_CONNECT);
        return true;
    }
    else
    {
        _eventHandler(MQTT_EVT_CONNECT_FAILED);
        return false;
    }
}

int MQTT::state()
{
    return _client->state();
}

void MQTT::loop()
{
    _client->loop();
    if (!_client->connected())
    {
        if (millis() - _lastReconnectAttempt > MQTT_RECONNECT_INTERVAL)
        {
            _lastReconnectAttempt = millis();
            if (connect())
            {
                _lastReconnectAttempt = 0;
            }
        }
    }
}

void MQTT::publish(const char *topic, const char *payload)
{
    _client->publish(topic, payload);
}

void MQTT::subscribe(const char *topic)
{
    _client->subscribe(topic);
}

void MQTT::onMessage(void (*callback)(char *, uint8_t *, unsigned int))
{
    _client->setCallback(callback);
}

bool MQTT::isConnected()
{
    return _client->connected();
}

void MQTT::onEvent(MQTTEventHandler handler)
{
    _eventHandler = handler;
}