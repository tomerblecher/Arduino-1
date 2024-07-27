#include <Arduino.h>
#include "SR04.h"
#include "Siren.h"
#include "pitches.h"

// put function declarations here:
void setRGBColors(int r, int g, int b, bool sonarScan = true);
bool handleButtonPush();
void testLEDs();
void sonarTest();
double calcBrightnessDivision();
// www.elegoo.com
// 2016.12.8

// Define Pins
#define RED 11
#define GREEN 10
#define BLUE 9
#define BUTTON 13
#define delayTime 0 // fading time between colors
#define SONAR_TRIGGER 7
#define SONAR_ECHO 6
#define BUZZER 3

#define BUZZER_FREQ_START NOTE_C4
#define BUZZER_FREQ_END NOTE_B5

SR04 mySonar(SONAR_ECHO, SONAR_TRIGGER);
Siren mySiren(BUZZER);

void setup()
{
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);
    pinMode(BUTTON, INPUT_PULLUP);
    pinMode(BUZZER, OUTPUT);

    Serial.begin(9600); // Initialize serial communication at 9600 baud
    Serial.println("Setup complete");
    // testLEDs();
    // sonarTest();
    mySiren.bip(1);
}

// define variables
int redValue;
int greenValue;
int blueValue;
bool isOn = true;
unsigned long debounceDelay = 100;  // Debounce time (50 ms is usually sufficient)
unsigned long lastDebounceTime = 0; // Variable to store the last debounce time

// main loop
void loop()
{
    if (!handleButtonPush())
    {
        return;
    }
    redValue = 255; // choose a value between 1 and 255 to change the color.
    greenValue = 0;
    blueValue = 0;

    // this is unnecessary as we've either turned on RED in SETUP
    // or in the previous loop ... regardless, this turns RED off
    // analogWrite(RED, 0);
    // delay(1000);
    setRGBColors(redValue, greenValue, blueValue);

    for (int i = 0; i < 255; i += 1) // fades out red bring green full when i=255
    {
        if (!handleButtonPush())
        {
            return;
        }
        redValue -= 1;
        greenValue += 1;
        setRGBColors(redValue, greenValue, blueValue);
        // The following was reversed, counting in the wrong directions
        // analogWrite(RED, 255 - redValue);
        // analogWrite(GREEN, 255 - greenValue);
        delay(delayTime);
    }

    redValue = 0;
    greenValue = 255;
    blueValue = 0;
    setRGBColors(redValue, greenValue, blueValue);

    for (int i = 0; i < 255; i += 1) // fades out green bring blue full when i=255
    {
        if (!handleButtonPush())
        {
            return;
        }
        greenValue -= 1;
        blueValue += 1;
        // The following was reversed, counting in the wrong directions
        // analogWrite(GREEN, 255 - greenValue);
        // analogWrite(BLUE, 255 - blueValue);
        setRGBColors(redValue, greenValue, blueValue);
        delay(delayTime);
    }

    redValue = 0;
    greenValue = 0;
    blueValue = 255;
    setRGBColors(redValue, greenValue, blueValue);

    for (int i = 0; i < 255; i += 1) // fades out blue bring red full when i=255
    {
        if (!handleButtonPush())
        {
            return;
        }
        // The following code has been rearranged to match the other two similar sections
        blueValue -= 1;
        redValue += 1;
        // The following was reversed, counting in the wrong directions
        // analogWrite(BLUE, 255 - blueValue);
        // analogWrite(RED, 255 - redValue);
        setRGBColors(redValue, greenValue, blueValue);
        delay(delayTime);
    }
}

void setRGBColors(int r, int g, int b, bool sonarScan)
{
    if (sonarScan)
    {
        double brightFactor = calcBrightnessDivision();
        r = r * brightFactor;
        g = g * brightFactor;
        b = b * brightFactor;
        int freqRadius = BUZZER_FREQ_END - BUZZER_FREQ_START;
        Serial.println("Brightness factor: " + String(brightFactor) + ", Freq: " + String(freqRadius * brightFactor));
        mySiren.startSound((freqRadius * brightFactor) + BUZZER_FREQ_START);
    }

    String log = "Red: " + String(r) + " Green: " + String(g) + " blue: " + String(b);
    analogWrite(RED, r);
    analogWrite(GREEN, g);
    analogWrite(BLUE, b);
}

bool handleButtonPush()
{
    // Read the current state of the button
    int reading = digitalRead(BUTTON);
    if (reading == HIGH)
    {
        return isOn;
    }

    // Check if the button state has changed (debouncing)

    if ((millis() - lastDebounceTime) > debounceDelay)
    {
        lastDebounceTime = millis();
        // If the button state has stabilized after the debounce delay
        isOn = !isOn;
        Serial.println("Button pressed, isOn: " + String(isOn));
        if (!isOn)
        {
            setRGBColors(0, 0, 0);
            mySiren.stopSound();
        }
    }

    return isOn;
}

void testLEDs()
{
    setRGBColors(255, 0, 0, false);
    delay(500);
    setRGBColors(0, 255, 0, false);
    delay(500);
    setRGBColors(0, 0, 255, false);
    delay(500);
}

void sonarTest()
{
    Serial.println("Activating sonar test");
    long distanceCM = mySonar.DistanceAvg();
    Serial.println("Detected object " + String(distanceCM) + "cm away");
}

double calcBrightnessDivision()
{
    long distance = mySonar.DistanceAvg();
    Serial.println("Distance: " + String(distance) + "cm");
    int maxDistance = 20;
    if (distance > maxDistance)
    {
        return 0;
    }
    else
    {
        return 1 - (float)distance / maxDistance;
    }
}