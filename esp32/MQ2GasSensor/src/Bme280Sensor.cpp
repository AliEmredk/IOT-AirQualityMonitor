#include "Bme280Sensor.h"
#include <Arduino.h>
#include <Wire.h>

Bme280Sensor::Bme280Sensor(int sdaPin, int sclPin) {
    _sdaPin = sdaPin;
    _sclPin = sclPin;
}

void Bme280Sensor::begin() {
    Wire.begin(_sdaPin, _sclPin);

    Serial.println("Starting BME280...");

    if (!_bme.begin(0x76)) {
        Serial.println("Could not find BME280 at 0x76, trying 0x77...");

        if (!_bme.begin(0x77)) {
            Serial.println("Could not find BME280 sensor!");
            Serial.println("Check wiring: VCC, GND, SDA=21, SCL=22");
        } else {
            Serial.println("BME280 found at 0x77");
        }
    } else {
        Serial.println("BME280 found at 0x76");
    }
}

float Bme280Sensor::readTemperature() {
    return _bme.readTemperature();
}

float Bme280Sensor::readHumidity() {
    return _bme.readHumidity();
}

float Bme280Sensor::readPressure() {
    return _bme.readPressure() / 100.0F;
}