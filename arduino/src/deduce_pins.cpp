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
    int outputThresholdlow = 70;
    int outputThresholdHigh = 1000;

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

    if ((samples[0][0] > outputThresholdlow || samples[0][0] < outputThresholdHigh) &&
        (samples[0][1] < outputThresholdlow || samples[0][1] > outputThresholdHigh) &&
        (samples[0][2] > outputThresholdlow || samples[0][2] < outputThresholdHigh) ) {
        Serial.println("IN_OUT");
        return IN_OUT;
    } 
    else if (
        (samples[0][0] < outputThresholdlow || samples[0][0] > outputThresholdHigh) &&
        (samples[0][1] > outputThresholdlow || samples[0][1] < outputThresholdHigh) &&
        (samples[0][2] > outputThresholdlow || samples[0][2] < outputThresholdHigh) ) {
        Serial.println("OUT_IN_IN");
        return OUT_IN_IN;
    }  else {
        Serial.println("IN_IN_OUT");
        return IN_IN_OUT;
    }

}