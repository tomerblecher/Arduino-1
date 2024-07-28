#include <Arduino.h>
#include "SR04.h"
#include "Siren.h"
#include "pitches.h"
#include "dht_nonblocking.h"
#include "LiquidCrystal.h"

// put function declarations here:
void setRGBColors(int r, int g, int b, bool sonarScan = true);
bool handleButtonPush();
void testLEDs();
// www.elegoo.com
// 2016.12.8

// Define Pins
#define RED 11
#define GREEN 10
#define BLUE 9
#define BUTTON 13
#define delayTime 30 // fading time between colors
#define SONAR_TRIGGER 7
#define SONAR_ECHO 6
#define THERMO 8
#define LCD_RS 2
#define LCD_E 3
#define LCD_D4 4
#define LCD_D5 5
#define LCD_D6 6
#define LCD_D7 7
#define DHT_SENSOR_TYPE DHT_TYPE_11

// define variables
int redValue;
int greenValue;
int blueValue;
bool isOn = true;
unsigned long debounceDelay = 100;  // Debounce time (50 ms is usually sufficient)
unsigned long lastDebounceTime = 0; // Variable to store the last debounce time

DHT_nonblocking dht_sensor(THERMO, DHT_SENSOR_TYPE);
LiquidCrystal lcdScreen(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

void setup()
{
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);
    pinMode(BUTTON, INPUT_PULLUP);

    Serial.begin(9600); // Initialize serial communication at 9600 baud
    Serial.println("Setup complete");
    lcdScreen.begin(16, 2);   
    lcdScreen.write("Loading..");
}

void setRGBColors(int r, int g, int b, bool sonarScan)
{
    String log = "Red: " + String(r) + " Green: " + String(g) + " blue: " + String(b);
    analogWrite(RED, r);
    analogWrite(GREEN, g);
    analogWrite(BLUE, b);
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

static bool measure_environment( float *temperature, float *humidity )
{
  static unsigned long measurement_timestamp = millis( );

  /* Measure once every four seconds. */
  if( millis( ) - measurement_timestamp > 3000ul )
  {
    if( dht_sensor.measure( temperature, humidity ) == true )
    {
      measurement_timestamp = millis( );
      return( true );
    } else {
        setRGBColors(0, 0, 255);
    }
  } else {
    setRGBColors(255, 0, 0);
  }

  return( false );
}

// main loop
void loop()
{
   float temp, humidity;
   if (measure_environment(&temp, &humidity)) {
        Serial.println("Temp: " + String(temp) + ", Humidity: " + humidity);
        lcdScreen.clear();
        lcdScreen.setCursor(0, 0);
        lcdScreen.print("Temp: " + String(temp) + "C" );
        lcdScreen.setCursor(0, 1);
        lcdScreen.print("Humidity: " + String(humidity) + "%");
   }
}