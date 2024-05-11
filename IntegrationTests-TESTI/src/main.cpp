#include "Arduino.h"

#define LED_PIN PC13   // LED connected to PC13
#define BAUD_RATE 9600 // UART baud rate

void setup() {
  pinMode(LED_PIN, OUTPUT);  // Set LED pin as output
  digitalWrite(LED_PIN, LOW);  // Turn off LED initially
  Serial1.begin(BAUD_RATE);  // Start UART communication at 9600 baud rate
  
  while (!Serial1.available());  // Wait for handshake initiation from PAULO
  int receivedMessage = Serial1.parseInt(); // Read the incoming data
  if (receivedMessage== 101) {
	  digitalWrite(LED_PIN, HIGH); // Turn LED on
	  Serial1.println(102);  // Send back handshake confirmation
	  delay(250);
	  digitalWrite(LED_PIN, LOW);  // Turn LED off
  }
}

void loop() {
}
