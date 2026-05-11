#ifndef BME280_SENSOR_H
#define BME280_SENSOR_H

#include <Adafruit_BME280.h>

class Bme280Sensor {
public:
    Bme280Sensor(int sdaPin, int sclPin);

    void begin();

    float readTemperature();
    float readHumidity();
    float readPressure();

private:
    int _sdaPin;
    int _sclPin;

    Adafruit_BME280 _bme;
};

#endif