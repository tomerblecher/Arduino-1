#ifndef SIREN_H
#define SIREN_H

#include <Arduino.h>

class Siren {
public:
    // Constructor
    Siren(int pin);

    // Method to start the siren with a specified number of repeats
    void startSiren(int repeats);
    void bip(int repeat);
    void startSound(int freq);
    void stopSound();

private:
    int buzzerPin; // Pin connected to the buzzer
};

#endif
