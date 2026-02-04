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

void injectInput(bool pattern[12], bool (&results)[6], int trial = 0) {
    static bool conditionPattern1[8] = {false,false,false,true,true,false,true,true}; //00011011
    static bool conditionPattern2[8] = {false,true,true,false,true,true,false,false}; //01101100
    static bool conditionPattern3[8] = {true,false,true,true,false,false,false,true}; //10110001
    static bool conditionPattern4[8] = {true,true,false,false,false,true,true,false}; //11000110

    int inputIndex = 0;
    for (int pin = A0; pin <= A11; ++pin) {
            switch(trial) {
                case 0:
                    if (pattern[pin - A0]) {
                        if (conditionPattern1[inputIndex]) {
                            digitalWrite(pin, HIGH);
                        } else {
                            digitalWrite(pin, LOW);
                        }
                        inputIndex++;
                    }
                    break;
                case 1:
                    if (pattern[pin - A0]) {
                        if (conditionPattern2[inputIndex]) {
                            digitalWrite(pin, HIGH);
                        } else {
                            digitalWrite(pin, LOW);
                        }
                        inputIndex++;
                    }
                    break;
                case 2:
                    if (pattern[pin - A0]) {
                        if (conditionPattern3[inputIndex]) {
                            digitalWrite(pin, HIGH);
                        } else {
                            digitalWrite(pin, LOW);
                        }
                        inputIndex++;
                    }
                    break;
                case 3:
                    if (pattern[pin - A0]) {
                        if (conditionPattern4[inputIndex]) {
                            digitalWrite(pin, HIGH);
                        } else {
                            digitalWrite(pin, LOW);
                        }
                        inputIndex++;
                    }
                    break;
        }
        int resultIndex = 0;
        for (int pin = A0; pin <= A11; ++pin) {
            if (!pattern[pin - A0]) {
                results[resultIndex] = digitalRead(pin);
                resultIndex++;
            }
        }
    }
}

void injectNOT(bool pattern[12], bool (&results)[6], int trial = 0) {
    int inputIndex = 0;
    for (int pin = A0; pin <= A11; ++pin) {
        if (pattern[pin - A0] && (trial == 0 || trial == 2)) {
            digitalWrite(pin, HIGH);
            inputIndex++;
        }
        else if (pattern[pin - A0] && (trial == 1 || trial == 3)) {
            digitalWrite(pin, LOW);
            inputIndex++;
        };
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
    int ICCount[6] = {0,0,0,0,0,0}; //AND, OR, NAND, XOR, NOT, NOR

    for (int i = 0; i < 4; ++i) {
        bool pattern[12];
        setPin(pinFormat, pattern);
        bool results[6];
        if (pinFormat == IN_OUT) {
            injectNOT(pattern, results, i);
            i++;
        } else injectInput(pattern, results, i);

        static bool FirstTest[5][4] = {
            {false,false,false,true}, //AND
            {false,true,true,true},    //OR
            {true,true,true,false},    //NAND
            {false,true,true,false},    //XOR
            {true,false,false,false} //NOR
        };

        static bool SecondTest[5][4] = {
            {false,false,true,false}, //AND
            {true,true,true,false},    //OR
            {true,true,false,true},    //NAND
            {true,true,false,false},    //XOR
            {false,false,false,true} //NOR
        };

        static bool ThirdTest[5][4] = {
            {false,true,false,false}, //AND
            {true,true,false,true},    //OR
            {true,false,true,true},    //NAND
            {true,false,false,true},    //XOR
            {false,false,true,false} //NOR
        };

        static bool FourthTest[5][4] = {
            {true,false,false,false}, //AND
            {true,false,true,true},    //OR
            {false,true,true,true},    //NAND
            {false,false,true,true},    //XOR
            {false,true,false,false} //NOR
        };

        static bool NOTTest[2][6] = {
            {false,false,false,false,false,false}, //NOT
            {true,true,true,true,true,true}   //NOT
        };

        if (pinFormat == IN_OUT) {
            bool match = true;
            for (int output = 0; output < 6; ++output) {
                if (results[output] != NOTTest[i % 2][output]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                ICCount[NOT]++;
            }
            continue;
        }
        else {
            for (int ic = 0; ic < 5; ++ic) {
                bool match = true;
                switch (i) {
                    case 0:
                        for (int output = 0; output < 4; ++output) {
                            if (results[output] != FirstTest[ic][output]) {
                                match = false;
                                break;
                            }
                        }
                        break;
                    case 1:
                        for (int output = 0; output < 4; ++output) {
                            if (results[output] != SecondTest[ic][output]) {
                                match = false;
                                break;
                            }
                        }
                        break;
                    case 2:
                        for (int output = 0; output < 4; ++output) {
                            if (results[output] != ThirdTest[ic][output]) {
                                match = false;
                                break;
                            }
                        }
                        break;
                    case 3:
                        for (int output = 0; output < 4; ++output) {
                            if (results[output] != FourthTest[ic][output]) {
                                match = false;
                                break;
                            }
                        }
                        break;
                }
                
                if (match) {
                    if (ic == 4) {
                        ICCount[NOR]++;
                    } else ICCount[static_cast<ICType>(ic)]++;
                }
            } 
        }
    }
    
    if (ICCount[AND] >= 4) {
        return AND;
    } 
    else if (ICCount[OR] >= 4) {
        return OR;
    } 
    else if (ICCount[NAND] >= 4) {
        return NAND;
    } 
    else if (ICCount[XOR] >=4) {
        return XOR;
    } 
    else if (ICCount[NOT] >=2) {
        return NOT;
    }
    else if (ICCount[NOR] >=4) {
        return NOR;
    }
    else {
        return RUSAK;
    }
}