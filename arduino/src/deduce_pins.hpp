#ifndef DEDUCE_PINS_HPP
#define DEDUCE_PINS_HPP

enum PinFormat {
    IN_OUT,
    IN_IN_OUT,
    OUT_IN_IN
};

PinFormat deducePins();

#endif