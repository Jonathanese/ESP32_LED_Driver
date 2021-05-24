#include "MQTT.h"
#include "secrets.h"

#define FRAME_TIME_MAINTAIN 100
#define FRAME_TIME_CONNECTING 10
#define RETRY_TIME 4000
enum CONNECT_STATE
{
    IDLE,
    WIFI_CONNECTING,
    MQTT_CONNECTING,
    MQTT_SUBSCRIBING,
    MAINTAINING
};

StopWatch _frame_timer;
StopWatch _retry_timer;
WiFiClient _wifi_client;
PubSubClient _mqtt_client(_wifi_client);
uint16_t _frame_time = FRAME_TIME_CONNECTING;
CONNECT_STATE _currentState = IDLE;

void unhandled_message_callback(char *topic, byte *payload, unsigned int length)
{
    char *message = new char[length + 1];

    for (uint8_t i = 0; i < length; i++)
    {
        message[i] = (char)payload[i];
    }

    message[length] = '\0';

    Debug.Message(DM_RECEIVE, "[" + String(topic) + "]: " + String(message));
    delete message;
}

void _wifiinit()
{
    Debug.Message(DM_INFO, "Connecting to WIFI");
    _currentState = WIFI_CONNECTING;
    _frame_time = FRAME_TIME_CONNECTING;

    WiFi.mode(WIFI_STA);
    WiFi.begin(SECRET_WIFI_NAME, SECRET_WIFI_PASSWORD);
}

void _mqttinit()
{
    Debug.Message(DM_INFO, "Connecting to MQTT");
    _currentState = MQTT_CONNECTING;
    _frame_time = FRAME_TIME_CONNECTING;
    _retry_timer.reset(RETRY_TIME);

    _mqtt_client.setServer(SECRET_MQTT_SERVER, 1883);
}

void _mqttsubscribeinit()
{
    Debug.Message(DM_INFO, "Subscribing.");
    _currentState = MQTT_SUBSCRIBING;
}

void _mqttconnect()
{
    if (!_mqtt_client.connected())
    {
        if (_retry_timer.repeat(4000))
        {
            if (_mqtt_client.connect(SECRET_MQTT_NAME, SECRET_MQTT_ID, SECRET_MQTT_PWD))
            {
                _mqttsubscribeinit();
            }
        }
    }
}

void _mqttsubscribe()
{
    if (_retry_timer.repeat(RETRY_TIME))
    {
        if (_mqtt_client.subscribe(SECRET_MQTT_COMMAND_TOPIC))
        {
            _currentState = MAINTAINING;
            _frame_time = FRAME_TIME_MAINTAIN;
            Debug.Message(DM_INFO, "Ready");
        }
    }
}

void _wificonnect()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        _mqttinit();
    }
}

void _maintain()
{
    _mqtt_client.loop();
    if (!_mqtt_client.connected())
    {
        if (!WiFi.isConnected())
        {
            _wifiinit();
        }
        else
        {
            _mqttinit();
        }
    }
}

void MQTT::loop()
{
    if (_frame_timer.repeat(_frame_time))
    {
        switch (_currentState)
        {
        case IDLE:
            _wifiinit();
            break;
        case WIFI_CONNECTING:
            _wificonnect();
            break;
        case MQTT_CONNECTING:
            _mqttconnect();
            break;
        case MQTT_SUBSCRIBING:
            _mqttsubscribe();
            break;
        case MAINTAINING:
            _maintain();
            break;
        }
    }
}

void MQTT::publish(const char* topic, const char* payload)
{
    _mqtt_client.publish(topic, payload, true);
}