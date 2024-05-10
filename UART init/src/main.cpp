#include "Arduino.h"

#define LED_PIN PC13   // LED connected to PC13
#define BAUD_RATE 9600 // UART baud rate

void setup() {
  pinMode(LED_PIN, OUTPUT);  // Set LED pin as output
  digitalWrite(LED_PIN, LOW);  // Turn off LED initially
  Serial1.begin(BAUD_RATE);  // Start UART communication at 9600 baud rate
}

void loop() {
  // Blink the LED
  digitalWrite(LED_PIN, HIGH); // Turn LED on
  delay(200);                  // Keep it on for 200 milliseconds
  digitalWrite(LED_PIN, LOW);  // Turn LED off
  delay(200);                  // Off for 200 milliseconds
  
  // Send a signal after blinking
  Serial1.println("blinked");  // Send a notification

  // Wait for the other Blue Pill to blink and send back a signal
  while (!Serial1.available());  // Wait until data is received
  
  String receivedMessage = Serial1.readString(); // Read the incoming data
  
  // Additional delay to separate the communication rounds visually
  delay(100);
}
