// #include <Wire.h>  // Include the Wire library to use I2C functions
// #include <Arduino.h>

// const int LED_PIN = PC13;

// void setup() {
//   pinMode(LED_PIN, OUTPUT);
//   Wire.begin(); // Start I2C as master
//   digitalWrite(LED_PIN, HIGH);
// }

// void loop() {
//   static int x = 0; // Variable to send
//   Wire.beginTransmission(9); // Begin transmission to the device with address 9
//   Wire.write(x);             // Send the variable x
//   Wire.endTransmission();    // End the transmission


//   x++; // Increment x
//   if (x > 5) x = 0; // Reset x to 0 if it exceeds 5
//   digitalWrite(LED_PIN, HIGH);
//   delay(250);
//   digitalWrite(LED_PIN, LOW);
//   delay(250);
// }



#include <Wire.h>
#include <Arduino.h>

const int LED_PIN = PC13; // LED pin (built-in LED on PC13)
volatile int receivedData = 0; // Variable to hold received data
int last = -1;


void receiveEvent(int howMany) {
  while (Wire.available()) { // Loop while there is data available
    receivedData = Wire.read(); // Read the data
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT); // Set the LED pin as output
  Wire.begin(9); // Start I2C as slave on address 9
  Wire.onReceive(receiveEvent); // Register a function to call when data is received
}


void loop() {
  // Blink LED for 200ms if the received data is 0

  if (receivedData != last) {
    digitalWrite(LED_PIN, HIGH);
    delay(250);
    digitalWrite(LED_PIN, LOW);
    delay(250);
  }
  // Blink LED for 400ms if the received data is 3
  else {
    digitalWrite(LED_PIN, LOW);
  }
  last = receivedData;
}

