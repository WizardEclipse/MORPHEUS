#include "Arduino.h"

// Define the pins where the LEDs are connected
int ledPins[] = {PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7, PA8, PA9, PA10, PA11, PA12, PA13, PA14, PA15,
                 PB0, PB1, PB3, PB4, PB5, PB6, PB7, PB8, PB9, PB10, PB11, PB12, PB13};
int numPins = sizeof(ledPins) / sizeof(ledPins[0]);  // Number of pins

/**
 * @brief Initialize all LED pins as digital write.
 */
void setup() {
  for (int i = 0; i < numPins; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  for (int i = 0; i < numPins; i++) {
    digitalWrite(ledPins[i], LOW);  // Turn LED off
    // Wait for one second before moving to the next LED
  }
}

/**
 * @brief Turn each LED on for 1 second and then off in sequence.
 */
void loop() {
  for (int i = 0; i < numPins; i++) {
    digitalWrite(ledPins[i], HIGH);  // Turn LED on
    delay(100);  // Wait for one second
  }
  for (int i = 0; i < numPins; i++) {
    digitalWrite(ledPins[i], LOW);  // Turn LED off
    delay(100);  // Wait for one second before moving to the next LED
  }
}
