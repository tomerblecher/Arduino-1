#ifndef TILT_H
#define TILT_H

#include <Arduino.h>
#include "Siren.h"

class Tilt {
public:
    // Constructor
    Tilt(int pin);

    // Method to get the current state of the tilt switch
    bool updateTilted();

    void turnOn();

    void turnOff();

    bool isLastTilted();

private:
    int tiltPin; // Pin connected to the tilt switch
    bool isPoweredOn;
    bool lastTilted;
};

#endif