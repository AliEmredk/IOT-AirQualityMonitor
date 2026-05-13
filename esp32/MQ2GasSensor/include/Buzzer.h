#ifndef BUZZER_H
#define BUZZER_H

class Buzzer {
public:
    Buzzer(int pin);

    void begin();
    void handle(bool isDanger);

private:
    int _pin;

    void beep();
};

#endif