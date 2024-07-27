#include <Arduino.h>
#include "Siren.h"
#include "Tilt.h"
#include <Servo.h>
#include "Enums.h"

// put function declarations here:
int myFunction(int, int);
void setRGBColors(int, int, int);
bool handleButtonPush();
void testLEDs();
void buzz(int);
void handleTiltButton();
void spinFan(SpinDirection direction);

// www.elegoo.com
// 2016.12.8

// Define Pins
#define RED 11
#define SERVO 3
#define BUZZER_ACT 2
#define GREEN 10
#define BLUE 9
#define BUTTON 13
#define BUZZER_PASS 4
#define TILE_INPUT 7
#define BUTTON_TILT 8
#define delayTime 10 // fading time between colors

Servo myServo;

void setup()
{
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, LOW);
    pinMode(BUTTON, INPUT_PULLUP);
    pinMode(BUTTON_TILT, INPUT_PULLUP);
    Serial.begin(9600); // Initialize serial communication at 9600 baud
    Serial.println("Setup complete");
    testLEDs();
    myServo.attach(SERVO);
    spinFan(SpinDirection::RIGHT);
    spinFan(SpinDirection::LEFT);
}

// define variables
int redValue;
int greenValue;
int blueValue;
bool isOn = true;
unsigned long debounceDelay = 100;  // Debounce time (50 ms is usually sufficient)
unsigned long lastDebounceTime = 0; // Variable to store the last debounce time
Tilt tiltInput(TILE_INPUT);

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

void setRGBColors(int r, int g, int b)
{
    String log = "Red: " + String(r) + " Green: " + String(g) + " blue: " + String(b);
    analogWrite(RED, r);
    analogWrite(GREEN, g);
    analogWrite(BLUE, b);
}

bool handleButtonPush()
{
    handleTiltButton();
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
            buzz(50);
            delay(100);
            buzz(50);
        }
    }

    return isOn;
}

void testLEDs()
{
    setRGBColors(255, 0, 0);
    delay(500);
    setRGBColors(0, 255, 0);
    delay(500);
    setRGBColors(0, 0, 255);
    delay(500);
}

void buzz(int miliseconds)
{
    digitalWrite(BUZZER_ACT, HIGH);
    delay(miliseconds);
    digitalWrite(BUZZER_ACT, LOW);
}

void handleTiltButton()
{
    int buttonState = digitalRead(BUTTON_TILT);
    if (buttonState == LOW)
    {
        tiltInput.turnOn();
        bool isSameResult = tiltInput.isLastTilted() == tiltInput.updateTilted();

        if(isSameResult) {
            return;
        }
        Serial.println(isSameResult);

        if (tiltInput.isLastTilted())
        {
            Serial.println("UPP");
        }
        else
        {
            Serial.println("DOWN");
        }
    }
    else
    {
        tiltInput.turnOff();
    }
}

void spinFan(SpinDirection direction) {
    Serial.println("Starting fan rotation to side: " + String(direction));
        if (direction == RIGHT) {
        for (int pos = 0; pos <= 180; pos++) {
            myServo.write(pos);
            delay(15); // Adjust delay for smooth movement
        }
    } else if (direction == LEFT) {
        for (int pos = 180; pos >= 0; pos--) {
            myServo.write(pos);
            delay(15); // Adjust delay for smooth movement
        }
    }

  Serial.println("Finishing fan rotation");
}