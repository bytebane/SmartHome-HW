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
  delay(300);
  digitalWrite(PIN_Y, HIGH);
  delay(300);
  digitalWrite(PIN_G, HIGH);
}

// Pattern R->G->Y
void setupSmartConfigLED()
{
  digitalWrite(PIN_R, HIGH);
  digitalWrite(PIN_G, LOW);
  digitalWrite(PIN_Y, LOW);
  delay(500);
  digitalWrite(PIN_R, LOW);
  digitalWrite(PIN_G, HIGH);
  digitalWrite(PIN_Y, LOW);
  delay(500);
  digitalWrite(PIN_R, LOW);
  digitalWrite(PIN_G, LOW);
  digitalWrite(PIN_Y, HIGH);
  delay(500);
}

// Pattern Y->G->Y->R
void setupWifiLED()
{
  digitalWrite(PIN_R, LOW);
  digitalWrite(PIN_G, LOW);
  digitalWrite(PIN_Y, HIGH);
  delay(500);
  digitalWrite(PIN_R, LOW);
  digitalWrite(PIN_G, HIGH);
  digitalWrite(PIN_Y, LOW);
  delay(500);
  digitalWrite(PIN_R, LOW);
  digitalWrite(PIN_G, LOW);
  digitalWrite(PIN_Y, HIGH);
  delay(500);
  digitalWrite(PIN_R, HIGH);
  digitalWrite(PIN_G, LOW);
  digitalWrite(PIN_Y, LOW);
  delay(500);
}

void setupEndLED()
{
  digitalWrite(PIN_R, HIGH);
  digitalWrite(PIN_G, HIGH);
  digitalWrite(PIN_Y, HIGH);
  delay(500);
  digitalWrite(PIN_R, LOW);
  digitalWrite(PIN_G, LOW);
  digitalWrite(PIN_Y, LOW);
  delay(500);
  digitalWrite(PIN_R, HIGH);
  digitalWrite(PIN_G, HIGH);
  digitalWrite(PIN_Y, HIGH);
  delay(500);
  digitalWrite(PIN_R, LOW);
  digitalWrite(PIN_G, LOW);
  digitalWrite(PIN_Y, LOW);
}

void wifiConnectedLED()
{
  digitalWrite(PIN_R, LOW);
  digitalWrite(PIN_G, LOW);
  digitalWrite(PIN_Y, HIGH);
}

void dbConnectedLED()
{
  digitalWrite(PIN_R, LOW);
  digitalWrite(PIN_G, HIGH);
  digitalWrite(PIN_Y, LOW);
}

void wifiErrorLED()
{
  digitalWrite(PIN_R, HIGH);
  digitalWrite(PIN_G, LOW);
  digitalWrite(PIN_Y, HIGH);
}

void dbErrorLED()
{
  digitalWrite(PIN_R, HIGH);
  digitalWrite(PIN_G, HIGH);
  digitalWrite(PIN_Y, LOW);
}
