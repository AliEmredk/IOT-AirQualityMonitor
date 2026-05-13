#include "GasSensor.h"
#include <Arduino.h>

GasSensor::GasSensor(int analogPin, int digitalPin) {
    _analogPin = analogPin;
    _digitalPin = digitalPin;
    _baseline = 0;
    _dangerThreshold = 0;
}

void GasSensor::begin() {
    pinMode(_digitalPin, INPUT);
}

void GasSensor::calibrate() {
    Serial.println("MQ-2 Gas Sensor Starting...");
    Serial.println("Warming up sensor...");

    for (int i = WARMUP_SECONDS; i > 0; i--) {
        Serial.print("Warm-up remaining: ");
        Serial.print(i);
        Serial.println(" seconds");
        delay(1000);
    }

    Serial.println("Measuring clean air baseline...");

    long total = 0;

    for (int i = 0; i < BASELINE_SAMPLES; i++) {
        int value = analogRead(_analogPin);
        total += value;

        Serial.print("Baseline sample ");
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.println(value);

        delay(200);
    }

    _baseline = total / BASELINE_SAMPLES;
    _dangerThreshold = _baseline + DANGER_OFFSET;

    Serial.println("Calibration finished.");
    Serial.print("Clean air baseline: ");
    Serial.println(_baseline);
    Serial.print("Danger threshold: ");
    Serial.println(_dangerThreshold);
}
    
int GasSensor::readAnalog() {
    return analogRead(_analogPin);
}

int GasSensor::readDigital() {
    return digitalRead(_digitalPin);
}

bool GasSensor::isDanger(int analogValue) {
    return analogValue > _dangerThreshold;
}

int GasSensor::getBaseline() {
    return _baseline;
}

int GasSensor::getDangerThreshold() {
    return _dangerThreshold;
}