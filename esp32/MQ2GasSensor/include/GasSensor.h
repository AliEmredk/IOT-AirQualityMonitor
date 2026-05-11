#ifndef GAS_SENSOR_H
#define GAS_SENSOR_H

class GasSensor {
public:
    GasSensor(int analogPin, int digitalPin);

    void begin();
    void calibrate();

    int readAnalog();
    int readDigital();

    bool isDanger(int analogValue);

    int getBaseline();
    int getDangerThreshold();

private:
    int _analogPin;
    int _digitalPin;

    int _baseline;
    int _dangerThreshold;

    static const int WARMUP_SECONDS = 120;
    static const int BASELINE_SAMPLES = 50;
    static const int DANGER_OFFSET = 500;
};

#endif