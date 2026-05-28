#ifndef BUZZER_H
#define BUZZER_H

class Buzzer {
public:
    Buzzer(int pin);

    void begin();
    void handle(bool isDanger);

private:
    int _pin;
    unsigned long _lastMelodyTime;

    static const int BUZZER_CHANNEL = 0;
    static const int BUZZER_RESOLUTION = 8;

    void playTone(int frequency, int durationMs);
    void dangerMelody();
};

#endif