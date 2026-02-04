#include "deduce_pins.hpp"

#include <Arduino.h>

int getDigits(int value) {
    int digits = 1;

    for (; value != value % 10; value /= 10) {
        ++digits;
    }

    return digits;
}

void printSamples(int samples[64][12]) {
    for (int sample = 0; sample < 1; ++sample) {
        for (int pin = A0; pin <= A11; ++pin) {
            int value = samples[sample][pin - A0];
            Serial.print(value);

            for (int space = 0; space < 5 - getDigits(value); ++space) {
                Serial.print(" ");
            }
        }
        Serial.println();
    }
}

PinFormat deducePins() {
    for (int pin = A0; pin <= A11; ++pin) {
        pinMode(pin, INPUT);
    }

    int samples[64][12];

    for (int sample = 0; sample < 1; ++sample) {
        for (int pin = A0; pin <= A11; ++pin) {
            samples[sample][pin - A0] = analogRead(pin);
        }
    }

    printSamples(samples);

    return IN_IN_OUT;
}