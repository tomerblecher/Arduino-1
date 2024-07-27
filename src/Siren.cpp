#include "Siren.h"

// Constructor
Siren::Siren(int pin)
{
    buzzerPin = pin;
    pinMode(buzzerPin, OUTPUT);
}

// Method to start the siren with a specified number of repeats
void Siren::startSiren(int repeats)
{
    Serial.println("Starting siren time: " + String(repeats));
    int minFrequency = 800;  // Minimum frequency of the siren
    int maxFrequency = 1100; // Maximum frequency of the siren
    int stepDelay = 5;       // Delay between frequency steps (in milliseconds)

    for (int i = 0; i < repeats; i++)
    {
        // Increase the frequency
        for (int freq = minFrequency; freq <= maxFrequency; freq += 1)
        {
            tone(buzzerPin, freq);
            delay(stepDelay);
        }
    }

    // Stop the tone after completing the repeats
    noTone(buzzerPin);
}

void Siren::bip(int repeats, int freq)
{
    Serial.println("Bipping " + String(repeats) + "times in " + String(freq) + "Khz");
    for (int i = 0; i < repeats; i++)
    {
        tone(buzzerPin, freq, 50);
        delay(10);
    }
    noTone(buzzerPin);
}

void Siren::startSound(int freq)
{
    // Avoid running tone multiple times when no changes were made

    if (this->isToneOn)
    {
        if (abs(freq - this->currentBuzzerFreq) < 100)
        {
            return;
        }
    }
    Serial.println("Starting sound on FREQ: " + String(freq)); 
    tone(buzzerPin, freq);
    this->currentBuzzerFreq = freq;
    this->isToneOn = true;
}

void Siren::stopSound()
{
    Serial.println("Stopping sound");
    noTone(buzzerPin);
    this->isToneOn = false;
}