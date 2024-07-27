#include "Tilt.h"

// Constructor
Tilt::Tilt(int pin): tiltPin(pin) {
    pinMode(tiltPin, INPUT);
}


// Method to get the current state of the tilt switch
bool Tilt::updateTilted() {
    bool isTilted = digitalRead(tiltPin) == HIGH;
    this->lastTilted = isTilted;
    return isTilted;
}

void Tilt::turnOn() {
    if (isPoweredOn) {
        return;
    }

    digitalWrite(tiltPin, HIGH);
    isPoweredOn = true;
}

void Tilt::turnOff() {
    if (!isPoweredOn) {
        return;
    }
    isPoweredOn = false;
    digitalWrite(tiltPin, LOW);
}

bool Tilt::isLastTilted() {
    return this->lastTilted;
}