#include "PayloadBuilder.h"

PayloadBuilder::PayloadBuilder(const char* deviceId) {
    _deviceId = deviceId;
}

String PayloadBuilder::buildTelemetryPayload(
    long timestamp,
    int analogValue,
    int digitalValue,
    int baseline,
    int dangerThreshold,
    float temperature,
    float humidity,
    float pressure,
    bool isDanger
) {
    String payload = "{";

    payload += "\"deviceId\":\"" + String(_deviceId) + "\",";
    payload += "\"timestamp\":" + String(timestamp) + ",";

    payload += "\"gas\":{";
    payload += "\"analogValue\":" + String(analogValue) + ",";
    payload += "\"digitalValue\":" + String(digitalValue) + ",";
    payload += "\"baseline\":" + String(baseline) + ",";
    payload += "\"dangerThreshold\":" + String(dangerThreshold) + ",";
    payload += "\"dangerDetected\":" + String(isDanger ? "true" : "false");
    payload += "},";

    payload += "\"environment\":{";
    payload += "\"temperatureC\":" + String(temperature) + ",";
    payload += "\"humidityPercent\":" + String(humidity) + ",";
    payload += "\"pressureHpa\":" + String(pressure);
    payload += "},";

    payload += "\"alarm\":{";
    payload += "\"buzzerActive\":" + String(isDanger ? "true" : "false");
    payload += "}";

    payload += "}";

    return payload;
}