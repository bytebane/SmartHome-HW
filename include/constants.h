#ifndef CONSTANTS_H
#define CONSTANTS_H

#define WiFi_rst 0 // WiFi Reset pin

// LED Indicator Pins - Output pins
#define PIN_R 21
#define PIN_G 23
#define PIN_Y 22

// Relay Pins - Output pins
#define PIN1 16 // Light Pin
#define PIN2 17 // Light Pin
#define PIN3 18 // Light Pin
#define PIN4 19 // Light Pin
#define PIN5 13 // Unused Pin
#define PIN6 12 // Unused Pin
#define PIN7 14 // Unused Pin
#define PIN8 27 // Fan Pin

#define LENGTH(x) (strlen(x) + 1)
#define EEPROM_SIZE 200

// Manual Control Pins - Input pins
// #define IN_PIN1 36
// #define IN_PIN2 39
// #define IN_PIN3 34
// #define IN_PIN4 35
// #define IN_PIN5 32

#define ON LOW
#define OFF HIGH

#endif
