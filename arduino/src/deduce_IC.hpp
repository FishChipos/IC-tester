#ifndef DEDUCE_IC_HPP
#define DEDUCE_IC_HPP
#include "deduce_pins.hpp"

enum ICType {
    AND,
    OR,
    NAND,
    XOR,
    RUSAK
};

ICType deduceIC(PinFormat pinFormat);

#endif