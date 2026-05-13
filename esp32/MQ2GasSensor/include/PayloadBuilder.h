#ifndef PAYLOAD_BUILDER_H
#define PAYLOAD_BUILDER_H

#include <Arduino.h>

class PayloadBuilder {
public:
    PayloadBuilder(const char* deviceId);

    String buildTelemetryPayload(
        long timestamp,
        int analogValue,
        int digitalValue,
        int baseline,
        int dangerThreshold,
        float temperature,
        float humidity,
        float pressure,
        bool isDanger
    );

private:
    const char* _deviceId;
};

#endif