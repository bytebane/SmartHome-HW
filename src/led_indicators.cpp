#include <led_indicators.h>
#include <Arduino.h>
#include <constants.h>

void setupStartLED()
{
  // Solution for PINS ON on Setup
  digitalWrite(PIN1, OFF);
  digitalWrite(PIN2, OFF);
  digitalWrite(PIN3, OFF);
  digitalWrite(PIN4, OFF);
  digitalWrite(PIN5, OFF);
  digitalWrite(PIN6, OFF);
  digitalWrite(PIN7, OFF);
  digitalWrite(PIN8, OFF);

  digitalWrite(PIN_R, HIGH);
  digitalWrite(PIN_G, HIGH);
  digitalWrite(PIN_Y, HIGH);
}

void setupEndLED()
{
  digitalWrite(PIN_R, LOW);
  digitalWrite(PIN_G, LOW);
  digitalWrite(PIN_Y, LOW);
}

void wifiConnectedLED()
{
  digitalWrite(PIN_R, LOW);
  digitalWrite(PIN_Y, HIGH);
}

void dbConnectedLED()
{
  digitalWrite(PIN_G, HIGH);
}

void wifiErrorLED()
{
  digitalWrite(PIN_R, HIGH);
  digitalWrite(PIN_G, LOW);
  digitalWrite(PIN_Y, LOW);
}

void dbErrorLED()
{
  digitalWrite(PIN_G, LOW);
  digitalWrite(PIN_R, HIGH);
}
