#include "MqttService.h"

MqttService::MqttService(const char* server, int port, const char* username)
    : _client(_espClient) {
    _server = server;
    _port = port;
    _username = username;
}

void MqttService::begin() {
    _client.setServer(_server, _port);
    _client.setBufferSize(1024);
}

void MqttService::reconnect() {
    while (!_client.connected()) {
        Serial.print("Connecting to Flespi MQTT...");

        String clientId = "esp32-air-monitor-01-" + String(random(0xffff), HEX);

        if (_client.connect(clientId.c_str(), _username, NULL)) {
            Serial.println("Connected!");
        } else {
            Serial.print("Connection failed! rc=");
            Serial.println(_client.state());
            delay(2000);
        }
    }
}

void MqttService::loop() {
    if (!_client.connected()) {
        reconnect();
    }

    _client.loop();
}

bool MqttService::publish(const String& topic, const String& payload) {
    return _client.publish(topic.c_str(), payload.c_str());
}

int MqttService::state() {
    return _client.state();
}