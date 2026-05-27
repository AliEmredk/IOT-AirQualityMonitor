#include "AppWiFi.h"

AppWiFi::AppWiFi(const char* ssid, const char* password) {
    _ssid = ssid;
    _password = password;
    _lastWiFiCheck = 0;
}
    
void AppWiFi::begin() {
    WiFi.onEvent(onWiFiEvent);
    connect();
}

void AppWiFi::onWiFiEvent(WiFiEvent_t event) {
    switch (event) {
        case ARDUINO_EVENT_WIFI_STA_CONNECTED:
            Serial.println("WiFi connected to router.");
            break;

        case ARDUINO_EVENT_WIFI_STA_GOT_IP:
            Serial.print("WiFi IP address: ");
            Serial.println(WiFi.localIP());
            break;

        case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
            Serial.println("WiFi disconnected.");
            break;

        default:
            break;
    }
}

bool AppWiFi::connect() {
    Serial.print("Connecting to WiFi: ");
    Serial.println(_ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(_ssid, _password);

    unsigned long startAttemptTime = millis();
    const unsigned long WIFI_TIMEOUT = 10000;

    while (WiFi.status() != WL_CONNECTED &&
           millis() - startAttemptTime < WIFI_TIMEOUT) {
        Serial.print(".");
        delay(500);
    }

    Serial.println();

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("WiFi connection successful.");
        return true;
    } else {
        Serial.println("WiFi connection failed.");
        return false;
    }
}

void AppWiFi::checkReconnect() {
    if (millis() - _lastWiFiCheck >= WIFI_CHECK_INTERVAL) {
        _lastWiFiCheck = millis();

        if (WiFi.status() != WL_CONNECTED) {
            Serial.println("WiFi not connected. Trying to reconnect...");
            connect();
        }
    }
}