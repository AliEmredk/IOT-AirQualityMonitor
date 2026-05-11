#include "Buzzer.h"
#include <Arduino.h>

Buzzer::Buzzer(int pin) {
    _pin = pin;
}

void Buzzer::begin() {
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
}

void Buzzer::beep() {
    for (int i = 0; i < 100; i++) {
        digitalWrite(_pin, HIGH);
        delayMicroseconds(500);
        digitalWrite(_pin, LOW);
        delayMicroseconds(500);
    }
}

void Buzzer::handle(bool isDanger) {
    if (isDanger) {
        Serial.println("DANGEROUS GAS DETECTED!");
        beep();
    } else {
        Serial.println("Air looks normal");
        digitalWrite(_pin, LOW);
    }
}