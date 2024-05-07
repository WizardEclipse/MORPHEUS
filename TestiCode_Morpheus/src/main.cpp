/*
Testi's code
*/

#include <Arduino.h>

int PWMVAL = 155;
int ADCMIN = 2000;
int ADCMAX = 3000;

int gpioPins[] = {PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7, PA8, PA9, PA10, PA11, PA12, PA13, PA14, PA15,
                 PB0, PB1, PB3, PB4, PB5, PB6, PB7, PB8, PB9, PB10, PB11, PB12, PB13};

int adcPins[] = {PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7, PB0, PB1};

int pwmPins[] = {PA_0, PA_1, PA_2, PA_3, PA_6, PA_7, PA_8, PA_9, PA_10, PB_0, PB_1, PB_6, PB_7, PB_8, PB_9};

void setup() {
  // setup communication with Paulo
}

void loop() {
  // listen to instructions from Paulo
  // testGPIO();
  // testADC();
  // testPWM();
}

void testGPIO() {
  for (int pinName : gpioPins) {
    pinMode(pinName, OUTPUT);
    digitalWrite(pinName, HIGH);
  }
  delay(1000); // Wait for 1000 millisecond(s) - 1 second
  for (int pinName : gpioPins) {
    digitalWrite(pinName, LOW);
  }
}

void testADC() {
  for (int pinName : adcPins) {
    pinMode(pinName, INPUT);
  }
  delay(1000); // Wait for 1000 millisecond(s) - 1 second
  for (int pinName : adcPins) {
    int readValue = analogRead(pinName);
    if (readValue > ADCMIN && readValue < ADCMAX) {
      pinMode(pinName, OUTPUT);
      digitalWrite(pinName, HIGH);
    } else {
      pinMode(pinName, OUTPUT);
      digitalWrite(pinName, LOW);
    }
  }
}

void testPWM() {
  for (int pinName : pwmPins) {
    pinMode(pinName, OUTPUT);
    analogWrite(pinName, PWMVAL);
  }
  delay(1000); // Wait for 1000 millisecond(s) - 1 second
  for (int pinName : pwmPins) {
    analogWrite(pinName, 0);
  }
}