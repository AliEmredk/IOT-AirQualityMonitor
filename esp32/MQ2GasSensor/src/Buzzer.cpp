#include "Buzzer.h"
#include <Arduino.h>

Buzzer::Buzzer(int pin) {
    _pin = pin;
    _lastMelodyTime = 0;
}

void Buzzer::begin() {
    pinMode(_pin, OUTPUT);

    // ESP32 PWM setup for buzzer
    ledcSetup(BUZZER_CHANNEL, 2000, BUZZER_RESOLUTION);
    ledcAttachPin(_pin, BUZZER_CHANNEL);

    ledcWrite(BUZZER_CHANNEL, 0);
}

void Buzzer::playTone(int frequency, int durationMs) {
    ledcWriteTone(BUZZER_CHANNEL, frequency);
    ledcWrite(BUZZER_CHANNEL, 128); // 50% duty cycle

    delay(durationMs);

    ledcWrite(BUZZER_CHANNEL, 0);
    delay(30);
}

void Buzzer::dangerMelody() {
    playTone(1200, 200);
    playTone(1800, 200);
    playTone(2200, 300);

    delay(100);

    playTone(1800, 200);
    playTone(2200, 500);
}

void Buzzer::handle(bool isDanger) {
    if (isDanger) {
        unsigned long currentTime = millis();

        if (currentTime - _lastMelodyTime >= 1600) {
            _lastMelodyTime = currentTime;

            Serial.println("DANGEROUS GAS DETECTED!");
            dangerMelody();
        }
    } else {
        ledcWrite(BUZZER_CHANNEL, 0);
    }
}