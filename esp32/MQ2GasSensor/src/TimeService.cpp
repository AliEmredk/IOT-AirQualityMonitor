#include "TimeService.h"
#include <Arduino.h>
#include <time.h>

void TimeService::syncTime() {
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");

    Serial.println("Syncing time with NTP...");

    struct tm timeinfo;

    while (!getLocalTime(&timeinfo)) {
        Serial.println("Waiting for NTP time...");
        delay(1000);
    }

    Serial.println("Time synchronized!");
}

long TimeService::getTimestamp() {
    time_t now;
    time(&now);
    return now;
}