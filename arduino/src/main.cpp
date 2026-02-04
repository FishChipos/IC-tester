#include <Arduino.h>

#include "deduce_pins.hpp"
#include "deduce_IC.hpp"

static bool buttonPressed = false;

void setup() {
    pinMode(2, OUTPUT);
    pinMode(3, INPUT);
    pinMode(4, OUTPUT);

    digitalWrite(2, HIGH);
    digitalWrite(4, LOW);

    Serial.begin(115200);

    delay(1000);
}

void loop() {
    if (!buttonPressed && digitalRead(3)) {
        buttonPressed = true;
    }

    if (buttonPressed && !digitalRead(3)) {
        delay(100);
        buttonPressed = false;
        PinFormat pinFormat = deducePins();
        ICType icType = deduceIC(pinFormat);
        Serial.print("Detected IC Type: ");
        switch (icType) {
            case AND:
                Serial.println("AND");
                break;
            case OR:
                Serial.println("OR");
                break;
            case NAND:
                Serial.println("NAND");
                break;
            case XOR:
                Serial.println("XOR");
                break;
            case NOT:
                Serial.println("NOT");
                break;
            case NOR:
                Serial.println("NOR");
                break;
            case RUSAK:
                Serial.println("RUSAK");
                break;
        }
    }
}