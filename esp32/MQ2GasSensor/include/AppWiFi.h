#ifndef APP_WIFI_H
#define APP_WIFI_H

#include <Arduino.h>
#include <WiFi.h>

class AppWiFi {
public:
    AppWiFi(const char* ssid, const char* password);

    void begin();
    bool connect();
    void checkReconnect();

private:
    const char* _ssid;
    const char* _password;

    unsigned long _lastWiFiCheck;
    const unsigned long WIFI_CHECK_INTERVAL = 5000;

    static void onWiFiEvent(WiFiEvent_t event);
};

#endif