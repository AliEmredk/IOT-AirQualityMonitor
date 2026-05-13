#ifndef MQTT_SERVICE_H
#define MQTT_SERVICE_H

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

class MqttService {
public:
    MqttService(const char* server, int port, const char* username);

    void begin();
    void reconnect();
    void loop();

    bool publish(const String& topic, const String& payload);
    int state();

private:
    const char* _server;
    int _port;
    const char* _username;

    WiFiClient _espClient;
    PubSubClient _client;
};

#endif