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

void MQTT::setLWT(const char *willTopic, uint8_t willQos, boolean willRetain, const char *willMessage)
{
    _willTopic = willTopic;
    _willQos = willQos;
    _willRetain = willRetain;
    _willMessage = willMessage;
}

bool MQTT::connect()
{
    bool connected = false;
    if (strlen(_willTopic) > 0)
    {
        if(strlen(_prefix) > 0) {
            char newTopic[strlen(_prefix) + strlen(_willTopic) + 1];
            strcpy(newTopic, _prefix);
            strcat(newTopic, _willTopic);
            connected = _client->connect(_id, _user, _pass, newTopic, _willQos, _willRetain, _willMessage);
        } else {
            connected = _client->connect(_id, _user, _pass, _willTopic, _willQos, _willRetain, _willMessage);
        }
    }
    else
    {
        connected = _client->connect(_id, _user, _pass);
    }

    if (connected)
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

const char *MQTT::state()
{
    switch (_client->state())
    {
    case MQTT_CONNECTION_TIMEOUT:
        return "MQTT_CONNECTION_TIMEOUT";
    case MQTT_CONNECTION_LOST:
        return "MQTT_CONNECTION_LOST";
    case MQTT_CONNECT_FAILED:
        return "MQTT_CONNECT_FAILED";
    case MQTT_DISCONNECTED:
        return "MQTT_DISCONNECTED";
    case MQTT_CONNECTED:
        return "MQTT_CONNECTED";
    case MQTT_CONNECT_BAD_PROTOCOL:
        return "MQTT_CONNECT_BAD_PROTOCOL";
    case MQTT_CONNECT_BAD_CLIENT_ID:
        return "MQTT_CONNECT_BAD_CLIENT_ID";
    case MQTT_CONNECT_UNAVAILABLE:
        return "MQTT_CONNECT_UNAVAILABLE";
    case MQTT_CONNECT_BAD_CREDENTIALS:
        return "MQTT_CONNECT_BAD_CREDENTIALS";
    case MQTT_CONNECT_UNAUTHORIZED:
        return "MQTT_CONNECT_UNAUTHORIZED";
    default:
        return "UNKNOWN";
    }
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
    // if prefix length is greater than 0, prepend it to the topic
    if (strlen(_prefix) > 0)
    {
        char newTopic[strlen(_prefix) + strlen(topic) + 1];
        strcpy(newTopic, _prefix);
        strcat(newTopic, topic);
        _client->publish(newTopic, payload);
    }
    else
    {
        +_client->publish(topic, payload);
    }
}

void MQTT::subscribe(const char *topic)
{
    if(strlen(_prefix) > 0) {
        char newTopic[strlen(_prefix) + strlen(topic) + 1];
        strcpy(newTopic, _prefix);
        strcat(newTopic, topic);
        _client->subscribe(newTopic);
    } else {
        _client->subscribe(topic);
    }
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

void MQTT::setPrefix(const char *prefix)
{
    _prefix = prefix;
}

void MQTT::setBufferSize(uint16_t size)
{
    _client->setBufferSize(size);
}