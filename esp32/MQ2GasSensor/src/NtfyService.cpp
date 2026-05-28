#include "NtfyService.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

NtfyService::NtfyService(const char* topic) {
    _topic = topic;
}

bool NtfyService::sendNotification(
    const char* title,
    const char* message,
    const char* priority,
    const char* tags
) {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Cannot send ntfy notification: WiFi not connected.");
        return false;
    }

    String url = "https://ntfy.sh/";
    url += _topic;

    WiFiClientSecure secureClient;
    secureClient.setInsecure(); // Simple for school project

    HTTPClient http;

    if (!http.begin(secureClient, url)) {
        Serial.println("Failed to start ntfy HTTP request.");
        return false;
    }

    http.addHeader("Title", title);
    http.addHeader("Priority", priority);
    http.addHeader("Tags", tags);
    http.addHeader("Content-Type", "text/plain");

    int responseCode = http.POST(message);

    Serial.print("ntfy response code: ");
    Serial.println(responseCode);

    http.end();

    return responseCode >= 200 && responseCode < 300;
}