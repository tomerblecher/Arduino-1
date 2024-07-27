#include "Siren.h"

// Constructor
Siren::Siren(int pin) {
    buzzerPin = pin;
    pinMode(buzzerPin, OUTPUT);
}

// Method to start the siren with a specified number of repeats
void Siren::startSiren(int repeats) {
    int minFrequency = 900;   // Minimum frequency of the siren
    int maxFrequency = 1100;  // Maximum frequency of the siren
    int stepDelay = 1;       // Delay between frequency steps (in milliseconds)

    for (int i = 0; i < repeats; i++) {
        // Increase the frequency
        for (int freq = minFrequency; freq <= maxFrequency; freq += 1) {
            tone(buzzerPin, freq);
            delay(stepDelay);
        }
    }

    // Stop the tone after completing the repeats
    noTone(buzzerPin);
}

void Siren::bip(int repeats) {
    for (int i = 0; i < repeats; i++) {
        tone(buzzerPin, 1450, 50);
       delay(10);
    }
    noTone(buzzerPin);
}

void Siren::startSound(int freq) {
    tone(buzzerPin, freq);
}

void Siren::stopSound() {
    noTone(buzzerPin);
}