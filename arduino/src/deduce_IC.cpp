#include "deduce_IC.hpp"

#include <Arduino.h>

void setPin(PinFormat pinFormat, bool (&result)[12]) {
    int loop = 0;
    for (int pin = A0; pin <= A11; ++pin) {
        switch (pinFormat)
        {
        case IN_OUT:
            if (loop == 0) {
                pinMode(pin, OUTPUT);
                result[pin - A0] = true;
                loop++;
            }
            else {
                pinMode(pin, INPUT);
                result[pin - A0] = false;
                loop = 0;
            };
            break;
        case IN_IN_OUT:
            if (loop == 0) {
                pinMode(pin, OUTPUT);
                result[pin - A0] = true;
                loop++;
            }
            else if (loop == 1) {
                pinMode(pin, OUTPUT);
                result[pin - A0] = true;
                loop++;
            }
            else {
                pinMode(pin, INPUT);
                result[pin - A0] = false;
                loop = 0;
            };
            break;
        case OUT_IN_IN:
            if (loop == 0) {
                pinMode(pin, INPUT);
                result[pin - A0] = false;
                loop++;
            }
            else if (loop == 1) {
                pinMode(pin, OUTPUT);
                result[pin - A0] = true;
                loop++;
            }
            else {
                pinMode(pin, OUTPUT);
                result[pin - A0] = true;
                loop = 0;
            };
            break;
        }
    }
}

void injectInput(bool pattern[12], bool (&results)[4]) {
    static bool conditionPattern[8] = {false,false,false,true,true,false,true,true};
    int inputIndex = 0;
    for (int pin = A0; pin <= A11; ++pin) {
        if (pattern[pin - A0]) {
            if (conditionPattern[inputIndex]) {
                digitalWrite(pin, HIGH);
            } else {
                digitalWrite(pin, LOW);
            }
            inputIndex++;
        }
    }
    int resultIndex = 0;
    for (int pin = A0; pin <= A11; ++pin) {
        if (!pattern[pin - A0]) {
            results[resultIndex] = digitalRead(pin);
            resultIndex++;
        }
    }
}

ICType deduceIC(PinFormat pinFormat) {
    bool pattern[12];
    setPin(pinFormat, pattern);
    bool results[4];
    injectInput(pattern, results);

    static bool IC[4][4] = {
        {false,false,false,true}, //AND
        {false,true,true,true},    //OR
        {true,true,true,false},    //NAND
        {false,true,true,false}    //XOR
    };
    for (int ic = 0; ic < 4; ++ic) {
        bool match = true;
        for (int output = 0; output < 4; ++output) {
            if (results[output] != IC[ic][output]) {
                match = false;
                break;
            }
        }
        if (match) {
            return static_cast<ICType>(ic);
        }
    }
    return RUSAK;
}